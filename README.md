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