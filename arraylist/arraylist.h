#pragma once
#ifndef arraylist_h
#define arraylist_h
#include "string"
#include <iostream>
#include <iterator>
namespace ctools
{
    template<class T>
    class ArrayList{
    private:
        T* arr;
        int size;
        int top;
        void arraycopye(T *toarr, T *originarr,int langth);
        int getpos(const T & val,int pos)const;
    public:
        ArrayList();
        ArrayList(T* arr_,int container_size,int top_);
        ~ArrayList();
        const T & operator[](int i)const; //beta edition
        T &operator[](int i);
        void append(T val);
        void pop_back();
        T* back();
        T* get(int index);
        int get(const T & val);
        void clear();
        void replace(int index,T val);
        std::string printarray();
        int getSize();
        T* getArr();
        T* getArr(int begin,int end);
        int getContainerSize();
        void deleteEle(int index);
        void setAllInfo(T* arr_,int container_size,int top_);
        ArrayList* getArray(int begin,int end);
        std::output_iterator_tag begin();
        std::output_iterator_tag end();
    };
} // namespace ctools
#endif