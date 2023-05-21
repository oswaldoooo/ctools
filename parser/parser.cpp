#include "parser.h"
#include "error/error.h"
#include "tools/tools.h"
#include <cstdio>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
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

error ctools::writeToFile(const char *filename,const char *content,actmode act){
    error err;
    ios::ios_base::openmode md;
    switch (act) {
    case ctools::actmode::append:
        md=ios::app;
        break;
    case ctools::actmode::trunc:
        md=ios::trunc;
        break;
    default:
        err.join("not the action mode");
        return err;
    }
    std::ofstream ofs;
    ofs.open(filename,md|ios::out);
    ofs<<content;
    ofs.close();
    return err;
}
// Ans<char*> ctools::readFrom(const char *filename){
//     Ans<char*> res;
//     ifstream ifs;
//     ifs.open(filename,ios::in);
//     ifs.seekg(0,ifs.end);
//     int filesize=ifs.tellg();
//     ifs.seekg(0,ifs.beg);
//     res.object=new char[filesize+1];
//     res.needfree=true;//tell ans this need free
//     ifs.read(res.object, filesize+1);
//     ifs.close();
//     return res;
// }
unsigned long ctools::filesize(const char *filename){
    ifstream ifs;
    ifs.open(filename,ios::in);
    ifs.seekg(0,ifs.end);
    unsigned long size=ifs.tellg();
    ifs.seekg(0,ifs.beg);
    ifs.close();
    return size;
}
char *ctools::formatsize(unsigned long origin){
#ifndef out_template
#define out_template "%lu %s"
#endif
    enum class sizeform{B,KB,MB,GB,TB,PB};
    int start=0;
    while (origin>=1024) {
        origin=origin/1024;
        start++;
    }
    string newres=to_string(origin);
    char *newcres=new char[newres.size()+3];
    int res;
    switch (start) {
    case (int)sizeform::B:
        res=snprintf(newcres,newres.length()+3,out_template , origin,"");
        break;
    case (int)sizeform::KB:
        res=snprintf(newcres,newres.length()+3,out_template , origin,"KB");
        break;
    case (int)sizeform::MB:
        res=snprintf(newcres,newres.length()+3,out_template , origin,"MB");
        break;
    case (int)sizeform::GB:
        res=snprintf(newcres,newres.length()+3,out_template , origin,"GB");
        break;
    case (int)sizeform::TB:
        res=snprintf(newcres,newres.length()+3,out_template , origin,"TB");
        break;
    case (int)sizeform::PB:
        res=snprintf(newcres,newres.length()+3,out_template , origin,"PB");
        break;
    default:
        res=snprintf(newcres,newres.length()+3,out_template , origin,"PB");
        break;
    }
    return newcres;
}
bool file_exist(char *filename){
    ifstream ifs(filename);
    return ifs.good();
}
bool insertInto(char *filename, char *content,unsigned int line_num){
    ifstream ifs;
    ifs.open(filename,ios::in);
    if(!ifs.good()) return false;
    ifs.seekg(0,ifs.end);
    int length=ifs.tellg();
    ifs.seekg(0,ifs.beg);
    char subcontent[length+1];
    char body[length+1];
    unsigned int line;
    while(ifs.getline(subcontent,length+1)){
        line++;
        subcontent[strlen(subcontent)]='\n';
        strncat(body,subcontent,strlen(subcontent));
        if(line==line_num) strncat(body, content, strlen(content));
        memset(subcontent, 0, strlen(subcontent));
    }
    ifs.close();
    ofstream ofs;
    ofs.open(filename,ios::out|ios::trunc);
    ofs<<body;
    ofs.close();
    return true;
}
bool replaceInFile(char *filename, char *content, unsigned int line_num){
    ifstream ifs;
    ifs.open(filename,ios::in);
    if(!ifs.good()) return false;
    ifs.seekg(0,ifs.end);
    int length=ifs.tellg();
    ifs.seekg(0,ifs.beg);
    char subcontent[length+1];
    char body[length+1];
    unsigned int line;
    while(ifs.getline(subcontent,length+1)){
        line++;
        if(line==line_num){
            strncat(body, content, strlen(content));
        }else{
            subcontent[strlen(subcontent)]='\n';
            strncat(body,subcontent,strlen(subcontent));    
        }
        memset(subcontent, 0, strlen(subcontent));
    }
    ifs.close();
    ofstream ofs;
    ofs.open(filename,ios::out|ios::trunc);
    ofs<<body;
    ofs.close();
    return true;
}
std::map<std::string, std::string> ctools::parselist(const char *filepath){
    struct stat fst;char *mid;
    std::map<string, string> ans;size_t dis;
    char *ti;
    if(stat(filepath, &fst)!=-1){
        char content[fst.st_size];
        char left[fst.st_size];
        char right[fst.st_size];
        ifstream ifs;
        ifs.open(filepath,ios::in);
        ti=new char[fst.st_size];
        while (ifs.getline(content,fst.st_size)) {
            mid=strstr(content,PARSE_MID);
            dis=strlen(content)-strlen(mid);
            strncpy(ti, content, dis);
            if(ans.count(ti)==0) ans.insert({ti,mid+strlen(PARSE_MID)});
            memset(ti,0,strlen(ti));
        }
        delete [] ti;

    }else printf("get %s status information failed\n", filepath);
    return ans;
}
void ctools::freelist(std::map<char *, char *> map){
    if(map.size()==0) return;
    for(auto [key,value]:map){
        delete[] key;delete[] value;
    }
}