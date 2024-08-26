

#ifndef REDISGRPC_CONTROLLER_H
#define REDISGRPC_CONTROLLER_H

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <time.h>

#include "Registry.h"
#include "Macros.h"
#include "redisgrpc.grpc.pb.h"
#include "redisgrpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

using redisgrpc::RedisGrpcServer;
using redisgrpc::InitRequest;
using redisgrpc::InitReply;
using redisgrpc::SetRequest;
using redisgrpc::SetReply;

#include "RedisGrpcServiceImpl.h"
#include "GlobalState.h"
#include "ControllerInterface.h"

namespace redisgrpc{
    class Controller: public ControllerInterface{
    public:
        Controller(std::string connectionId): _registry(new Registry()), _connectionID(connectionId){
            GlobalState::sClientAddressToController[connectionId] = this;
            this->Init();
        };

      
        void Init(){
            _registry->Init(this->_connectionID);
        }

        void RunServer() override{
            RedisGrpcServiceImpl service (_connectionID, _registry);

            grpc::EnableDefaultHealthCheckService(true);

            grpc::reflection::InitProtoReflectionServerBuilderPlugin();

            ServerBuilder builder;
           .
            builder.AddListeningPort(_connectionID, grpc::InsecureServerCredentials());
            
            builder.RegisterService(&service);
            
            server = builder.BuildAndStart();
            std::cout << "RedisGrpc server listening on " << _connectionID << std::endl;
            _serverStatus = SERVER_STATUS::RUNNING;
            
            server->Wait();
        }

       
        void ShutDown(bool& serverStopped) override{
            std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(100);
            std::thread t([&](){
                this->server->Shutdown(deadline);
            });
            t.detach();
            _serverStatus = SERVER_STATUS::KILLED;
            serverStopped = true;
        }

        // Mainly used for testing
        inline std::string getConnectionID() const {
            return this->_connectionID;
        }

        // Get server status
        inline SERVER_STATUS getServerStatus() const{
            return this->_serverStatus;
        }

    private:
        std::shared_ptr<Registry> _registry; 
        std::string _connectionID; 
        std::unique_ptr<Server> server; 
        SERVER_STATUS _serverStatus = SERVER_STATUS::NOT_STARTED;

    };


} 

#endif 