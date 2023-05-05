#include <algorithm>
#include <memory>
#include <stdexcept>
#pragma onece;
#ifndef tools_h
#define tools_h
#include <map>
#include <vector>
#include <string>
#include "error/error.h"
namespace ctools
{
    struct none{};
    enum class ans_mod{normal,array};
    template<typename T>
    struct Ans
    {
        T object;
        error err;
        ~Ans(){
            // if need free,delete the object
            // if(needfree){
            //     delete[] object;
            // }
        }
        // Ans(Ans&& origin){object=std::move(origin.object);}
        bool needfree=false;
    };
    template <typename T>
    struct Ansunique{
        std::unique_ptr<T> object;
        std::unique_ptr<T[]> objectarr;
        bool isobject;
        bool isobjectarr;
        ans_mod md=ans_mod::normal;
        error err;
        Ansunique(ans_mod newmod=ans_mod::normal){
            md=newmod;
            switch (md) {
            case ans_mod::normal:
                object=std::unique_ptr<T>(new T);
                break;
            case ans_mod::array:
                objectarr=std::unique_ptr<T[]>(new T[10]);
                break;
            default:
                throw std::out_of_range("not support mode");
                break;
            }
        }
        Ansunique(Ansunique&& origin){
            switch (md) {
            case ans_mod::normal:
                object=std::move(origin.object);
                break;
            case ans_mod::array:
                objectarr=std::move(origin.objectarr);
                break;
            default:
                throw std::out_of_range("not support mode");
                break;
            }
        }
        void setnormal(std::unique_ptr<T>& origin){
            md=ans_mod::normal;
            object=std::move(origin);
        }
        void setarray(std::unique_ptr<T[]>&origin){
            md=ans_mod::array;
            objectarr=std::move(origin);
        }

    };
    template <typename T>
    struct AnsVec{
        std::vector<T> object;
        error err;
    };
    template <typename T,typename E>
    struct AnsMap{
        std::map<T, E> object;
        error err;
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
    //sleep n seconds
    void sleep(unsigned int n);
} // namespace ctools
#endif
