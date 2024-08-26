
#ifndef REDISGRPC_CACHE_H
#define REDISGRPC_CACHE_H

#include <iostream>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <memory>
#include <utility>




namespace redisgrpc{
    
    namespace lib {
        using SetOfInt_T = std::set<int>;
        using Cache_T = std::unordered_map<std::string, std::pair<std::string,int>>;
        using MapOfIntToSetOfStr_T = std::unordered_map<int, std::unordered_set<std::string>>;
        class Cache{

        private:
            size_t _maxNumEntries; 

            size_t _numElementsToBeRemoved = 0.1*_maxNumEntries;

           
            SetOfInt_T _minFrequency;

            
            Cache_T _data;

         
            MapOfIntToSetOfStr_T _freqToKeys;

           
            void removeLeastFrequencyElements();

        public:

      
            Cache(size_t maxNumEntries = 100000): _maxNumEntries(maxNumEntries){
            };

           
            inline Cache_T getData(){return _data;}

            std::unordered_map<std::string, std::string> getDataWithoutFreq() const;

           
            const SetOfInt_T& getMinFrequencySet() const{
                return _minFrequency;
            };

           
            const MapOfIntToSetOfStr_T& getFreqToKeysMap() const{
                return _freqToKeys;
            };

           
            std::pair<bool,std::string> Get(std::string key);

          
            void Set(std::string key, std::string val);

           
            bool toDB() const;

            
            void print();

        }; 
    }; 
}; 

#endif 