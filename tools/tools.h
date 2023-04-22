#pragma onece;
#ifndef tools_h
#define tools_h
#include <map>
#include <vector>
#include <string>
#include "/home/dev/cpp/lib/error/error.h"
namespace ctools
{
    struct none{};
    template<class T>
    struct RETURN
    {
        T object;
        error errs;
    };
    template<class T>
    std::map<T,none> *arrayTomap(const T *src,int langth);
    void printVector(std::vector<char*> *src);
    std::vector<char*> *fitter_str(char* src,char* sym);
    void deleteStr(std::vector<char*> *origin);
    char **getfilename(char *filename);
    void wordcount(const char *src,const std::map<char,none> &delim,std::map<std::string,int> &output);
    //spilt the string
    std::vector<std::string> spilt(const char *origin,const char *delim);
} // namespace ctools
#endif
