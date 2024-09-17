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

#ifndef FILE_BUFFER_HPP
#define FILE_BUFFER_HPP

#include <fstream>
#include "buffer.hpp"

template<IO D> class BinaryFile;

template<> class BinaryFile<Out>{
private:
  string filename;
  ofstream file;
public:
  BinaryFile(const string filename);
  void close();
  void write(Byte* buffer, size_t size);
};

template<> class BinaryFile<In>{
private:
  string filename;
  ifstream file;
public:
  BinaryFile(const string filename);
  void close();
  size_t read(Byte* buffer, size_t size);
};

inline BinaryFile<Out>::BinaryFile(const string filename) {
  file.open(filename.c_str(), ios::binary | ios::trunc);
  assert(file.is_open());
}

inline void BinaryFile<Out>::close() {
  file.close();
}

inline void BinaryFile<Out>::write(Byte* buffer, size_t size) {
  file.write((const char*)buffer, size);
}

inline BinaryFile<In>::BinaryFile(const string filename) {
  file.open(filename.c_str(), ios::binary);
}

inline void BinaryFile<In>::close() {
  file.close();
}

inline size_t BinaryFile<In>::read(Byte* buffer, size_t size) {
  file.read((char*)buffer, size);
  return  file.gcount();
}

#endif
