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

#ifndef GRID_HPP
#define GRID_HPP

#include "config.hpp"
#include "step.hpp"

//! Maximal width of the Grid (with borders)
static const size_t max_width = max_length - 1;
//! Maximal height of the Grid (with borders)
static const size_t max_height = max_length / 2 + 2;

using CellData = uint16_t;

//! Class for the Gameboard where self avoiding polygons will be contructed
class Grid{
private:
  //! Number of cells in the maximal GameBoard, this for self avoinding
  //! polygons of length max_length
  static const size_t size = max_width * max_height;
  //! An array of CellData for each Cell of the maximal GameBoard
  CellData tab[size];
  //! x coordinate of the base cell
  size_t x_base;
  //! y coordinate of the base cell, it is always equal to 1
  const size_t y_base = 1;
  //! Give the index of the cell at position (x,y)
  static size_t pos(size_t x, size_t y);
public:
  //! Construct a Grid
  //! \param length length of the self avoidinf polygons we want to construct
  Grid(size_t length);
  //! Return access to the CellData of the cell of index i  
  CellData& operator[](size_t i);
  //! Return CellData of the cell of index i
  const CellData& operator[](size_t size) const;
  //! Return the index of the base cell
  size_t base_cell() const;

  //! Return the index of the cell we reach after a move
  //! \param c index of the cell we start with
  //! \param s the step we perform from c
  static size_t move(size_t c, Step s);

  //! Fill the grrid with a given value
  void fill(CellData v);
};

inline Grid::Grid(size_t l) {
  // x coordinate of the base cellx
  x_base = 1 + (l - 6) /2;
}


inline CellData& Grid::operator[](size_t c) {
  return tab[c];
}

inline const CellData& Grid::operator[](size_t c) const {
  return tab[c];
}

inline size_t Grid::base_cell() const {
  return pos(x_base, y_base);
}

inline size_t Grid::pos(size_t x, size_t y) {
  return x + y *  max_width;
}

inline size_t Grid::move(size_t c, Step s) {
  switch(s) {
  case Left :
    return c - 1;
    break;
  case Right:
    return c + 1;
    break;
  case Up:
    return c + max_width;
    break;
  case Down:
    return c - max_width;
    break;
  default:
    assert(false);
    break;
  }
}

inline void Grid::fill(CellData v) {
  for (size_t i = 0; i < size; ++ i) {
    tab[i] = v;
  }
}

#endif
