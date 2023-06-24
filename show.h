#pragma once
#include "json/json.h"
#include <string>
struct Name{
	char* first_name;
	char* last_name;
	std::string String(){
		std::string ans=first_name;
		ans+=".";
		return ans+last_name;
	}
};
struct Ans{
	std::string ans;
	std::string err;
};