#pragma onece;
#ifndef tools_h
#define tools_h
#include <map>
#include <vector>
namespace ctools
{
    struct none;
    template<class T>
    std::map<T,none> *arrayTomap(T *src,int langth);
    void printVector(std::vector<char*> *src);
    std::vector<char*> *fitter_str(char* src,char* sym);
    void deleteStr(std::vector<char*> *origin);
} // namespace ctools
#endif