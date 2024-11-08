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

#ifndef COEFFICIENTS_HPP
#define COEFFICIENTS_HPP

#include "config.hpp"
#include "rationnal.hpp"

//**********************
//* Coefficients C_m,n *
//**********************

//! Maximal coefficient index
static const size_t max_index_coefficients = max_length / 2 + 2;

//! Number of coefficients
static const size_t number_coefficients = ((max_index_coefficients + 1) * (max_index_coefficients + 2)) / 2;

//! Array of coefficients
extern Reel coefficients[number_coefficients];

//! Return the indice of coefficient C_{i,j}
size_t pos(size_t i, size_t j);

//! Return an approximation of coefficient C_{i,j}
Reel get_coefficient(size_t i, size_t j);

//! Compute all the coefficients
void compute_coefficients();

//! Represent an analytic coefficient
struct CoefficentAnalytic{
  //! The coefficient is a-4b/pi
  Rationnal a,b;
};

//********************
//* Inline functions *
//********************

inline size_t
pos(size_t i, size_t j){
  return (j * (j + 1)) / 2 + i;
}

inline Reel
get_coeff(size_t i, size_t j){
  return i < j ? coeffs[pos(i, j)] : coeffs[pos(j, i)];
}

#endif
