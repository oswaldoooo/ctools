#ifndef error_h
#define error_h
#include <string>
class error{
private:
    std::string content;
public:
    error();
    error(std::string _content);
    //join the error content to error
    void join(std::string _content);
    //justify the error is null
    bool isNull();
    //clear the error content
    void clear();
};
#endif