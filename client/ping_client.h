
#ifndef CLIENT_PING_CLIENT_H_
#define CLIENT_PING_CLIENT_H_


#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "ping.grpc.pb.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using ping::PingRequest;
using ping::PingReply;
using ping::Ping;

class PingClient {
 public:
  PingClient(std::shared_ptr<Channel> channel)
      : stub_(Ping::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string sendPing(const std::string& ping) {
    // Data we are sending to the server.
	PingRequest request;
    request.set_ping(ping);

    // Container for the data we expect from the server.
    PingReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->sendPing(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.pong();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<Ping::Stub> stub_;
};


#endif /* CLIENT_PING_CLIENT_H_ */
