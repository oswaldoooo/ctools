#ifndef log_h
#define log_h
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <iostream>
#define DEFAULT_SIZE 10<<10
void to_log(const char *filename,const char *content);
// void to_log(const char *filename,const char *templates ,...);
//need hanle free the char *
template <typename T,typename E>
void getmap(const std::map<T, E> &newmap,char *dst);
template <typename T,typename E>
void getmap(const std::map<T, E> &newmap,char *dst){
	std::string res="";
	for(const auto &[key,value]:newmap){
		res+=std::to_string(key)+"\t"+std::to_string(value)+"\n";
	}
	const char *ans=res.c_str();
	strncpy(dst, ans, strlen(ans)+1);
}
#endif