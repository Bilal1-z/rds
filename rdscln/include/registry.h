#ifndef REDISGRPC_REGISTRY_H
#define REDISGRPC_REGISTRY_H

#include <string>
#include <memory>
#include <utility>
#include <unordered_map>

#include "Cache.hpp"
#include "Logger.h"

namespace redisgrpc{
    class Registry{
    private:
        redisgrpc::lib::Logger _logger;
    public:
       
        Registry() = default;

       
        void Init(std::string connection_id);

        std::pair<bool,std::string> Get(std::string uniqueID, std::string key);

   
        void Set(std::string uniqueID, std::string key, std::string value);

       
        std::unordered_map<std::string, std::string> getDataWithoutFreq(std::string uniqueID);

    private:
       
        void _init(const std::string& uniqueID);

      
        std::shared_ptr<redisgrpc::lib::Cache> _getInstance(const std::string& uniqueID);

        
        std::unordered_map<std::string, std::shared_ptr<redisgrpc::lib::Cache>> _uniqueIDToCacheMap;
    };
}
#endif 