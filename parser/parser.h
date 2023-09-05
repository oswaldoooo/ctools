#ifndef parser_h
#define parser_h
#include <ctools/stdlib/error.h>
#include <ctools/tools/tools.h>
#include <map>
#include <string>
#include <vector>
#ifndef PARSE_MID
#define PARSE_MID "="
#endif
namespace ctools {
enum class _mode { release,
    debug };
static _mode mode = _mode::release;
class parser {
private:
    char* start;
    char* end;
    bool isinit = false;
    bool matchsymbol(const char* dst, const char* src);

public:
    parser();
    parser(char* start_, char* end_);
    void setparsesymbol(char* start_, char* end_);
    std::vector<std::string> parsecontent(std::string src);
    std::vector<std::string> parsecontent(char* src);
};
/* pass the delims,return cstring,need handle free the pointer */
char* parsercontent(const char* src, const char* delims);
// pass the delims,return vector
std::vector<std::string> parsercontentplus(const char* src, const char* delims);
// pass the delims,return map
std::map<std::string, int> parecontentplusplus(const char* src, const char* delims);
enum class actmode { append,
    trunc };
error writeToFile(const char* filename, const char* content, actmode act = actmode::append);
// Ans<char*> readFrom(const char *filename);
unsigned long filesize(const char* filename);
// need handle free the result
char* formatsize(unsigned long origin);
bool file_exist(char* filename);
bool insertInto(char* filename, char* content, unsigned int line_num);
bool replaceInFile(char* filename, char* content, unsigned int line_num);
std::map<std::string, std::string> parselist(const char*);
void formatlist(const char*, const std::map<std::string, std::string>&);
void freelist(std::map<char*, char*> map);
}

#endif