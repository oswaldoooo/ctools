#include <mutex>
#include "arraylist.h"
// #include <iterator>
std::mutex mt;
using namespace ctools;
template<class T>
void arraylistcopy(ArrayList<T>* dst,ArrayList<T>* src);


// #include <iostream>
// #include "string"

template <class T>
void ArrayList<T>::arraycopye(T *toarr, T *originarr, int langth)
{
    T* onehead=toarr;
    T* twohead=originarr;
    for(int e=0;e<langth;e++){
        *onehead=*twohead;
        onehead++;
        twohead++;
    }
}

template <class T>
ArrayList<T>::ArrayList()
{
    size=5;
    arr=new T[size];
    top=0;
}

template <class T>
ArrayList<T>::ArrayList(T *arr_, int container_size, int top_)
{
    arr=arr_;
    size=container_size;
    top=top_;
}

template <class T>
ArrayList<T>::~ArrayList()
{
    delete[] arr;
}

template <class T>
void ArrayList<T>::append(T val)
{
    std::lock_guard lg(mt);//add lock
    if (top>=size)
    {
        size=size*2;
        T* newarr=new T[size];
        arraycopye(newarr,arr,top);
        delete[] arr;
        arr=newarr;
    }
    arr[top]=val;
    top++;
}
template <class T>
void ArrayList<T>::pop_back()
{
    std::lock_guard lg(mt);
    top--;
}
template <class T>
T* ArrayList<T>::back()
{
    return arr+top-1;
}
template <class T>
T* ArrayList<T>::get(int index)
{
    if (index>=top)
    {
        return nullptr;
    }
    
    return arr+index;
}
template <class T>
void ArrayList<T>::clear()
{
    std::lock_guard lg(mt);
    delete[] arr;
    arr=new T[5];
}
template <class T>
void ArrayList<T>::replace(int index,T val)
{
    if (index<top)
    {
        std::lock_guard lg(mt);
        arr[index]=val;
    }
    
}
template <class T>
std::string ArrayList<T>::printarray()
{
    for(size_t i=0;i<top;i++){
        std::cout<<arr[i];       
    }
    std::cout<<std::endl;
    return "";
}
template <class T>
int ArrayList<T>::getSize()
{
    return top;
}

template<class T>
int ArrayList<T>::getContainerSize() {
    return size;
}

template<class T>
T *ArrayList<T>::getArr() {
    T* newhead=new T[top];
    for (size_t i=0; i<top; i++) {
        *(newhead+i)=*(arr+i);
    }
    return newhead;
}
template<class T>
T *ArrayList<T>::getArr(int begin,int end) {
    if (end>=top||begin<0) {
        return nullptr;
    }
    T* newarr=new T[end-begin];
    T* newhead=arr+begin;
    for (size_t i=0; i<end-begin; i++) {
        newarr[i]=*newhead;
        newhead++;
    }
    return newarr;
}

template<class T>
void ArrayList<T>::setAllInfo(T *arr_, int container_size, int top_) {
    arr=arr_;
    size=container_size;
    top=top_;
}

template <class T>
ArrayList<T> *ArrayList<T>::getArray(int begin, int end)
{
    if (end>=top||begin<0) {
        return nullptr;
    }
    T* newarr=new T[end-begin];
    T* newhead=arr+begin;
    for (size_t i=0; i<end-begin; i++) {
        newarr[i]=*newhead;
        newhead++;
    }
    ArrayList<T>* newarrlist=new ArrayList<T>(newarr,end-begin,end-begin);
    return newarrlist;
}

template <class T>
std::output_iterator_tag ctools::ArrayList<T>::begin()
{
    return std::output_iterator_tag();
}

template<class T>
void arraylistcopy(ArrayList<T> *dst, ArrayList<T> *src) {
    dst->setAllInfo(src->getArr(),src->getContainerSize(),src->getSize());
}
template<class T>
void ArrayList<T>::deleteEle(int index){
    std::lock_guard lg(mt);
    for (int i=index; i<top-1; i++) {
        arr[i]=arr[i+1];
    }
    top--;
}

template <class T>
int ArrayList<T>::get(const T &val)
{
    int pos=0;
    return this->getpos(val,pos);
}

template <class T>
int ArrayList<T>::getpos(const T &val, int pos)const
{
    if (pos<top)
    {
        if (val==arr[pos])
        {
            return pos;
        }
        int left=this->getpos(val,2*pos+1);
        int right=this->getpos(val,2*pos+2);
        if (left>=0)
        {
            return left;
        }else if (right>=0)
        {
            return right;
        }
    }
    return -1;
}

//beta edition
template <class T>
const T & ArrayList<T>::operator[](int i) const
{
    if (i<top&&i>=0)
    {
        return arr[i];
    }
    throw "index out of range";
    // return arr[0];
    
}

template <class T>
T &ArrayList<T>::operator[](int i)
{
    if (i<top&&i>=0)
    {
        return arr[i];
    }
    throw "index out of range";
}