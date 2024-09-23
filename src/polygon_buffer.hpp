//****************************************************************************//
//    Copyright (C) 2024 Jean Fromentin <jean.fromentin@univ-littoral.fr>     //
//                                                                            //
//  This file is part of SquareSAP <https://github.com/jfromentin/square_sap> //
//                                                                            //
//  HP-Combi is free software: you can redistribute it and/or modify it       //
//  under the terms of the GNU General Public License as published by the     //
//  Free Software Foundation, either version 3 of the License, or             //
//  (at your option) any later version.                                       //
//                                                                            //
//  HP-Combi is distributed in the hope that it will be useful, but WITHOUT   //
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     //
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License      //
//  for  more details.                                                        //
//                                                                            //
//  You should have received a copy of the GNU General Public License along   //
//  with SquareSAP. If not, see <https://www.gnu.org/licenses/>.              //
//****************************************************************************//

#ifndef POLYGON_STREAM_HPP
#define POLYGON_STREAM_HPP

#include "polygon.hpp"
#include "bit_buffer.hpp"

template<IO D, class T> class PolygonBuffer;


template<class T> class PolygonBuffer<Out, T>{
private:
  size_t length;
  Polygon last;
  BitBuffer<Out,T> buffer;
public:
  PolygonBuffer();
  void open(T* output, size_t l);
  void write(const Polygon& polygon);
  void clear();
  void close();
};

template<class T> class PolygonBuffer<In, T>{
private:
  size_t length;
  Polygon last;
  BitBuffer<In, T> buffer;
public:
  PolygonBuffer();
  void open(T* input, size_t length);
  void read(Polygon& P);
  void clear();
  void close();
};

template<class T> inline PolygonBuffer<Out, T>::PolygonBuffer() {
}

template<class T> inline void PolygonBuffer<Out, T>::open(T* output, size_t l) {
  buffer.open(output);
  length = l;
  last.set_length(l);
}

template<class T> inline void PolygonBuffer<Out, T>::write(const Polygon& p) {
  size_t lp = p.common_prefix(last);
  buffer.write_int(lp, 6);
  for (size_t i = lp; i < length - 1; ++ i) {
    buffer.write_int(p[i], 2);
  }
  last = p;
}

template<class T> inline void PolygonBuffer<Out, T>::clear() {
  last.set_length(length);
  buffer.clear();
}

template<class T> inline void PolygonBuffer<Out, T>::close() {
  buffer.close();
}

template<class T> inline PolygonBuffer<In, T>::PolygonBuffer() {
}

template<class T> inline void PolygonBuffer<In, T>::open(T* input, size_t l) {
  buffer.open(input);
  length = l;
  last.set_length(length);
}


template<class T> inline void PolygonBuffer<In, T>::read(Polygon& P) {
  P = last;
  size_t lp = buffer.read_int(6);
  for (size_t i = lp; i < length - 1; ++ i) {
    P[i] = (Step)buffer.read_int(2);
  }
  last = P;
}

template<class T> inline void PolygonBuffer<In, T>::clear() {
  last.set_length(length);
  buffer.clear();
}

template<class T> inline void PolygonBuffer<In, T>::close() {
  buffer.close();
}

#endif
