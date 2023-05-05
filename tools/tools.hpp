#include "tools.h"
namespace ctools {
    namespace array {
        enum class forwards{left,right};
        void arraymove(void *src,unsigned int step,forwards fr,size_t length);

        inline void arraymove(void *src, unsigned int step,forwards fr,size_t length){
        	int startpos;
        	int endpos;
        	switch (fr) {
        	case forwards::left:
        		startpos=0;
        		endpos=length-step;
        		break;
        	case forwards::right:

        		break;
        	default:

        		break;
        	}
        	for(int i=startpos;i<endpos;i++){
        		src[i]=src[i+step];
        	}
        }
    }
}
