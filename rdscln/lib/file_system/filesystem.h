
#define REDISGRPC_FILESYSTEM_H


#include <string>
#include <unordered_map>
#include <utility>
#include <iostream>
#include <fstream>
#include <filesystem> 

namespace fs = std::filesystem;

namespace redisgrpc {
    namespace lib{
        using Cache_T = std::unordered_map<std::string, std::pair<std::string,int>>;
        class FileSystem{
        private:
            std::string _fileName;
            std::string _directoryName;
        private:
            FileSystem() = delete;
        public:
            inline static bool sendContentsToDB( const Cache_T& cache, const std::string& fileName="redisgrpc_cache", const std::string& directoryName = "db"){
               
                fs::path directory = directoryName;
                if (!fs::exists(directory)) {
                    fs::create_directory(directory);
                    std::cout << "Directory " << directoryName << " created." << std::endl;
                } else {
                    std::cout << "Directory " << directoryName << " already exists." << std::endl;
                }

                
                fs::path filePath = directory / (fileName+".rg");
                std::ofstream outFile(filePath);
                if (!outFile.is_open()) {
                    std::cerr << "Error opening file for writing." << std::endl;
                    return false;
                }

                for(const auto&[k,v] : cache){
                    
                    outFile << k << " " << v.first << std::endl;
                }

                
                outFile.close();

                std::cout << "File " << fileName << "saved inside directory " << directoryName << std::endl;

                return true;
            }
                 }; 
    } 
}


 
