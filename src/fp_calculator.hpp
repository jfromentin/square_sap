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

#ifndef FPCALCULATOR_HPP
#define FPCALCULATOR_HPP

#include "avx_matrix.hpp"
#include "grid.hpp"
#include "polygon.hpp"
#include "coefficients.hpp"

static const size_t max_number_vertices = 3 * max_length;

class FpCalculator{
private:
  size_t length;
  Grid grid;
  size_t nv;
  size_t ne;
  int vx[max_number_vertices];
  int vy[max_number_vertices];
  AvxMatrix B;
  AvxMatrix C;
  void explore_cell(size_t cell);
  void add_edge(size_t v, size_t w);
  void compute_graph(const Polygon& P);
  
  //void compute_matrices();
public:
  FpCalculator(size_t length);
  Reel operator()(const Polygon& P);
};

inline FpCalculator::FpCalculator(size_t l) : length(l), grid(l) {
  compute_coefficients();
}



inline void FpCalculator::add_edge(size_t v, size_t w) {
  B.get(v, w) = 1;
  B.get(w, v) = 1;
}
#endif
