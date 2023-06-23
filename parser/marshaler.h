#pragma once
#include <string>
namespace ctools {
template <typename T>
void unmarshal(T*,const char*);//parse from buffer to struct
template <typename T>
std::string marshal(T*);
}