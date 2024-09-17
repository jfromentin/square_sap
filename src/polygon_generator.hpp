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

#include <cassert>
#include <cstdint>
#include <iostream>
#include <lzma.h>
#include "array.hpp"
#include "gameboard.hpp"
#include "polygon.hpp"
#include "polygon_buffer.hpp"
#include "binary_file.hpp"
#include "stack.hpp"
#include "lzma_buffer.hpp"

using namespace std;

// 4 ->                 1
// 6 ->                 2
// 8 ->                 7
//10 ->                28
//12 ->               124
//14 ->               588
//16 ->             2 938
//18 ->            15 268
//20 ->            81 826
//22 ->           449 572
//24 ->          252 1270
//26 ->        14 385 376
//28 ->        83 290 424
//30 ->       488 384 528
//32 ->     2 895 432 660
//34 ->    17 332 874 364
//36 ->   104 653 427 012
//38 ->   636 737 003 384
//40 -> 3 900 770 002 646


char to_char(Step s);

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

static const size_t BUFFER_SIZE = 4096;
class PolygonGenerator{
private:
  size_t l;
  GameBoard gameboard;
  Polygon word;
  size_t kappa[max_length];
  Stack<StackInfo> stack;
  StackInfo* current_stack_info;
  uint8_t apply(uint16_t c, uint8_t k, Step s);
  void try_add(size_t c, size_t cn, size_t k, Step s);
  Array<string> split_prefixs(string prefixs);
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
