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

#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include <iostream>
#include <cstdint>
#include <cassert>
#include "config.hpp"

//! CellData is an 16 bits unsigned integer composed of two 8bits unsigned integer :
//! - d which is the minimal number of steps needed to reach the base cell.
//! - t which is the position, in the path, of the step that reach the cell
using CellData = uint16_t;

//! Construct a CellData from the values d and t
//! \param d minimal number of steps needed to reach the base cell
//! \param t time when the curent path reach that cell.
inline CellData init_cell_data(uint8_t d, uint8_t t) {
  return  d + (t << 8);
}

//! Return the value d of a given CellData
//! \param c a CellData
inline uint8_t get_d(CellData c) {
  return c & 255;
}

//! Return the value t of a given CellData
//! \param c a CellData
inline uint8_t get_t(CellData c) {
  return c >> 8;
}

//! Set the value t of a given CellData
//! \param c a CellData
//! \parma t the value to set
inline void set_t(CellData &c, uint8_t t) {
  c &= 127;
  c += (t << 8);
}

//! Maximal width of the GameBoard (with borders)
static const size_t max_width = max_length - 1;
//! Maximal height of the GameBoard (with borders)
static const size_t max_height = max_length / 2 + 2;

//! Class for the Gmaeboard where self avoiding polygons will be contructed
class GameBoard{
private:
  //! Number of cells in the maximal GameBoard, this for self avoinding
  //! polygons of length max_length
  static const size_t size = max_width * max_height;
  //! An array of CellData for each Cell of the maximal GameBoard
  CellData tab[size];
  //! Size of self avoing polygons we want to construc
  size_t length;
  //! Actual width of the considered GameBoard
  size_t width;
  //! Actual heaight ot the considered GameBoard
  size_t height;
  
  size_t x_base;
  const size_t y_base = 1;
  size_t pos(size_t x, size_t y) const;
  bool on_border(size_t x, size_t y);
public:
  GameBoard(size_t length);
  CellData& operator[](size_t size);
  const CellData& operator[](size_t size) const;
  size_t base_cell() const;

};

inline size_t GameBoard::pos(size_t x, size_t y) const {
  return x + y *  max_width;
}

inline CellData& GameBoard::operator[](size_t c) {
  return tab[c];
}

inline const CellData& GameBoard::operator[](size_t c) const {
  return tab[c];
}

inline size_t GameBoard::base_cell() const {
  return pos(x_base, y_base);
}

#endif
