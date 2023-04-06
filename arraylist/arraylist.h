template<class T>
class ArrayList{
private:
    T* arr;
    int size;
    int top;
    void arraycopye(T *toarr, T *originarr,int langth);
public:
    ArrayList();
    ~ArrayList();
    void append(T val);
    void pop_back();
    T* back();
    T* get(int index);
    void clear();
    void replace(int index,T val);
    void printarray();
    int getSize();
    T* getArr();
    T* getArr(int begin,int end);
    int getContainerSize();
    void deleteEle(int index);
    void setAllInfo(T* arr_,int container_size,int top_);
};



template<class T>
void arraylistcopy(ArrayList<T>* dst,ArrayList<T>* src);


#include <iostream>
#include "string"

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
ArrayList<T>::~ArrayList()
{
    delete[] arr;
}

template <class T>
void ArrayList<T>::append(T val)
{
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
    delete[] arr;
    arr=new T[5];
}
template <class T>
void ArrayList<T>::replace(int index,T val)
{
    if (index<top)
    {
        arr[index]=val;
    }
    
}
template <class T>
void ArrayList<T>::printarray()
{
    for(size_t i=0;i<top;i++){
        // cout<<
    }
    std::cout<<std::endl;
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

template<class T>
void arraylistcopy(ArrayList<T> *dst, ArrayList<T> *src) {
    dst->setAllInfo(src->getArr(),src->getContainerSize(),src->getSize());
}
template<class T>
void ArrayList<T>::deleteEle(int index){
    for (int i=index; i<top-1; i++) {
        arr[i]=arr[i+1];
    }
    top--;
}
