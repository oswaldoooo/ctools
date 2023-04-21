#include "parser.h"
#include "repository/tools/tools.h"
#include <vector>
#include <string>
#include <cstring>
using namespace std;
using namespace ctools;
bool parser::matchsymbol(const char *dst, const char *src)
{
    if(strlen(dst)==strlen(src)){
        for(int i=0;i<strlen(src);i++){
            if(src[i]!=dst[i]){//one symbol dont match ,return false
                return false;
            }
        }
        return true;
    }
    return false;
}
parser::parser(char *start_, char *end_)
{
    start=start_;
    end=end_;
    isinit=true;
}

void parser::setparsesymbol(char *start_, char *end_)
{
    start=start_;
    end=end_;
    isinit=true;   
}

vector<string> parser::parsecontent(string src)
{
    if(!isinit){
        throw "parser dont init,you should init first";
    }
    vector<string> res;
    bool matchstart=false;
    bool ok;
    string recstr="";
    if(src.length()>strlen(start)+strlen(end)){
        //source string must over the start and end symbol
        for(int i=0;i<src.length();i++){
            if(!matchstart&&src[i]==start[0]){//macth start,begin recording
                ok=matchsymbol(src.substr(i,strlen(start)).c_str(),start);
                if(ok){
                    matchstart=true;
                    i+=strlen(start);
                }
            }
            if(matchstart){
                if(src[i]==end[0]){//macth end,end recording
                    ok=matchsymbol(src.substr(i,strlen(end)).c_str(),end);
                    if(ok){
                        matchstart=false;
                        if(recstr.length()>0){
                            res.push_back(recstr);//put in result vector,and reset recstr
                            recstr="";
                        }
                    }else{
                        recstr+=src[i];
                    }
                }else{
                    recstr+=src[i];
                }
            }
        }
    }
    if (matchstart){
        throw "your end symbol lost";
    }
    return res;
}

vector<string> parser::parsecontent(char *src)
{
    if(!isinit){
        throw "parser dont init,you should init first";
    }
    vector<string> res;
    bool matchstart=false;
    bool ok;string recstr="";
    char target[strlen(src)];
    if(strlen(src)>strlen(start)+strlen(end)){
        for(int i=0;i<strlen(src);i++){
            if(!matchstart&&src[i]==start[0]){//macth start,begin recording
                strncpy(target,src+i,strlen(start));
                ok=matchsymbol(target,start);
                if(ok){
                    matchstart=true;
                    i+=strlen(start);
                }
            }
            if(matchstart){
                if(src[i]==end[0]){//macth end,end recording
                    strncpy(target,src+i,strlen(start));
                    ok=matchsymbol(target,end);
                    if(ok){
                        matchstart=false;
                        if(strlen(src)>0){
                            res.push_back(recstr);//put in result vector,and reset recstr
                            recstr="";
                        }
                    }else{
                        recstr+=src[i];
                    }
                }else{
                    recstr+=src[i];
                }
            }
        }
    }
    return res;
}

char *ctools::parsercontent(const char *src, const char *delims)
{
    char *results=new char[strlen(src)];
    int start=0;
    map<char,none> *commap=arrayTomap<char>(delims,strlen(delims));
    for(int i=0;i<strlen(src);i++){
        if(commap->count(src[i])==0){
            results[start]=src[i];
            start++;
        }
    }
    delete commap;
    return results;
}

std::vector<std::string> ctools::parsercontentplus(const char *src, const char *delims)
{
    vector<string> res;bool isrecord=false;
    string buffer="";
    map<char,none> *commap=arrayTomap<char>(delims,strlen(delims));
    for(int i=0;i<strlen(src);i++){
        if(commap->count(src[i])!=0){
            if(isrecord){
                isrecord=false;
                if(buffer.length()>0){
                    res.push_back(buffer);
                    buffer="";
                }
            }
        }else{
            if(!isrecord){
                isrecord=true;
            }
            buffer+=src[i];
        }
    }
    if(isrecord){
        res.push_back(buffer);
    }
    delete commap;
    return res;
}

std::map<std::string, int> ctools::parecontentplusplus(const char *src, const char *delims)
{
    map<string,int> res;bool isrecord=false;
    string buffer="";
    map<char,none> *commap=arrayTomap<char>(delims,strlen(delims));
    for(int i=0;i<strlen(src);i++){
        if(commap->count(src[i])!=0){
            if(isrecord){
                isrecord=false;
                if(buffer.length()>0){
                    if(res.count(buffer)!=0){
                        res.insert({buffer,1});
                    }else{
                        res.at(buffer)++;
                    }
                    buffer="";
                }
            }
        }else{
            if(!isrecord){
                isrecord=true;
            }
            buffer+=src[i];
        }
    }
    if(isrecord){
        if(res.count(buffer)!=0){
            res.insert({buffer,1});
        }else{
            res.at(buffer)++;
        }
    }
    delete commap;
    return res;
}
