#ifndef parser_h
#define parser_h
#include <string>
#include <vector>
#include <map>
namespace ctools{
    class parser{
    private:
        char *start;
        char *end;
        bool isinit=false;
        bool matchsymbol(const char *dst,const char *src);
    public:
        parser();
        parser(char *start_,char *end_);
        void setparsesymbol(char *start_,char *end_);
        std::vector<std::string> parsecontent(std::string src);
        std::vector<std::string> parsecontent(char *src);
    };
    /* pass the delims,return cstring,need handle free the pointer */
    char* parsercontent(const char *src,const char *delims);
    //pass the delims,return vector
    std::vector<std::string> parsercontentplus(const char *src,const char *delims);
    //pass the delims,return map
    std::map<std::string,int> parecontentplusplus(const char *src,const char *delims);
}

#endif