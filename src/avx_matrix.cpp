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

#include "avx_matrix.hpp"

//------------------
// AvxMatrix::clear
//------------------

void
AvxMatrix::clear(){
  for(size_t i = 0; i < avx_size; ++ i) avx[i] = zeros;
}

//--------------------
// AvxMatrix::display
//--------------------

void
AvxMatrix::display(size_t nl, size_t nc) const{
  for(size_t i = 0; i < nl; ++ i){
    for(size_t j = 0; j < nc; ++ j){
      cout << get(i, j) << '\t';
    }
    cout << endl;
  }
}

//--------------------------------
// AvxMatrix::get_diag_square_sym
//--------------------------------

Reel
AvxMatrix::get_diag_square_sym(size_t i, size_t n) const{
  size_t n_avx = (n - 1) / 4 + 1;
  Block b;
  b.avx=zeros;
  for(size_t k = 0; k < n_avx; ++ k){
    __m256d a = avx[i * avx_rank + k];
    b.avx = _mm256_fmadd_pd(a, a, b.avx);
  }
  return b.data[0] + b.data[1] + b.data[2] + b.data[3];
}

//---------------------
// AvxMatrix::from_C_B
//---------------------

void
AvxMatrix::from_C_B(const AvxMatrix& C, const AvxMatrix& B, size_t n){
  size_t n_avx = (n - 1) / 4 + 1;
  Block c;
  for(size_t i = 0; i < n; ++ i){
    for(size_t j = 0; j < n; ++ j){
      c.avx = zeros;
      for(size_t k = 0; k < n_avx; ++ k){
	__m256d a = C.avx[i * avx_rank + k];
	__m256d b = B.avx[j * avx_rank + k];
	c.avx = _mm256_fmadd_pd(a, b, c.avx);
      }
      get(i, j) = (0.25) * (c.data[0] + c.data[1] + c.data[2] + c.data[3]);
    }
    get(i, i) += 1;
    get(i, n) = 1;
  }
}

//-----------------------
// AvxMatrix::swap_lines
//-----------------------

void
AvxMatrix::swap_lines(size_t i, size_t j, size_t navx){
  size_t ind_i = i * avx_rank;
  size_t ind_j = j * avx_rank;
  for(size_t k = 0; k < navx; ++ k){
    __m256d a = avx[ind_i];
    avx[ind_i] = avx[ind_j];
    avx[ind_j] = a;
    ++ ind_i;
    ++ ind_j;
  }
}

//---------------------
// AvxMatrix::mul_line
//---------------------

void
AvxMatrix::mul_line(size_t i, Reel a, size_t navx){
  __m256d b = _mm256_set1_pd(a);
  size_t ind_i = i * avx_rank;
  for(size_t k = 0; k < navx; ++ k){
    avx[ind_i] = _mm256_mul_pd(avx[ind_i], b);
    ++ ind_i;
  }
}

//-------------------------
// AvxMatrix::add_mul_line
//-------------------------

void
AvxMatrix::add_mul_line(size_t i, size_t j, Reel a, size_t navx){
  __m256d b = _mm256_set1_pd(a);
  size_t ind_i = i * avx_rank;
  size_t ind_j = j * avx_rank;
  for(size_t k = 0; k < navx; ++ k){
    avx[ind_i] = _mm256_fmadd_pd(avx[ind_j], b, avx[ind_i]);
    ++ ind_i;
    ++ ind_j;
  }
  
}

//------------------
// AvxMatrix::Gauss
//------------------

Reel
AvxMatrix::Gauss(size_t nl, size_t nc){
  size_t navx = (nc - 1) / 4 + 1;
  Reel det = 1;
  size_t np = 0; //np=0
  for(size_t j = 0; j < nc; ++ j){
    for(size_t p = np; p < nl; ++ p){
      Reel c = get(p, j);
      if(c != 0){
	det *= c;
	mul_line(p, 1.0 / c, navx);
	for(size_t k = 0; k < nl; ++ k){
	  if(k != p){
	    add_mul_line(k, p, -get(k,j), navx);
	  }
	}
	if(p != np){
	  swap_lines(np, p, navx);
	  det *= -1;
	}
	++ np;
	break;
      } 
    }
  }
  return det;
}
