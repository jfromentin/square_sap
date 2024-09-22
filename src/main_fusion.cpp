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
#include "fusion.hpp"


int main(int argc, char** argv) {
  int length;
  string input, output;
  try {  
    TCLAP::CmdLine cmd("Fusion self avoiding polygons from multiple files in a unique file", ' ', "0.1");    TCLAP::ValueArg<int> lengthArg("l", "length", "length of polygons", true, 16, "integer");
    TCLAP::ValueArg<string> inputArg("i", "input", "prefix of .dat filenames to fusion (ex : sap, for dap_0.sap, ...)", true, "", "string");
    TCLAP::ValueArg<string> outputArg("o", "output", "filename of the unique output file", true, "", "string");

    cmd.add(outputArg);
    cmd.add(inputArg);
    cmd.add(lengthArg);
    cmd.parse(argc, argv);
    length = lengthArg.getValue();
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
  if (length < 2 or length > 40) {
    cerr << "Length must be in [2, 40]" << endl;
  }
  Fusion fusion(length, input, output);
  fusion.run();
  exit(0);
}
