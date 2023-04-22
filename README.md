# CTOOLS
### **ArrayList Tutorial**
```cpp
#include "ctools/arraylist/arraylist.h"
ArrayList<int> arr;
int val=20;
arr.append(val);//append the val to arr's back
arr.pop_back();//delete the arr's back value
arr.back();//return the arr's back value pointer
arr.get(index);//return the index postion pointer,if out of range,it'll return nullptr
arr.get(val);//return the val's index number
arr.clear();//clear the arr
int newval=40;
arr.replace(0,newval);//repalce the index 0 to newval
arr.printarray();//just print the arr :-)
arr.getSize();//get arr's length
arr.getArr();//return the arr[]
arr.getArr(start,end);//return the arr[start:end]
arr.getContainerSize();//the real array in arr's max container size
arr.deleteEle(index);//delete the index element
int* newarr=new int[10];
arr.setAllInfo(newarr,10,0);//custom the array by yourself
```
### **Parser**
```cpp
char start[]="/*";
char end[]="*/";
char testwords[]="/* hello im a test sentence */";
string testwords_str=testwords;
parser pr(start,end);
vector<string> res=pr.parsecontent(testwords);
vector<string> restwo=pr.parsecontent(testwords_str);

char *ans;char delims[]="/*";
ans=parsecontent(testwords,delims);//it'll return hello im a test sentence
vector<string> newres=parsecontentplus(testwords,delims);//it'll return {hello im a test sentence}
map<string,int> newresmap=parsecontentplusplus(testwords,delims);//it will return {hello im a test sentence : 1}
```
### **error**
```cpp
error errone;//create an empty error
error errtwo("im wrong");//create an error with something
errone.join("a new wrong");//join something in error
errone.join(errtwo);//join another erro into this error
errone.clear();//clear this error
errone.String();//output error's content 

```