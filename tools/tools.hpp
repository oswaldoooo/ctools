#pragma once
#include "tools.h"
#include "error/error.hpp"
#include <cassert>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <type_traits>
namespace ctools {
    namespace array {
        enum class forwards{left,right};
        template<typename T>
		void arraymove(void *src, unsigned int step, forwards fr, size_t length);
		void *cmemset(void *src,int value,unsigned int length);
		template<typename T>
        inline void arraymove(T *src, unsigned int step,forwards fr,size_t length){
        	int startpos;
        	int endpos;
        	switch (fr) {
        	case forwards::left:
        		startpos=0;
        		endpos=length-step;
				for(int i=startpos;i<endpos;i++){
					src[i]=src[i+step];//left move
				}
				memset(src+length-step, 0, sizeof(T)*step);
        		break;
        	case forwards::right:
				startpos=length-1;
				endpos=step;
				for(int i=startpos;i>=endpos;i--){
					src[i]=src[i-step];
				}
				memset(src, 0, sizeof(T)*step);
        		break;
        	default:
				throw NoElement("not this mode");
        		break;
        	}
        }
        inline void *cmemset(void *src,int value,unsigned int length){
        	// assert(src);
        	char *newsrc=(char *)src;
        	// assert(newsrc);
        	while(length-->0){
        		*newsrc++=value;
        	}
        	return src;
        }
    }
}

inline void *mymemcpy(void *dest, const void *src, size_t count)
{
    if (dest == nullptr || src == nullptr)
    {
        return nullptr;
    }
    auto tempDistance = static_cast<char*>(dest) - static_cast<const char*>(src);
 
    if ( tempDistance > 0 && static_cast<size_t>(tempDistance)  < count )
    {
        char *tempDest = static_cast<char*>(dest) + count - 1;
        const char* tempSrc = static_cast<const char*>(src) + count - 1;
        while (count-- > 0)
        {
            *tempDest = *tempSrc;
            tempDest--;
            tempSrc--;
        }
    }
    else
    {
        char *tempDest = static_cast<char*>(dest);
        const char* tempSrc = static_cast<const char*>(src);
        while (count-- > 0)
        {
            *tempDest = *tempSrc;
            tempDest++;
            tempSrc++;
        }
    }
    return dest;
}

inline void *my_memset(void *dest, int set, unsigned len)
{
	if (dest == NULL || len < 0)
	{
		return NULL;
	}
	char *pdest = (char *)dest;
	while (len-->0)
	{
		*pdest++ = set;
	}
	return dest;
}
 inline void* my_memmove(void*dest, const void* src, size_t n)
{
	char*_dest = (char*)dest;
	const char*_src = (char*)src;
	if (_dest > _src&&_dest < _src + n)//发生区域重叠
	{
		while (n--)
		{
			*_dest--= *(_src+n);//逆向复制
		}
	}
	else//未发生区域重叠 正向复制
	{
		while (n--)
		{
			*_dest++ = *_src++;
		}
	}
	return dest;
}

