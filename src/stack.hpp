//****************************************************************************//
//    Copyright (C) 2024 Jean Fromentin <jean.fromentin@univ-littoral.fr>     //
//                                                                            //
//  This file is part of SquareSAP <https://github.com/jfromentin/square_sap> //
//                                                                            //
//  SquareSAP is free software: you can redistribute it and/or modify it      //
//  under the terms of the GNU General Public License as published by the     //
//  Free Software Foundation, either version 3 of the License, or             //
//  (at your option) any later version.                                       //
//                                                                            //
//  SquareSap is distributed in the hope that it will be useful, but WITHOUT  //
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     //
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License      //
//  for  more details.                                                        //
//                                                                            //
//  You should have received a copy of the GNU General Public License along   //
//  with SquareSAP. If not, see <https://www.gnu.org/licenses/>.              //
//****************************************************************************//

#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>

using namespace std;

template<class T>
class Stack{
private:
  size_t capacity;
  size_t size;
  T** data;
public:
  Stack(size_t capacity);
  void pop(T** S);
  void push();
  T* next();
  bool is_empty();
  ~Stack();
};

template<class T> Stack<T>::Stack(size_t c) {
  capacity = c;
  data = new T*[capacity];
  for(size_t i = 0; i < capacity; ++ i) {
    data[i] = new T;
  }
  size = 0;
}


template<class T>
inline void Stack<T>::pop(T** S) {
  swap(*S, data[-- size]);
}

template<class T>
inline void Stack<T>::push() {
  size ++;
  assert(size <= capacity);
}

template<class T>
inline T* Stack<T>::next() {
  return data[size];
}

template<class T>
inline bool Stack<T>::is_empty() {
  return size==0;
}

template<class T>
Stack<T>::~Stack() {
  for(size_t i = 0; i < capacity; ++ i) {
    delete data[i];
  }
  delete[] data;
}

#endif
