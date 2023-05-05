#include "tools.h"
#include "error/error.hpp"
#include <cstring>
#include <stdexcept>
namespace ctools {
    namespace array {
        enum class forwards{left,right};
        template<typename T>
		void arraymove(T *src,unsigned int step,forwards fr,size_t length);
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
				memset(src+step, 0, sizeof(T)*step);
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
    }
}
