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

  Polygon P;
  P.set_length(8);
  P[0] = Right;
  P[1] = Up;
  P[2] = Left;
  P[3] = Down;
  /* P[2] = Up;
  P[3] = Left;
  P[4] = Left;
  P[5] = Down;
  P[6] = Right;
  P[7] = Down;*/
  P.display();
  cout << endl;
  //FpCalculator fp(8);
  FpCalculator fp(4);
  fp(P);
  /* int length;
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
  cout << "Input = " << input << endl;
  cout << "Output = " << output << endl;
  cout << "Length = " << length << endl;
  cout << "Number = " << number << endl;
  cout << "Frequency = " << frequency << endl;
  ReadSapFile reader(length, input, number);
  size_t temp = 0;
  for (size_t i = 0; i < number; ++ i){
    const Polygon& P = reader.read_polygon();
    //temp += (size_t)P[10];
        cout << i << " : ";
    P.display();
    cout << endl; 
  }
  cout << temp << endl;*/
}
