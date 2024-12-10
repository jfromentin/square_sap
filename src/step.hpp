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

#ifndef STEP_HPP
#define STEP_HPP

#include <cstdint>
#include <cassert>

using namespace std;

enum Step:uint8_t {Left = 0, Right = 1, Up = 2, Down = 3};

char step_to_char(Step s);
Step char_to_step(char c);

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

#endif
