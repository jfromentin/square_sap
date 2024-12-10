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
#include "fp_calculator.hpp"

Reel FpCalculator::operator()(const Polygon& P) {
  compute_graph(P);
  AvxMatrix M;
  
  M.clear();
  M.from_C_B(C, B, nv);
  Reel det = M.Gauss(nv, nv + 1);
  
  Reel res = 0;
  for (size_t i = 0; i < nv; ++ i) {
    res += B.get_diag_square_sym(i, nv) * M.get(i, nv);
  }
  res *= (det * 0.25);
  
  return res;
}

void FpCalculator::compute_graph(const Polygon& P) {
  size_t cell = grid.base_cell();
  size_t ne = 0;
  nv = 0;
  B.clear();
  C.clear();
  grid.fill(-1);
  explore_cell(cell);
  for (size_t i = 0; i < length; ++ i) {
    size_t down = grid.move(cell, Down);
    size_t left = grid.move(cell, Left);
    size_t right = grid.move(cell, Right);
    size_t up = grid.move(cell, Up);
    explore_cell(down);
    explore_cell(left);
    explore_cell(right);
    explore_cell(up);
    // Update B matrix
    size_t v = grid[cell];
    add_edge(v, grid[down]);
    add_edge(v, grid[left]);
    add_edge(v, grid[right]);
    add_edge(v, grid[up]);
    cell = grid.move(cell, P[i]);
  } 
}

inline void FpCalculator::explore_cell(size_t cell) {
  if (grid[cell] == 65535) {
    grid[cell] = nv;
    int x = cell % max_width;
    vx[nv] = x;
    int y =  cell / max_width;
    vy[nv] = y;
    ++ nv;
    for (size_t i = 0 ; i < nv ; ++ i) {
      int64_t dx = x - vx[i];
      dx = (dx < 0) ? -dx : dx;
      int64_t dy = y - vy[i];
      dy = (dy < 0) ? -dy : dy;
      Reel c = get_coeff(dx, dy);
      C.get(i, nv - 1) = c;
      C.get(nv - 1, i) = c;
    }
  }
}
