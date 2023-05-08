#include "cinsecure.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <thread>

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
void getLine(char *buffer, bool &is_input, bool &force_end){
	size_t i=0;
	system("stty -icanon");
	system("stty -echo");
   	while(!force_end){
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
   	is_input=true;
   	system("stty icanon");
   	system("stty echo");
   	printf("end input,%d\n",is_input);
}

bool getLineUltra(char *buffer,unsigned sec){
	clock_t now=clock();
	// bool isinput=false;
	bool isinput=false;
	bool forceend=false;
	std::thread newthread(getLine,buffer,std::ref(isinput),std::ref(forceend));
	if(newthread.joinable()) newthread.detach();//wait input
	while(clock()-now<sec*CLOCKS_PER_SEC&&!isinput);
	forceend=true;
	return isinput;
}

// test code here

// int main(int argc, char const *argv[])
// {
// 	printf("input your words:\n");
// 	char *newbuffer=new char[100];
// 	if(getLineUltra(newbuffer, 4)){
// 		printf("your input is %s\n", newbuffer);
// 	}else{
// 		printf("time out\n");
// 	}
// 	delete [] newbuffer;
// 	return 0;
// }