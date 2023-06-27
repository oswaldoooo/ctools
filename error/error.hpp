#pragma once
#include <exception>
#include <string>
class NoElement:public std::exception{
private:
    std::string core;
public:
    NoElement()=delete;
    NoElement(std::string &input):core(input){}
    NoElement(const char *input):core(input){}
    const char* what() const noexcept{return core.c_str();}
};
namespace ctools {
    class error{
    public:
        virtual bool isempty()=0;
        virtual std::string what()=0;
    };
    class logic_error:public error{
    private:
        std::string core;
    public:
        void join(error err){
            core+="\n"+err.what();
        }
        bool isempty() override {
            return core.size()==0;
        }
        std::string what() override{
            return core;
        }
    };

}