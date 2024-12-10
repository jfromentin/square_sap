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

#include "lzma_buffer.hpp"

string Lzma::message(lzma_ret error) const {
  // possible errors are documented in
  // lzma/container.h (src/liblzma/api/lzma/container.h in the source
  // package or e.g. /usr/include/lzma/container.h depending on the
  // install prefix).
  switch (error) {
  case LZMA_OK:
    return "No error";
    break;
  case LZMA_STREAM_END:
    return "Reach end of stream";
    break;
  case LZMA_NO_CHECK:
    return "Input stream has no integgrity check";
    break;
  case LZMA_UNSUPPORTED_CHECK:
    return "Cannot compute integrity check";
    break;
  case LZMA_GET_CHECK:
    return "Integrity check type is now available";
    break;
  case LZMA_MEM_ERROR:
    return "Memory allocation failed";
    break;
  case LZMA_MEMLIMIT_ERROR:
    return "Memory usage limit was reached";
    break;
  case LZMA_FORMAT_ERROR:
    return "File format not recognized";
    break;
  case LZMA_OPTIONS_ERROR:
    return "Specified preset is not supported";
    break;
  case LZMA_DATA_ERROR:
    return "Data is corrupt";
    break;
  case LZMA_BUF_ERROR:
    return "No progress is possible";
    break;
  case LZMA_PROG_ERROR:
    return "Programming error";
    break;
    /*  case LZMA_SEEK_NEEDED:
    return "Request to change the input file position";
    break;*/
  default:
    return  "Unknown error";
    break;
  }
}
