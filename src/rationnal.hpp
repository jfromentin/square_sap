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

#ifndef RATIONNAL_HPP
#define RATIONNAL_HPP

#include <iostream>
#include <cstdint>
#include <iostream>
#include "config.hpp"

using namespace std;

//*************
//* Rationnal *
//*************

//! Class for representing rationnals.
class Rationnal{
private:
  //! Numerator
  Int num;
  //! Denominator
  Int den;
  //! Normalise th fraction
  void normalize();
public:
  //! Construct the null rationnal
  Rationnal();
  
  //! Construct the rationnal n
  //! \param n integer
  Rationnal(Int n);

  //! Construct the rationnal n/d
  //! \param n numerator integer
  //! \param d denominator integer
  Rationnal(Int n, Int d);

  //! Copy constructor
  //! \param r rationnal to copy
  Rationnal(const Rationnal& r);

  //! Copy operator
  //! \param r rationnal to copy
  Rationnal& operator=(const Rationnal& r);

  //! Numerator accessor
  Int numerator() const;

  //! Denominator accessor
  Int denominator() const;

  //! Return an approximation of the rationnal
  explicit operator Reel() const;

  //! Addition operators 
  Rationnal operator+(const Rationnal& r) const;
  Rationnal operator+(Int n) const;
  friend Rationnal operator+(Int n, const Rationnal& r);

  //! Substration operators
  Rationnal operator-(const Rationnal& r) const;
  Rationnal operator-(Int n) const;
  friend Rationnal operator-(Int n, const Rationnal& r);

  //! Multiplication operators
  Rationnal operator*(const Rationnal& r) const;
  Rationnal operator*(Int n) const;
  friend Rationnal operator*(Int n, const Rationnal& r);

  //! Division operators
  Rationnal operator/(const Rationnal& r) const;
  Rationnal operator/(Int n) const;
  friend Rationnal operator/(Int n, const Rationnal& r);
};

//-----------------
// Other functions
//-----------------
ostream& operator<<(ostream& os, const Rationnal& r);

Int gcd(Int a, Int b);

//********************
//* Inline functions *
//********************

inline
Rationnal::Rationnal(): num(0), den(1) {
}

inline
Rationnal::Rationnal(Int n): num(n), den(1) {
}

inline
Rationnal::Rationnal(Int n, Int d): num(n), den(d) {
  normalize();
}

inline
Rationnal::Rationnal(const Rationnal& r): num(r.num), den(r.den) {
}

inline Rationnal&
Rationnal::operator=(const Rationnal& r) {
  num = r.num;
  den = r.den;
  return *this;
}
  
inline Int
Rationnal::numerator() const {
  return num;
}

inline Int
Rationnal::denominator() const {
  return den;
}

inline 
Rationnal::operator Reel() const {
  Reel n = num;
  Reel d = den;
  return n / d;
}

inline void
Rationnal::normalize() {
  Int d = gcd((num < 0) ? -num : num, den);
  num /= d;
  den /= d;
}

inline Rationnal
Rationnal::operator+(const Rationnal& r) const {
  return Rationnal(num * r.den + r.num * den, den * r.den);
}

inline Rationnal
Rationnal::operator+(Int n) const {
  return Rationnal(num + n * den, den);
}

inline Rationnal
Rationnal::operator-(const Rationnal& r) const {
  return Rationnal(num * r.den - r.num * den, den * r.den);
}

inline Rationnal
Rationnal::operator-(Int n) const {
  return Rationnal(num - n * den, den);
}

inline Rationnal
Rationnal::operator*(const Rationnal& r) const {
  return Rationnal(num * r.num, den * r.den);
}

inline Rationnal
Rationnal::operator*(Int n) const {
  return Rationnal(num * n, den);
}

inline Rationnal
Rationnal::operator/(const Rationnal& r) const{
  Int s = (r.num < 0) ? -1 : 1;
  return Rationnal(s * num * r.den, s * den * r.num);
}

inline Rationnal
Rationnal::operator/(Int n) const {
  Int s = (n < 0) ? -1 : 1;
  return Rationnal(s * num, s * den * n);
}

inline Rationnal
operator+(Int n, const Rationnal& r) {
  return Rationnal(n * r.den + r.num, r.den);
}

inline Rationnal
operator-(Int n, const Rationnal& r) {
  return Rationnal(-n * r.den + r.num, r.den);
}

inline Rationnal
operator*(Int n, const Rationnal& r) {
  return Rationnal(n * r.num, r.den);
}

inline Rationnal
operator/(Int n, const Rationnal& r) {
  Int s = (n < 0) ? -1 : 1;
  return Rationnal(s * r.num, s * r.den * n);
}

#endif

