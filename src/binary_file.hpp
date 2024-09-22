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

//! Class for In/Output binary file
template<IO D> class BinaryFile;

//! Class for output binary file. It allow us to have a Buffer's like interface
template<> class BinaryFile<Out>{
private:
  //! Filename of the file
  string filename;
  //! Output file stream
  ofstream file;
public:
  //! Empty constructor
  BinaryFile();
  //! Open the output BinaryFile from a filename
  //! \param filename name of the file to open
  void open(const string filename);
  //! Write Bytes in the output BinaryFile from a buffer
  //! \param the source buffer
  //! \param size number of bytes to write
  void write(Byte* buffer, size_t size);
  void clear();
  //! Close the  BinaryFile
  void close();
};

//! Class for input binary file. It allow us to have a Buffer's like interface
template<> class BinaryFile<In>{
private:
  //! Filename of the file
  string filename;
  //! Input file stream
  ifstream file;
public:
  //! Empty constructor
  BinaryFile();
  //! Open the input BinaryFile from a filename
  //! \param filename name of the file to open
  void open(const string filename);
  //! Write Bytes in the output Binary
  //! Construct an input BinaryFile
  //! \param filename  name of the file to open
  BinaryFile(const string filename);
   //! Read Bytes from the BinaryFiles to in the output BinaryFile from a bufferto a buffer
  //! \param buffer the destination buffer
  //! \param size maximum number of bytes to read
  //! \return the actual number ob Bytes read
  size_t read(Byte* buffer, size_t size);
  void clear();
  //! Close the BinaryFile
  void close();
};

inline BinaryFile<Out>::BinaryFile() {
}

inline void BinaryFile<Out>::open(const string filename) {
  assert(!file.is_open());
  file.open(filename.c_str(), ios::binary | ios::trunc);
  assert(file.is_open());
}

inline void BinaryFile<Out>::clear() {
}

inline void BinaryFile<Out>::close() {
  file.close();
}

inline void BinaryFile<Out>::write(Byte* buffer, size_t size) {
  assert(file.is_open());
  file.write((const char*)buffer, size);
}

inline BinaryFile<In>::BinaryFile() {
}

inline void BinaryFile<In>::open(const string filename) {
  file.open(filename.c_str(), ios::binary);
}

inline void BinaryFile<In>::clear() {
}
inline void BinaryFile<In>::close() {
  file.close();
}

inline size_t BinaryFile<In>::read(Byte* buffer, size_t size) {
  file.read((char*)buffer, size);
  return  file.gcount();
}

#endif
