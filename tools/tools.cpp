//#include "arraylist.h"
#include <map>
#include <vector>
#include <iostream>
#include <cstring>
#include "tools.h"
namespace ctools
{
    template<class T>
    std::map<T,none> *arrayTomap(T *src,int langth){
        std::map<T,none> *resmap=new std::map<T,none>;
        none newnone;
        for (int i = 0; i < langth; i++)
        {
            if (resmap->count(src[i])==0)
            {
                
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
    char **getfilename(char *filename)
    {
        bool isrecord=false;
        char prix[8];
        char realname[strlen(filename)];
        std::fill(prix,prix+8,'\0');
        int start=0;
        for(int i=0;i<strlen(filename);i++){
            if(filename[i]=='.'){
                if(!isrecord){
                    isrecord=true;
                    start=0;
                }else{
                    throw "filename format dont support";
                }
            }else{
                if(isrecord){
                    //record prix
                    prix[start]=filename[i];
                    start++;
                }else{
                    //record filename
                    realname[start]=filename[i];
                    start++;
                }
            }
        }
        char *namearr[2];
        namearr[0]=realname;
        namearr[1]=prix;
        return namearr;
    }
} // namespace ctools
