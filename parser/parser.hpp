#pragma once
#include <map>
#include <sstream>
#include <string>
#include <vector>
// split the string to vector and map
std::vector<std::string> str_split_(std::string& origin, const char delim = '\n');
std::map<std::string, std::string> str_split(std::string& origin, const char delim = '\n', const char linedelim = '=', bool allowreplace = false);
std::vector<std::string> str_split_(std::string& origin, const char delim)
{
    std::vector<std::string> ans;
    std::istringstream ist(origin);
    std::string token;
    while (std::getline(ist, token, delim)) {
        ans.push_back(token);
    }
    return ans;
}

std::map<std::string, std::string> str_split(std::string& origin, const char delim, const char linedelim, bool allowreplace)
{
    std::map<std::string, std::string> ans;
    std::istringstream ist(origin);
    std::string token;
    char linedelim_str[2] = { linedelim };
    size_t off_pos;
    while (std::getline(ist, token, delim)) {
        const char* token_str = token.c_str();
        const char* end_str = strstr(token_str, linedelim_str);
        off_pos = end_str - token_str;
        if (ans.count(token.substr(0, off_pos)) > 0) {
            if (allowreplace) {
                ans.at(token.substr(0, off_pos)) = token.substr(off_pos + 1);
            }
        } else {
            ans.insert({ token.substr(0, off_pos), token.substr(off_pos + 1) });
        }
    }
    return ans;
}