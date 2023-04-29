#pragma onece;
#ifndef tools_h
#define tools_h
#include <map>
#include <vector>
#include <string>
#include "../error/error.h"
namespace ctools
{
    struct none{};
    template<class T>
    struct Ans
    {
        T object;
        error err;
        ~Ans(){
            // if need free,delete the object
            if(needfree) delete [] object;
        }
        bool needfree=false;
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
    template<class T>
    class pointerplus{
    private:
        T* core;
        bool needfree;
        bool isarr=false;
    public:
        pointerplus()=delete;
        pointerplus(T* newcore,bool needfree_=false);
        pointerplus(T* newcore,bool isarre,bool needfree_=false);
        ~pointerplus();
        T* load();
    };
    template<class T>
    pointerplus<T>::pointerplus(T* newcore,bool needfree_){
        needfree=needfree_;
        core=newcore;
    }
    template<class T>
    pointerplus<T>::pointerplus(T* newcore,bool isarre,bool needfree_){
        isarr=isarre;
        needfree=needfree_;
        core=newcore;
    }
    template<class T>
    pointerplus<T>::~pointerplus(){
        if(needfree){
            if(isarr){
                delete[] core;
            }else{
                delete core;
            }
        }
    }
} // namespace ctools
#endif
