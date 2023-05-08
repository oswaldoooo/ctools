#pragma once
#include "error.h"
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