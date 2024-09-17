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

#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include "config.hpp"

using namespace std;

enum Step:uint8_t {Left = 0, Right = 1, Up = 2, Down = 3};

char step_to_char(Step s);
Step char_to_step(char c);

class Polygon{
private:
  size_t length;
  Step steps[max_length];
public:
  Polygon();
  Polygon& operator=(const Polygon& p);
  void set_length(size_t length);
  Step& operator[](size_t i);
  Step operator[](size_t i) const;
  void display() const;
  size_t common_prefix(const Polygon& p) const;
};

inline char step_to_char(Step s) {
  switch(s){
  case Left:
    return 'L';
  case Right:
    return 'R';
  case Up:
    return 'U';
  case Down:
    return 'D';
  default:
    return '?';
    break;
  };
}
inline Step char_to_step(char c) {
  switch(c){
  case 'L':
    return Left;
  case 'R':
    return Right;
  case 'U':
    return Up;
  case 'D':
    return Down;
  default:
    assert(false);
    break;
  };
  return Right;
}

inline Polygon::Polygon() {
}

inline Polygon& Polygon::operator=(const Polygon& P) {
  length = P.length;
  for (size_t i = 0; i < length; ++ i) {
    steps[i] = P.steps[i];
  }
  return *this;
}

inline void Polygon::set_length(size_t l) {
  length = l;
  for(size_t i = 0; i < l / 2 - 1; ++ i) {
    steps[i] = Right;
    steps[i + l / 2] = Left;
  }
  steps[l / 2 - 1] = Up;
  steps[l - 1] = Down;
}

inline Step& Polygon::operator[](size_t i) {
  assert(0 <= i);
  assert(i< length);
  return steps[i];
}

inline Step Polygon::operator[](size_t i) const {
  assert(0 <= i);
  assert(i< length);
  return steps[i];
}

inline void Polygon::display() const {
  for (size_t i = 0; i < length; ++ i) {
    cout << step_to_char(steps[i]);
  }
}

inline size_t Polygon::common_prefix(const Polygon& P) const {
  size_t i;
  for (i = 0; i < length; ++ i){
    if (steps[i] != P.steps[i]) return i;
  }
  return i;
}
#endif
