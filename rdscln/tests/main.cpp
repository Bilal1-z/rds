#include <iostream>

#include <memory>
#include <string>
#include <thread>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "Logger.h"
#include "Controller.h"



using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using redisgrpc::lib::Logger;
using redisgrpc::Controller;
using redisgrpc::Registry;



int main(int argc, char* argv[]) {
    // Initialize empty registry
    //Registry r;
    // Set default port value
    uint16_t portValue = 50051;
    // Parse command line arg
    if(argc == 3){
        if (strcmp(argv[1], "-p") == 0){
            portValue = static_cast<uint16_t>(std::stoi(argv[2]));

        }
    }

    // Construct a unique server address
    std::string server_address = absl::StrFormat("0.0.0.0:%d", portValue);
    // Instantiate a controller
    Controller c(server_address);
    // Run server at the port number
    std::thread r([&](){
        c.RunServer();
    });
    r.join();
    return 0;
}