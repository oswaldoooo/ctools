#pragma once
#ifndef calculator_h
#define calculator_h
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "tools/tools.hpp"
namespace ctools {
	namespace math {
		struct binaryNum{
			char *arr;
			unsigned int length;
			binaryNum()=delete;
			binaryNum(const binaryNum &origin){
				//copy construct method
				arr=new char[origin.length];
				length=origin.length;
				memcpy(arr, origin.arr, sizeof(char)*length);
			}
			binaryNum(const char *origin,const unsigned int length_):length(length_){
				arr=new char[length_];
				std::memcpy(arr,origin,sizeof(char)*length_);
			}
			operator int(){
				//转换为int
				int res=0;
				for(int i=length-1;i>=0;i--){
					if(arr[i]=='1') res+=pow(2,length-i-1);
				}
				return res;
			}
			operator char*(){
				return arr;
			}
			void format(){
				int i=0;
				while(i++<length&&arr[i]!='1');
				ctools::array::arraymove(arr,i,array::forwards::left,length);//left move drop the '0'
			}
			~binaryNum(){delete [] arr;}
			const binaryNum operator++(int){
				binaryNum temp(arr,length);
				//if first is 0,it'll be 1
				int i=length-1;
				while(i>=0&&arr[i]=='1'){
					arr[i]=0+'0';
					i--;
				}
				arr[i]=1+'0';
				return temp;
			}
			binaryNum &operator+=(const binaryNum &right){
				using namespace std;
				int minlen=min(this->length,right.length);
				int maxlen=max(this->length,right.length);
				char *ansarr=new char[maxlen+2];
				memset(ansarr, '0', sizeof(char)*(maxlen+1));
				// printf("ansarr is  %s,right is %s\n", ansarr,(char*)right.arr);
				int buff=0;int mid;
				//the same postion calculate first
				for(int i=0;i<minlen;i++){
					mid=this->arr[this->length-i-1]+right.arr[right.length-1-i]+buff-'0'*2;
					buff=mid/2;
					mid=mid%2;
					ansarr[maxlen-i]=mid+'0';//int to char
				}
				int e=minlen;
				//different position calculate then
				if((this->length-minlen)>0){
					while(buff>0||e<this->length){
						mid=buff+this->arr[this->length-e-1]-'0';//char to int
						buff=mid/2;
						mid=mid%2;
						ansarr[maxlen-e]=mid+'0';//int to char
						e++;
					}
				}else if((right.length-minlen)>0){
					while(buff>0||e<right.length){
						mid=buff+right.arr[right.length-e-1]-'0';
						buff=mid/2;
						mid=mid%2;
						ansarr[maxlen-e]=mid+'0';
						e++;
					}
				}
				if(buff>0) ansarr[0]='1';
				delete [] this->arr;//delete before array
				this->arr=ansarr;
				this->length=maxlen+1;
				return *this;
			}
		};
		// binaryNum transfer(const char *origin);
		binaryNum transfer(int origin);
		Ans<binaryNum> transfer(const char * origin);
		char *transfer(const binaryNum origin);
		//expand the binary number to target length
		void expand(binaryNum &origin,unsigned int length);
	}
}
std::ostream &operator<<(std::ostream &input,const ctools::math::binaryNum &origin);
ctools::math::binaryNum operator+(const ctools::math::binaryNum &left,const ctools::math::binaryNum &right);
ctools::math::binaryNum operator-(const ctools::math::binaryNum &left, const ctools::math::binaryNum &right);

#endif