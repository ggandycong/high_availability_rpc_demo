#ifndef SERVER_PING_SERVERIMPL_H_
#define SERVER_PING_SERVERIMPL_H_
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "ping.grpc.pb.h"


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using ping::PingRequest;
using ping::PingReply;
using ping::Ping;

// Logic and data behind the server's behavior.
class PingServiceImpl final : public Ping::Service {

  Status sendPing(ServerContext* context, const PingRequest* request,
		  	  PingReply* reply) override {
	std::string pingResult;
	int ret = runShellCommand(request->ping(), pingResult);
	if(0 != ret){
		std::cerr << "Failed to call runShellCommand  "<< std::endl;
		return Status::CANCELLED;
	}
	reply->set_pong(pingResult);
	return Status::OK;
  }
  int runShellCommand(const std::string& cmd, std::string& result) {
		FILE *fp;
		if ((fp = popen(cmd.c_str(), "r")) == NULL) {
			result = std::string("failed to popen");
			return -1;
		}
		int bufflen = 1024;
		char buff[bufflen];
		while (fgets(buff, bufflen, fp) != NULL) {
			result += std::string(buff);
		}

		if (pclose(fp) != 0) {
			result = std::string("Failed to close command stream");
			return -1;
		}
		return 0;
	}
};

#endif
