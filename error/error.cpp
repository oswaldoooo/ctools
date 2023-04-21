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
    content+="\n"+_content;
}

bool error::isNull()
{
    return content.length()>0;
}

void error::clear()
{
    content="";
}
