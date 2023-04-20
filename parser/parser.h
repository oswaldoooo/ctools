#ifndef parser_h
#define parser_h
#include <string>
#include <vector>
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
}
#endif