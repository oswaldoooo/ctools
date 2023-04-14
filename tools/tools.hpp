//#include "arraylist.h"
#include <map>
#include <vector>
#include <iostream>
namespace ctools
{
    struct none
    {};
    template<class T>
    std::map<T,none> *arrayTomap(T *src,int langth){
        std::map<T,none> *resmap=new std::map<T,none>;
        for (int i = 0; i < langth; i++)
        {
            if (resmap->count(src[i])==0)
            {
                none newnone;
                resmap->insert({src[i],newnone});
            }else{
                throw "array repeat";
            }
        }
        return resmap;
        
    }
    void printVector(std::vector<char*> *src){
        char *target;
        for (int i = 0; i < src->size(); i++)
        {
            target=src->at(i);
            std::cout<<target<<"\t";
        }
        std::cout<<std::endl;
        
    }
    std::vector<char*> *fitter_str(char* src,char* sym){
        std::cout<<"the langth is "<<strlen(sym)<<std::endl;
        std::map<char,none>* resmap=arrayTomap<char>(sym,strlen(sym));
        std::vector<char*> *resvec=new std::vector<char*>;
        bool issave=false;int start=0;
        char *buffer=new char[strlen(sym)+1];
        for (int i = 0; i < strlen(src); i++)
        {
            if (resmap->count(src[i])>0)
            {
                //触发关键字，移除
                if (issave)
                {
                    buffer[start]='\0';
                    resvec->push_back(buffer);
                    buffer=new char[strlen(src)+1];
                    issave=false;
                }
                
            }else{
                if(!issave){
                    start=0;
                    // char buff[strlen(src)+1];
                    // buffer=new char[strlen(src)];
                    buffer[start]=src[i];
                    issave=true;
                    start++;
                }else{
                    buffer[start]=src[i];
                    start++;
                }
            }
            
        }
        if (strlen(buffer)>0)
        {
            resvec->push_back(buffer);
        }
        
        delete resmap;
        return resvec;
    }
    void deleteStr(std::vector<char*> *origin){
        for (int i = 0; i < origin->size(); i++)
        {
            delete[] origin->at(i);
        }
        delete origin;
        
    }
} // namespace ctools
