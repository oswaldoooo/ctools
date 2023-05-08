#include "cinsecure.h"
#include <stdio.h>
#include <stdlib.h>
void getInput(char *buffer){
	size_t i=0;
	system("stty -icanon");
	system("stty -echo");
   	while(1){
   		buffer[i]=getchar();
		if(i == 0 && buffer[i] == BACKSPACE)
		{
			i=0;                              //若开始没有值，输入删除，则，不算值
			buffer[i]='\0';
			continue;
		}
		else if(buffer[i] == BACKSPACE)
		{
			printf("\b \b");                    //若删除，则光标前移，输空格覆盖，再光标前移
			buffer[i]='\0';
			i=i-1;                              //返回到前一个值继续输入
			continue;                           //结束当前循环
		}
		else if(buffer[i] == '\n')               //若按回车则，输入结束
		{
			buffer[i]='\0';
			break;
		}
		else
		{
			// printf("*");
		}
		i++;
   	}
   	system("stty icanon");
   	system("stty echo");
}