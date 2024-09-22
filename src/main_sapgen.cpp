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
#include "polygon_generator.hpp"
//#include "bit_buffer.hpp"
//#include "binary_file.hpp"

void generate(size_t l, string prefixs, string filename, size_t split) {
  PolygonGenerator gen(l);
  gen.exec(prefixs, filename, split);
}

int main(int argc, char** argv) {
  int length;
  int split;
  string prefixs;
  string filename;
  try {  
    TCLAP::CmdLine cmd("Produce self avoiding polygons of a given length with fixed prefix on the square latice.", ' ', "0.1");
    TCLAP::ValueArg<int> lengthArg("l", "length", "length of polygons", true, 16, "integer");
    TCLAP::ValueArg<string> prefixsArg("p", "prefix", "prefixs of self avoiding polygons seprated by _", false, "R", "string");
    TCLAP::ValueArg<string> outputArg("o", "output", "base output filename to store output", true, "polygons", "string");
    TCLAP::ValueArg<int> splitArg("s", "split", "maximal number of self avoiding polygons in a file", false, 0, "integer");
    cmd.add(lengthArg);
    cmd.add(prefixsArg);
    cmd.add(outputArg);
    cmd.add(splitArg);
    cmd.parse(argc, argv);
    length = lengthArg.getValue();
    prefixs = prefixsArg.getValue();
    filename = outputArg.getValue();
    split = splitArg.getValue();
    /*cout << "Length = " << length << endl;
    cout << "Prefixs = " << prefixs << endl;
    cout << "Filename = " << filename << endl;
    cout << "Split = " << split << endl;*/
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
  generate(length, prefixs, filename, split);
  exit(0);
}
