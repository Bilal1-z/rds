#include "Registry.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

using redisgrpc::Registry;
using redisgrpc::lib::Cache;

void Registry::Init(std::string connection_id){
    this->_init(connection_id);
    _logger.log(absl::StrFormat("Cache initialized with connection id %s", connection_id));
}

std::pair<bool,std::string> Registry::Get(std::string uniqueID, std::string key){
    _logger.log(absl::StrFormat("Getting Value for Key:%s", key));
    const auto& cachePtr = this->_getInstance(uniqueID);
    return cachePtr->Get(key);
}

void Registry::Set(std::string uniqueID, std::string key, std::string value){
     _logger.log(absl::StrFormat("Setting Value:%s for Key:%s ",value, key));
    const auto& cachePtr = this->_getInstance(uniqueID);
    cachePtr->Set(key, value);
}

std::unordered_map<std::string, std::string> Registry::getDataWithoutFreq(std::string uniqueID){
    const auto& cachePtr = this->_getInstance(uniqueID);
    return cachePtr->getDataWithoutFreq();
}


void Registry::_init(const std::string& uniqueID){
    this->_uniqueIDToCacheMap[uniqueID] = std::make_shared<Cache>(Cache());
}

std::shared_ptr<Cache> Registry::_getInstance(const std::string& uniqueID){
    if(this->_uniqueIDToCacheMap.find(uniqueID) != this->_uniqueIDToCacheMap.end()){
        return this->_uniqueIDToCacheMap[uniqueID];
    }
    this->_uniqueIDToCacheMap[uniqueID] = std::make_shared<Cache>(Cache());
    return this->_uniqueIDToCacheMap[uniqueID];
}