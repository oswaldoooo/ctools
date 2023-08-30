
# CTOOLS
## **Install**
```shell
wget https://brotherhoodhk.org/products/shells/ctools_installer.sh
sudo bash ctools_installer.sh
```
## **Uninstall**
```
wget https://brotherhoodhk.org/products/shells/ctools_uninstall.sh
sudo bash ctools_uninstall.sh
```
## **New Features**
#### **Logger Use** 30 August 2023
```cpp
#include <ctools/output.hpp>
int main(){
    //this will output to stdout directly
    logout("hello ctools!");
    //this will write to test.log
    int fid=open("test.log", O_CREAT | O_WRONLY | O_APPEND, 0640);
    if (fid < 0) throw std::logic_error("open test.log failed");
    logout("hello ctools!",fid);
    close(fid);
    return 0;
}
```
#### **Net-Kits and New FIFO** 6 july 2023
add the fifo class and netkits header.them all are header-only.
examples here
```cpp
#include "ctools/net/fifo.hpp"
#include <string>
int main(){
    ctools::FIFO fiforead("fifo",ctools::mod::read);
    ctools::FIFO fifowrite("fifo",ctools::mod::write);
    fifowrite.write("hello world");
    std::string ans=fiforead().read();
    printf("the accept from fifo %s\n",ans.c_str());
    //you can also do like this
    fifowrite<<"hello world";
    char buffer[1<<10];
    fiforead>>buffer;
    printf("the accept from fifo %s\n",buffer);
    return 0;
}
```
#### **Cmirco NetKits** 24 june 2023
example here
```cpp
int main(){
    int msgpipe[2];int anspipe[2];int ok;
    ok=pipe(msgpipe);
    if(ok==-1)throw std::logic_error("create msg pipe failed");
    ok=pipe(anspipe);
    if(ok==-1)throw std::logic_error("create ans pipe failed");
    switch (fork()) {//open a new process for dial to server
    case 0:
        //goto child process
        {
            printf("current pid %d\n", getpid());
            ctools::Bstclient bcl("bst://127.0.0.1:8000/v1/greet",msgpipe,anspipe);
            // ctools::Bsclient bcl(localhost)
            bcl.connect();
            dial(&bcl, msgpipe[0], anspipe[1]);
            exit(-1);
        }
    case -1:
        //failed
        printf("create child fork failed\n");
        break;
    }
    char inbuffer[1<<10];
    char outbuffer[1<<10];
    struct Name name;struct Ans ans;
    char first_name[20];char last_name[20];
    name.first_name=first_name;name.last_name=last_name;
    std::string res;int wok;int rok;
    printf("prepare binding keys,now pid is %d\n",getpid());
    while(true){
        if(strlen(outbuffer)>0)memset(outbuffer, 0, sizeof(char)*strlen(outbuffer));
        std::cout<<"name: ";
        std::cin>>name.first_name>>name.last_name;
        res=ctools::marshal(&name);//encode
        wok=write(msgpipe[1], res.c_str(),1<<10);//send to server
        if (wok!=-1){
            printf("wait from back\n");
            rok=read(anspipe[0], outbuffer, 1<<10);
            printf("get from back\n");
            if (rok!=-1){
                ctools::unmarshal(&ans, outbuffer);//decode
                printf("from server:%s\n", outbuffer);
            }else{
                printf("read from ans pipe failed\n");
            }
        }else{
            printf("write to msg pipe failed\n");
        }
    }
    //continue
    return 0;
}
```

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
### **CinPlus**
```cpp
#include "tools/cinplus.h"
#include "tools/cinsecure.h"
char buff[100];
//Ultra will hide your input,it equal to plus + secure
if(getLineUltra(buff,20)){//the input will store in buff,and the wait you time most is 20 seconds,or it will return false,time out
    printf("%s\n",);
}else{
    printf("time out\n");
}

//plus will not hide your input,but it still wait you with the limit time
cinplus<int> newcin;//it's use default waittime 10 seconds
if(newcin.get()){
    printf("%d\n",newcin.load());    
}else{
    printf("time out\n");
}

if(newcin.getline()){
    newcin.getstr();
}else{
    printf("time out\n");
}

//cinsecure,just hidden input,not input limit time
getInput(buff);
printf("%s\n",buff);
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
char filename[]="site.cnf";
map<string,string> ans=parselist(site.cnf);
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
### **array**
```cpp
ctools::array::arraymove(src, move_step, ctools::array::forwards::left, array_length);//it might throw exception
```