#include "boost/asio.hpp"
#include "boost/asio/buffer.hpp"
#include "boost/asio/io_service.hpp"
#include "boost/asio/ip/address.hpp"
#include "boost/system/detail/error_code.hpp"
#include "parser/marshaler.h"
#include "show.h"
#include "netkits.h"
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <regex>
using namespace boost::asio;
typedef std::string string;
namespace ctools {
	class Bsclient:public Client{
	private:
		int *msgpipe;
		int *anspipe;
	public:
		bool close=false;
		bool needwait=true;
		ip::tcp::endpoint end;
		io_service io;
		ip::tcp::socket *sock;
		Bsclient(string address,int port){
			msgpipe=new int[2];
			anspipe=new int[2];
			if(pipe(msgpipe)==-1) throw std::logic_error("create msgpipe failed");
			if(pipe(anspipe)==-1) throw std::logic_error("create anspipe failed");
			end=ip::tcp::endpoint(ip::address::from_string(address),port);
			sock=new ip::tcp::socket(io);
		}
		int msgRPipe(){
			return msgpipe[0];
		}
		int msgWPipe(){
			return msgpipe[1];
		}
		int ansRPipe(){
			return anspipe[0];
		}
		int ansWPipe(){
			return anspipe[1];
		}
		void connect(){
			sock->connect(end);
		}
		bool isclose() override{
			return close;
		}
		bool needwaitreturn() override{
			return needwait;
		}
		void getback() override{

		}
		void write_some(std::string &data, boost::system::error_code &ec) override{
			sock->write_some(boost::asio::buffer(data),ec);
		}
		std::string read_some(boost::system::error_code &ec) override{
			string data;
			sock->read_some(boost::asio::buffer(data),ec);
			if (!ec){
				if(write(anspipe[1],data.c_str(),sizeof(char)*data.length())==-1){
					printf("write to anspipe failed\n");
				}

			}
			return data;
		}
	};

	class Bstclient:public Client{
	private:
		int *msgpipe;
		int *anspipe;
		string path;
	public:
		bool close=false;
		bool needwait=true;
		ip::tcp::endpoint end;
		io_service io;
		ip::tcp::socket *sock;
		Bstclient(string url){
			#define reg_exp "^bs(t|u|)://(.*):([\\d]{1,5})/(.*)"
			std::regex expression(reg_exp);
			std::smatch match;
			if(!std::regex_search(url,match,expression))throw std::logic_error("bs url is not vaild");
			int port=atoi(match[3].str().c_str());
			path=match[4].str();
			msgpipe=new int[2];
			anspipe=new int[2];
			if(pipe(msgpipe)==-1) throw std::logic_error("create msgpipe failed");
			if(pipe(anspipe)==-1) throw std::logic_error("create anspipe failed");
			end=ip::tcp::endpoint(ip::address::from_string(match[2]),port);
			sock=new ip::tcp::socket(io);
		}
		int msgRPipe(){
			return msgpipe[0];
		}
		int msgWPipe(){
			return msgpipe[1];
		}
		int ansRPipe(){
			return anspipe[0];
		}
		int ansWPipe(){
			return anspipe[1];
		}
		void connect(){
			sock->connect(end);
		}

		bool isclose() override{
			return close;
		}
		bool needwaitreturn() override{
			return needwait;
		}
		void getback() override{

		}
		void write_some(std::string &data, boost::system::error_code &ec) override{
			sock->write_some(boost::asio::buffer(data),ec);
		}
		string read_some(boost::system::error_code &ec) override{
			string data;
			sock->read_some(boost::asio::buffer(data),ec);
			if (!ec){
				if(write(anspipe[1],data.c_str(),sizeof(char)*data.length())==-1){
					printf("write to anspipe failed\n");
				}

			}
			return data;
		}
	};

}





void dial(ctools::Bsclient *cl,int,int);
void dial(ctools::Bsclient *cl,int msgrpipe,int answpipe){
	char cachebuffer[1<<10];boost::system::error_code ec;
	char outbuffer[1<<10];
	// debug point
	string tosend;
	// cl->connect();
	string accp;
	while(!cl->isclose()){
		ec.clear();
		if(strlen(outbuffer)>0)memset(outbuffer, 0, sizeof(char)*strlen(outbuffer));
		if(strlen(cachebuffer)>0)memset(cachebuffer, 0, strlen(cachebuffer)*sizeof(char));
		if(read(msgrpipe,cachebuffer,1<<10)==-1){

		}else{
			tosend=cachebuffer;
			// debug point
			printf("accept from front %s\n", tosend.c_str());
			cl->sock->write_some(buffer(tosend),ec);
			if (ec){
				std::cout<<ec.what()<<std::endl;
			}else{
				// debug point
				if (cl->needwaitreturn()){
					// cl->sock->read_some(buffer(outbuffer),ec);
					printf("wait msg\n");
					cl->sock->read_some(buffer(outbuffer),ec);
					if (ec){
						std::cout<<"error:"<<ec.what()<<std::endl;		
					}else{
						// debug point
						printf("write to front pipe,msg %s\n",outbuffer);
						//write to pipe
						if(write(answpipe, outbuffer, sizeof(char)*strlen(outbuffer))==-1){
							printf("write to front failed\n");
						}
					}
				}
			}
		}
	}
}