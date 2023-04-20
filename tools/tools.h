#pragma onece;
#ifndef tools_h
#define tools_h
#include <map>
#include <vector>
#include <string>
namespace ctools
{
    struct none{};
    template<class T>
    std::map<T,none> *arrayTomap(const T *src,int langth);
    void printVector(std::vector<char*> *src);
    std::vector<char*> *fitter_str(char* src,char* sym);
    void deleteStr(std::vector<char*> *origin);
    char **getfilename(char *filename);
    void wordcount(const char *src,const std::map<char,none> &delim,std::map<std::string,int> &output);
} // namespace ctools
#endif