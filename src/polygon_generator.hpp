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

#ifndef POLYGON_GENERATOR_HPP
#define POLYGON_GENERATOR_HPP

#include "array.hpp"
#include "gameboard.hpp"
#include "polygon.hpp"
#include "polygon_buffer.hpp"
#include "binary_file.hpp"
#include "stack.hpp"
#include "lzma_buffer.hpp"

using namespace std;

using StackInfo = uint32_t;

inline StackInfo init_stack_info(uint16_t c, uint8_t k, Step s) {
  return  c + (k << 16) + (s << 24);
}

inline uint16_t get_c(StackInfo s) {
  return s & 65535;
}

inline uint8_t get_k(StackInfo s) {
  return (s >> 16) & 255;
}

inline Step get_s(StackInfo s) {
  return (Step)(s >> 24);
}

class PolygonGenerator{
private:
  size_t length;
  GameBoard gameboard;
  Polygon word;
  size_t kappa[max_length];
  Stack<StackInfo> stack;
  StackInfo* current_stack_info;

  //Output
  ofstream output_data;
  BinaryFile<Out> output_file;
  LzmaBuffer<Out, BinaryFile<Out>> lzma_output;
  PolygonBuffer<Out, LzmaBuffer<Out, BinaryFile<Out>>> polygon_buffer;
  
  //Data
  string filename;
  string prefixs;
  size_t split;
  size_t file_number;
  size_t file_size;
  size_t sap_number;

  void write_sap();
  
  uint8_t apply(uint16_t c, uint8_t k, Step s);
  void try_add(size_t c, size_t cn, size_t k, Step s);
  Array<string> split_prefixs(string prefixs);
  void init_output();
  void close_output();

  void exec_prefix(string prefix);
  
public:
  PolygonGenerator(size_t length);
  ~PolygonGenerator();
  void init(string prefix);
  size_t exec(string prefixs, string filename, size_t split);
  
  void compress(string filename);
  void decompress(string filename);
};

inline PolygonGenerator::~PolygonGenerator() {
  delete current_stack_info;
}

#endif
