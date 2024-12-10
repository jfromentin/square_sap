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

#include "coefficients.hpp"

//*****************************
//* Defintion of externs data *
//****************************

Reel coefficients[number_coefficients];

//******************
//* compute_coeffs *
//******************

void compute_coefficients(){
  CoefficientAnalytic ca[number_coefficients];
  ca[pos(0, 0)] = {0, 0};  //C_(0, 0) = 0
  ca[pos(0, 1)] = {-1, 0}; //C_(1, 0) = -1
  //Compute C_(i, i)
  Rationnal r = 0;
  for (size_t i = 1; i <= max_index_coefficients; ++ i) {
    Rationnal t(1, 2 * i - 1);
    r = r + t;
    ca[pos(i, i)] = {0, -4 * r};
  }
  for (size_t j = 2; j <= max_index_coefficients; ++ j) {
    // C(0, j) = 4 * C(0, j - 1) - C(0, j - 2) - 2 * C(1, j - 1)
    ca[pos(0, j)].a = 4 * ca[pos(0, j - 1)].a - ca[pos(0, j - 2)].a - 2 * ca[pos(1, j - 1)].a;
    ca[pos(0, j)].b = 4 * ca[pos(0, j - 1)].b - ca[pos(0, j - 2)].b - 2 * ca[pos(1, j - 1)].b;
    for (size_t i = 1; i <= j - 2; ++ i) {
      // C(i, j) = 4 * C(i, j - 1) - C(i, j - 2) - C(i - 1, j - 1) - C(i + 1, j - 1)
      ca[pos(i, j)].a = 4 * ca[pos(i, j - 1)].a - ca[pos(i, j - 2)].a - ca[pos(i - 1, j - 1)].a - ca[pos(i + 1, j - 1)].a;
      ca[pos(i, j)].b = 4 * ca[pos(i, j - 1)].b - ca[pos(i, j - 2)].b - ca[pos(i - 1, j - 1)].b - ca[pos(i + 1, j - 1)].b;
    }
    // C(j - 1, j) = 2 * C(j - 1, j - 1) - C(j - 2, j - 1)
    ca[pos(j - 1, j)].a = 2 * ca[pos(j - 1, j - 1)].a - ca[pos(j - 2, j - 1)].a;
    ca[pos(j - 1, j)].b = 2 * ca[pos(j - 1, j - 1)].b - ca[pos(j - 2, j - 1)].b;
  }
  Rationnal fpii(1725033, 5419351);
  Reel epii = 2.275957200481571e-15; //epii=1/pi-1/fpii
  for (size_t j = 0; j <= max_index_coefficients; ++ j) {
    for (size_t i = 0; i <= j; ++ i) {
      size_t ind = pos(i, j);
      Rationnal f = ca[ind].a + fpii * ca[ind].b;
      coefficients[pos(i, j)] = (Reel)f + (Reel)ca[ind].b * epii;
    }
  }
}
