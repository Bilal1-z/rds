#ifndef REDISGRPC_LOGGER_H
#define REDISGRPC_LOGGER_H

#include <iostream>

namespace redisgrpc{
    namespace lib{
        class Logger{
        public:
            Logger(){
                _init();
            };
            inline void log(std::string &&input){
                this->_print();
                std::cout << input << std::endl;
                this->_print();
            }
            inline void error(std::string &&input){
                this->_print();
                std::cerr << input << std::endl;
                this->_print();
            }
            const int getNumDashes() const {
                return this->_numDashes;
            }
            std::string getProjectName() const{
                return this->_projectName;
            }
        private:
            const std::string _projectName = "redisgrpc";
            void _print(){
                for(size_t idx = 0; idx < _numDashes; ++idx) {
                    std::cout << symbol;
                }
                std::cout << std::endl;
            }
            const int _numDashes = 20;
            const char symbol = '-';
            inline void _init(){
                std::cout << _projectName << std::endl;
                for(size_t idx = 0; idx < _numDashes; ++idx) {
                    std::cout << symbol;
                }
                std::cout << std::endl;
            }
        };
    } 
}; 
#endif 