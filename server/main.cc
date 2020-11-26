
#include<unistd.h>
#include<stdio.h>


#include <iostream>
#include <memory>
#include <string>
#include<set>
#include<sstream>
#include<fstream>


#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "ping.grpc.pb.h"

#include "ping_serverImpl.h"

#include"../common/zk_handle.h"
#include"../common/zk_register.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using ping::PingRequest;
using ping::PingReply;
using ping::Ping;


const char* host_list = "127.0.0.1:2181";
const int time_out = 3000;
//临时节点,而且不是序列化
//std::string nodeMsg = "IP[127.0.0.1]PORT[12345]ZK_PATH[/ping/host1]IS_EPHEMERAL[1]IS_SEQUENCE[0]";


void RunServer(const std::string& server_address) {
  PingServiceImpl service;
  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}
std::string readFile(const std::string fileName){
	std::ifstream t(fileName);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}
int main(int argc, char** argv) {
	if(argc<2){
		printf("%s, file.conf \n", argv[0]);
		return -1;
	}
	printf("=======================start register server=====================\n");
	zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
	struct Stat stat;
	set<string> node_list;
	string path_info;
	std::string raw_name;

	std::shared_ptr<ZkHandle> pZkhandle(new ZkHandle);
	if(pZkhandle->zkInit(host_list, time_out) == 0){
		printf("pZkhandle zkInit ok \n");
	}

	//创建永久节点
	if(pZkhandle->zkExists("/ping", stat)!=0){
		pZkhandle->zkCreateNode("/ping", "this is a server", false,false,  raw_name);
	}

	//注册节点
	ZKRegister zkRegister(pZkhandle);
	ZkNodeInfo zkNode(readFile(argv[1]));
	zkRegister.setZkNode(zkNode);
	zkRegister.zkDoRegister();

	pZkhandle->zkGetChildren("/",node_list);
	pZkhandle->zkGetNodeInfo(zkNode.m_zk_path.c_str(),path_info,stat);


	printf("=======================start ping server=====================\n");
	//运行服务
	std::string address = zkNode.m_ip+":"+zkNode.m_port;
	RunServer(address);
	return 0;
}
