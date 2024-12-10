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

#ifndef FUSION_HPP
#define FUSION_HPP

#include "array.hpp"
#include "binary_file.hpp"
#include "polygon.hpp"
#include "polygon_buffer.hpp"
#include "lzma_buffer.hpp"

class Fusion {
private:
  string input;
  string output;
  size_t length;
  ifstream data_file;
  Array<size_t> number_sap;
  PolygonBuffer<Out, LzmaBuffer<Out, BinaryFile<Out>>> polygon_output;
  BinaryFile<In> file_input;
  LzmaBuffer<In, BinaryFile<In>> lzma_input;
  PolygonBuffer<In, LzmaBuffer<In, BinaryFile<In>>> polygon_input;
  void run_on_prefix(size_t i);
  void read_data_file();
  size_t get_number_sap(string line);
  size_t n_total;
public:
  Fusion(size_t length, string input, string output);
  void run();
  
};

inline Fusion::Fusion(size_t l, string i, string o) {
  length = l;
  input = i;
  output = o;
}

  
  /*
  string line;
  for (size_t i = 0; i < 64; ++ i) {
    string filename = input + "_" + to_string(i) + ".dat";
    ifstream file(filename.c_str());
    // Read not used first 5 lines
    getline(file, line);
    getline(file, line);
    getline(file, line);
    getline(file, line);
    getline(file, line);
    int file_number = 0;
    while(file.good()) {
      getline(file, line);
      if (line.size() > 0) {
	
	cout << file_number << " : " << line << endl;
	++ file_number;
      }
    }fu
   
    

    }*/
 


#endif
