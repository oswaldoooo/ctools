
#include "parser/marshaler.h"
#include "show.h"
#include "json/reader.h"
#include "json/value.h"
#include "json/writer.h"
#include <cstring>
#include <stdexcept>
#include <string>
template<>
std::string ctools::marshal(struct Name *name){
	Json::Value jv;
	Json::FastWriter writer;
	jv["first_name"]=name->first_name;
	jv["last_name"]=name->last_name;
	return writer.write(jv);
}
void cstr_tostr(char*,const char*);
inline void cstr_tostr(char *dst, const char *origin){
	dst=new char[std::strlen(origin)+1];
	std::strncpy(dst,origin,strlen(origin));
}
template <>
void ctools::unmarshal(struct Name *dst, const char *data){
	Json::Value jv;
	Json::Reader rd;
	if(rd.parse(data,jv)){
		std::string fn=jv["first_name"].asString();
		std::string ln=jv["last_name"].asString();
		dst->first_name=new char[fn.length()+1];
		dst->last_name=new char[ln.length()+1];
		strncpy(dst->first_name, fn.c_str(), fn.length());
		strncpy(dst->last_name, ln.c_str(), ln.length());
	}else{
		throw std::logic_error("parse failed");
	}
}
template<>
std::string ctools::marshal(struct Ans *data){
	Json::Value jv;
	Json::FastWriter writer;
	jv["ans"]=data->ans;
	jv["error"]=data->err;
	return writer.write(jv);

}
template <>
void ctools::unmarshal(struct Ans *dst, const char *data){
	Json::Value jv;
	Json::Reader rd;
	if(rd.parse(data,jv)){
		dst->ans=jv["ans"].asString();
		dst->err=jv["error"].asString();
	}else{
		throw std::logic_error("parse failed");
	}
}
