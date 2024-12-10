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

#include "gameboard.hpp"

GameBoard::GameBoard(size_t l) {
  length = l;
  // Width of the gameboard
  width = l - 1;
  // Height of the gameboard
  height = l / 2 + 2;
  // x coordinate of the base cellx
  x_base = 1 + (l - 6) /2;
  // Initialise CellData of each cell of the gameboard
  for (size_t y = 0; y < height; ++ y) {
    for (size_t x = 0; x < width; ++ x) {
      // Test if the cell is on the border
      if (on_border(x, y)) {
	// If true, init cell data with t and d equal to 127 (~ +infty)
	// REM we don't use 255 since we want add this value to another one and
	// in this case the value of 255 raise to an overflo. 
	tab[pos(x, y)] = init_cell_data(127, 127);
      }
      else {
	// If not, compute Manhattan distance of the cell to the base cell
	int dx = (x < x_base) ? x_base - x : x - x_base;
	uint8_t sd = dx + (y - 1);
	// Init the cell with this distance and t = 127 (~ +infty)
	tab[pos(x, y)] = init_cell_data(sd, 127);
      }
    }
  }
}

bool GameBoard::on_border(size_t x, size_t y) {
  if (x == 0 or x == width - 1 or y == 0 or y == height - 1) return true;
  if (y == 1 and x < x_base) return true;
  return false;
}
