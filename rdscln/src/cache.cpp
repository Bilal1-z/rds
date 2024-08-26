#include "Cache.hpp"
#include "Macros.h"
#include <cassert>

#include "FileSystem.h"

using redisgrpc::lib::Cache;
using redisgrpc::lib::FileSystem;


std::pair<bool,std::string> Cache::Get(std::string key){
   
    if(this->_data.find(key) == this->_data.end()){
        return std::make_pair(false, __GET_ERROR__);
    }

   
    int oldFreq = this->_data[key].second;

    
    this->_data[key].second += 1;

   
    int newFreq = this->_data[key].second;

   

   
    auto itToPairOfFreqAndKeys = this->_freqToKeys.find(oldFreq);

   
    if(itToPairOfFreqAndKeys != this->_freqToKeys.end()){
        
        auto itToKey = itToPairOfFreqAndKeys->second.find(key);

       
        if(itToKey != itToPairOfFreqAndKeys->second.end()){

          
            itToPairOfFreqAndKeys->second.erase(itToKey);

           
            if(itToPairOfFreqAndKeys->second.empty()){

               
                this->_freqToKeys.erase(oldFreq);

               
                this->_minFrequency.erase(oldFreq);
            }
        }
        
        else{
           
        }
    }

   
    this->_freqToKeys[newFreq].insert(key);

    
    this->_minFrequency.insert(newFreq);

    return std::make_pair(true, this->_data.at(key).first);
}


void Cache::removeLeastFrequencyElements(){
   
    auto itToMinFreq = this->_minFrequency.begin();

    assert(itToMinFreq != this->_minFrequency.end());

    
    int minFreq = *itToMinFreq;

   
    auto keys = this->_freqToKeys[minFreq];

   
    for(const auto&k : keys){
        this->_data.erase(k);
    }

    
    this->_minFrequency.erase(itToMinFreq);

   
    this->_freqToKeys.erase(minFreq);
}


void Cache::Set(std::string key, std::string value){
   
    if(this->_data.find(key) != this->_data.end()){
        return;
    }

    
    if(this->_data.size() == this->_maxNumEntries){
        
        this->removeLeastFrequencyElements();
    }

    this->_freqToKeys[1].insert(key);

 
    this->_minFrequency.insert(1);

    this->_data[key].first = value;

   
    this->_data[key].second = 1;
}


std::unordered_map<std::string, std::string> Cache::getDataWithoutFreq() const{
  
    std::unordered_map<std::string, std::string> out;
    for(const auto& keyValue : this->_data){
        out[keyValue.first] = keyValue.second.first;
    }
    return out;
}



bool Cache::toDB() const{
    return FileSystem::sendContentsToDB(this->_data);
}


void Cache::print(){
    for(auto&[k,v] : this->_data){
        std::cout<< "Key: "<< k << " " << "Value: " << v.first << " " << "Frequency: "<< v.second << std::endl;
    }
}