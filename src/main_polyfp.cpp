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

#include <iostream>
#include <tclap/CmdLine.h>
#include "read_sap_file.hpp"
#include "fp_calculator.hpp"

int main(int argc, char** argv) {

  int length;
  int frequency;
  size_t number;
  
  string input, output;
   try {  
    TCLAP::CmdLine cmd("Compute somme of Fp for multiple self avoiding polygons", ' ', "0.1");
    TCLAP::ValueArg<int> lengthArg("l", "length", "length of polygons", true, 16, "integer");
    TCLAP::ValueArg<size_t> numberArg("n", "number", "number of polygons in input file", true, 0, "integer"); 
    TCLAP::ValueArg<string> inputArg("i", "input", "file conaitinng self avoinding polygons", true, "", "string");
    TCLAP::ValueArg<string> outputArg("o", "output", "filename of output file", true, "", "string");
    TCLAP::ValueArg<size_t> frequencyArg("f", "frequency", "frequency to record polygon on the corresponding fp", true, 0, "integer");
    cmd.add(outputArg);
    cmd.add(inputArg);
    cmd.add(lengthArg);
    cmd.add(frequencyArg);
    cmd.add(numberArg);
    cmd.parse(argc, argv);
    length = lengthArg.getValue();
    frequency = frequencyArg.getValue();
    number = numberArg.getValue();
    input = inputArg.getValue();
    output = outputArg.getValue();
  }
   catch (TCLAP::ArgException &e){
    cerr <<"[Error] "<<e.error()<<" for arg "<<e.argId()<<endl;
    return -1;
  }
  if (length%2 != 0)  {
    cerr << "Length must be even." << endl;
    exit(-1);
  }
  if (length < 4 or length > 40) {
    cerr << "Length must be in [4, 40]" << endl;
  }
  //cout << "Input: " << input << endl;
  ofstream file_info(output + ".info", ios::out | ios::trunc);
  ofstream file_data(output + ".fp", ios::binary | ios::out | ios::trunc);
  file_info << std::setprecision(20);
  ReadSapFile reader(length, input, number);
  FpCalculator fp_calculator(length);
  Reel sum_fp = 0;
  for (size_t i = 0; i < number; ++ i){
    const Polygon& P = reader.read_polygon();
    //cout << i << ": ";
    //P.display();
    
    Reel fp = fp_calculator(P);
    //cout << " : " << fp << endl;
    sum_fp += fp;
    if (i % frequency == 0) {
      P.display(file_info);
      file_info << ": " << fp << endl;
    }		   			 
  }
  file_data.write((const char*)&sum_fp, sizeof(Reel));
  file_info << "Sum of fp: " << sum_fp << endl;
  Reel proba = sum_fp * 2 * length;
  for (int i = 0; i < length; ++ i) {
    proba = proba / 4;
  }
  file_data.write((const char*)&proba, sizeof(Reel));
  file_info << "Probability: " << proba << endl;
  
  
}
