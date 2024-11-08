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

#ifndef AVX_MATRIX_HPP
#define AVX_MATRIX_HPP

#include <immintrin.h>
#include <iostream>
#include "config.hpp"

using namespace std;


//*************
//* AvxMatrix *
//*************

//! A Matrix class that take advantage of avx instructions
class AvxMatrix{
public:
  //! Maximum size of the matrix
  static const size_t rank = 3 * max_length + 1;
  
  //! Corresponding length for m256 array
  static const size_t avx_rank = (rank - 1) / 4 + 1;

  //! Real number of colums; must be a multiple of 4
  static const size_t columns = 4 * avx_rank;

  //! Total length of the m256 array
  static const size_t avx_size = avx_rank * columns;

  //! Total length of the double array
  static const size_t capacity = 16 * avx_size;

  //! Array of the matrix coefficients
  union{
    __m256d avx[avx_size];
    Reel data[capacity];
  };

  //! The empty constructor
  AvxMatrix();

  //! Coefficient accessor
  //! \param i line position of the coefficient
  //! \param j column position of the coefficient
  Reel get(size_t i, size_t j) const;
  Reel& get(size_t i, size_t j);

  //! Fill matrix with 0
  void clear();
  
  //! Display command
  //! \param nl number of lines to display
  //! \param nc number of columns to display
  void display(size_t nl, size_t nc) const;

  //! Assume the current matrix M is symmetric and
  //! return the ith diagonal term of M square
  Reel get_diag_square_sym(size_t i, size_t n) const;

  //! Set the current Matrix to be I + 1/4.C.B where B
  //! is supposed to be symmetric
  //! \param C a matrix of size n x n
  //! \param B a symmetric matrix of size n x n
  //! \param n size of matrices B and C
  void from_C_B(const AvxMatrix& C, const AvxMatrix& B, size_t n);

  //! Swap lines i and j of the matrix
  //! \param i line number
  //! \param j line number (differenet from i)
  //! \param navx number of m256 blocs to consier per lines
  void swap_lines(size_t i, size_t j, size_t navx);

  //! Multiply line i by a
  //! \param i line number
  //! \param a non zero scalar
  //! \param navx number of m256 blocs to consider per lines 
  void mul_line(size_t i, Reel a, size_t navx);

  //! Add to line i a multiple by a of line j
  //! \param i line number
  //! \param j line number
  //! \param a non zero scalar
  //! \param navx number of m256 blocs to consider per lines
  void add_mul_line(size_t i, size_t j, Reel a, size_t navx);

  //! Perform Gauss reduction on a top-left submatrix
  //! Return the corresponding minor
  //! \param nl number of lines of the submatrix
  //! \param nc number of columns of the submatrix
  Reel Gauss(size_t nl, size_t nc);
};

//*****************
//* AVX constants *
//*****************

//! Array of 4 zeros
static const __m256d zeros = _mm256_set1_pd(0);

//*************
//* Avx block * 
//*************

//! Block structure mixing representing m256 as array of 4 doubles
union  Block{
  __m256d avx;
  Reel data[4];
};

//******************
//* Inline methods *
//******************

inline
AvxMatrix::AvxMatrix(){
}

inline Reel&
AvxMatrix::get(size_t i, size_t j){
  return data[i * columns + j];
}

inline Reel
AvxMatrix::get(size_t i, size_t j) const{
  return data[i * columns + j];
}

#endif
