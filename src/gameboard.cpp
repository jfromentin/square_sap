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

#include "gameboard.hpp"

GameBoard::GameBoard(size_t l) {
  length = l;
  width = l - 1;
  height = l / 2 + 2;
  x_base = 1 + (l - 6) /2;
  //y_base = 1;
  for (size_t y = 0; y < height; ++ y) {
    for (size_t x = 0; x < width; ++ x) {
      if (on_border(x, y)) {
	tab[pos(x, y)] = init_cell_data(127, 127);
      }
      else {
	int dx = (x < x_base) ? x_base - x : x - x_base;
	uint8_t sd = dx + (y - 1);
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
