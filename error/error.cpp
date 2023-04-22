#include <string>
#include "error.h"

error::error()
{
}

error::error(std::string _content)
{
    content=_content;
}

void error::join(std::string _content)
{
    content+=_content+"\n";
}

void error::join(const error &_err)
{
    content+=_err.content;
}

bool error::isNull()
{
    return content.length()>0;
}

void error::clear()
{
    content.clear();
}

bool error::operator==(const error &target)
{
    if(content.compare(target.content)==0){
        return true;
    }else{
        return false;
    }
}

const char *error::String()
{
    return content.c_str();
}
