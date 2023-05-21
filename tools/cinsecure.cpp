#include "cinsecure.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <ios>
#include <iostream>
#include <ostream>
#include <thread>
#include <csignal>
#include <fstream>
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

static size_t pos=0;
static char content[100];
static char filename[30];
void ListenKey(void (*funcp)(int),unsigned sec){
	clock_t now=clock();
	system("stty -icanon");
	system("stty -echo");
	char ch;
	while (sec<=0||clock()-now<sec*CLOCKS_PER_SEC) {
		ch=getchar();
		funcp(ch);
		if(content[pos-4]==27&&content[pos-3]==91){//delete forward key
			content[pos-4]=0;
			content[pos-3]=0;
			content[pos-2]=0;
			content[pos-1]=0;
		}
	}
	system("stty icanon");
	system("stty echo");
}

// test code here
void ifis(int origin){
	if(origin!=127){
		content[pos++]=origin;
		printf("%c",origin);
	}else{
		if (pos>0)content[--pos]=0;
		if(content[pos-1]=='\n'){
			printf("\r");
		}else{
			printf("\b \b");
		}
	}
	
}
void save(int){
	std::ofstream ofs;
	ofs.open(filename,std::ios::out);
	ofs<<content;
	ofs.close();
	system("clear");
	exit(1);
}
void termial(){

	std::thread newthread(ListenKey,ifis,-1);
	if(newthread.joinable()) newthread.detach();

}
int main(int argc, char const *argv[])
{
	signal(SIGINT, save);
	system("clear");
	printf("input open file: ");
	scanf("%s",filename);
	std::ifstream ifs(filename);
	if(ifs.good()){
		if(ifs.read(content, 1000)){
			printf("%s",content);
		}
	}
	std::thread newthread(ListenKey,ifis,-1);
	if(newthread.joinable()) newthread.detach();
	while(1);
	return 0;
}
// int main(){
// 	int ch;
// 	while(true){
// 		ch=getchar();
// 		std::cout<<(float)ch<<std::endl;
// 	}
// }