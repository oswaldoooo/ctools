#include "calculator.h"
#include "tools/tools.h"
// #include "tools/tools.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>
ctools::math::binaryNum ctools::math::transfer(int origin){
	if(origin==0){
		char arr[]={'0'};
		binaryNum bn(arr,1);
		return bn;
	}else{
		float val=log2(origin);
		int length=floor(val)+1;
		char arr[length+1];int pos=0;
		std::map<int, bool> resmap;
		while (origin>0&&val>=0) {
			
			//使用计数法
			if(origin>0) resmap.insert({floor(val),true});
			origin-=pow(2,floor(val));
			val=log2(origin);
			pos++;
		}
		//剩余部分全部0填充
		memset(arr, '0', sizeof(char)*(length));
		for(auto &[key,val]:resmap){
			arr[length-key-1]=1+'0';
		}
		binaryNum ans(arr,length);
		return ans;
	}
}
std::ostream &operator<<(std::ostream &input,const ctools::math::binaryNum &origin){
	for(int i=0;i<origin.length;i++){
		input<<origin.arr[i];
	}
	return input;
}

ctools::Ans<ctools::math::binaryNum> ctools::math::transfer(const char* origin){
	int num;binaryNum start=transfer(0);
	error err;
	for(int i=0;i<strlen(origin);i++){
		num=origin[i]-'0';
		if(num<10&&num>=0){
			//char is a number 
			num*=pow(10,strlen(origin)-1-i);
			start+=transfer(num);
		}else{
			err.join("Non number char is included");
		}
	}
	if(!err.isNull()) printf("error is %s\n", err.String());
	Ans<binaryNum> res(start);
	res.err=err;
	return res;

}

void ctools::math::expand(binaryNum &origin,unsigned int length){
	if(origin.length>=length) return;//do nothing
	char *newarr=new char[length+1];
	memset(newarr, 0, sizeof(int)*length);
	memmove(newarr+length-origin.length, origin.arr, sizeof(int)*origin.length);//copy the origin's array to newarray
	delete [] origin.arr;
	origin.arr=newarr;
	origin.length=length;
}
//二进制计算

ctools::math::binaryNum operator+(const ctools::math::binaryNum &left,const ctools::math::binaryNum &right){
	using namespace std;
	int minlen=min(left.length,right.length);
	int maxlen=max(left.length,right.length);
	char *ansarr=new char[maxlen+2];
	memset(ansarr, 0, sizeof(char)*(maxlen+2));
	int buff=0;int mid;
	//the same postion calculate first
	for(int i=0;i<minlen;i++){
		mid=left.arr[left.length-i-1]+right.arr[right.length-1-i]+buff-'0'*2;
		buff=mid/2;
		mid=mid%2;
		ansarr[maxlen-i]=mid+'0';//int to char
	}
	int e=minlen;
	//different position calculate then
	if((left.length-minlen)>0){
		while(buff>0&&e<left.length){
			mid=buff+left.arr[left.length-e-1]-'0';//char to int
			buff=mid/2;
			mid=mid%2;
			ansarr[maxlen-e]=mid+'0';//int to char
			e++;
		}
	}else if((right.length-minlen)>0){
		while(buff>0&&e<right.length){
			mid=buff+right.arr[right.length-e-1]-'0';
			buff=mid/2;
			mid=mid%2;
			ansarr[maxlen-e]=mid+'0';
			e++;
		}
	}
	if(buff>0) ansarr[0]='1';
	ctools::math::binaryNum res(ansarr,maxlen+1);
	delete [] ansarr;
	return res;
}
//warning - override is still not complete!!!dont use it
ctools::math::binaryNum operator-(const ctools::math::binaryNum &left, const ctools::math::binaryNum &right){
	using namespace std;
	int minlen=min(left.length,right.length);
	int maxlen=max(left.length,right.length);
	char ansarr[maxlen+2];
	memset(ansarr, '0', sizeof(int)*(maxlen+1));
	int buff=0;int mid;
	//the same postion calculate first
	for(int i=0;i<minlen;i++){
		mid=left.arr[left.length-i-1]-right.arr[right.length-1-i]+buff;
		buff=mid/2;
		mid=abs(mid%2);//the abs of 
		ansarr[maxlen-i]=mid+'0';//int to char
	}
	int e=minlen;
	//different position calculate then
	if((left.length-minlen)>0){
		while(buff>0&&e<left.length){
			mid=buff+left.arr[left.length-e-1]-'0';//char to int
			buff=mid/2;
			mid=mid%2;
			ansarr[maxlen-e]=mid+'0';//int to char
			e++;
		}
	}else if((right.length-minlen)>0){
		while(buff>0&&e<right.length){
			mid=buff+right.arr[right.length-e-1]-'0';
			buff=mid/2;
			mid=mid%2;
			ansarr[maxlen-e]=mid+'0';
			e++;
		}
	}
	if(buff>0){
		ansarr[0]='1';
	}
	ctools::math::binaryNum res(ansarr,maxlen+1);
	return res;	
}