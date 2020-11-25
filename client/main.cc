
#include<unistd.h>
#include<stdio.h>

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "ping.grpc.pb.h"


#include"../common/zk_discovery.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using ping::PingRequest;
using ping::PingReply;
using ping::Ping;

#include "ping_client.h"

const char* host_list = "127.0.0.1:2181";
const int time_out = 3000;
const string path = "/ping";

int main(int argc, char** argv) {
	printf("=======================start discovery=====================\n");
	zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
	std::shared_ptr<ZkHandle> pZkhandle(new ZkHandle);
	if(pZkhandle->zkInit(host_list, time_out) == 0){
		printf("pZkhandle zkInit ok \n");
	}

	ZKDiscovery zkDiscovery(path,pZkhandle);
	zkDiscovery.zkDoDiscovery();
	std::map<string, ZkNodeInfo> nodes = zkDiscovery.getPathToNodes();

	//具体节点信息
	for (auto node : nodes){
		printf("path = %s, value = %s\n",node.first.c_str(), node.second.ToString().c_str());
	}
	if(nodes.empty()){
		printf("There is no nodes ok \n");
		return -1;
	}
	printf("=======================start client connect=====================\n");
	//默认选择第一个ip
	ZkNodeInfo zkNode = nodes.begin()->second;
	std::string address = zkNode.m_ip+":"+zkNode.m_port;
	printf("client connect to %s\n", address.c_str());
	PingClient ping(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
	std::string cmd("ping -c 1 -W 2 www.baidu.com");
	std::string reply = ping.sendPing(cmd);
	std::cout << "ping received: " << reply << std::endl;
	return 0;
}
