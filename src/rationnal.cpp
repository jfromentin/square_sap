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

#include "rationnal.hpp"

//-----
// gcd
//-----
Int gcd(Int a, Int b) {
  Int r = a;
  Int rp = b;
  while (r != 0) {
    Int q = rp / r;
    Int t = r;
    r = rp - (q * r);
    rp = t;
  }
  return rp;
}

//------------
// operator<<
//------------
ostream& operator<<(ostream& os,const Rationnal& r){
  if (r.denominator() == 1) return os << (long int) r.numerator();
  return os << (long int)r.numerator() << '/' << (long int)r.denominator();
}
