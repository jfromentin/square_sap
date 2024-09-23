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

#ifndef LZMA_BUFFER_HPP
#define LZMA_BUFFER_HPP

#include "buffer.hpp"
#include <lzma.h>

using namespace std;

template<IO D, class T> class LzmaBuffer;

class Lzma{
protected:
  //! Buffer capacity
  static const size_t capacity = 65536;
  //! Stream used by LZMA algoritm
  lzma_stream strm;
  //! Return error message associated to LZMA return value
  string message(lzma_ret ret) const;
};

template<class T> class LzmaBuffer<Out, T>:Lzma{
private:
  //! Output buffer
  Byte output_buffer[capacity];
  //! Output where flush buffer
  T* output;
public:
  //! Empty constructor
  LzmaBuffer();
  //! Open the LZMA defalte buffer to flush in output object
  void open(T* output);
  //! Write data to deflate in the LZMA buffer
  void write(Byte* buffer, size_t size);
  //! Close the buffer
  void close();
  //! Clear all internal data
  void clear();
};

template<class T> class LzmaBuffer<In, T>:Lzma{
private:
  // Input buffer
  Byte input_buffer[capacity];
  // Object where read data
  T* input;
  // LZMA action use by the LZMA algorithm
  lzma_action action = LZMA_RUN;
public:
  //! Empty constructor
  LzmaBuffer();
  //! Open LZMA inflate buffer to read from input object
  void open(T* input);
  // Read inflated data from input object
  size_t read(Byte* buffer, size_t size);
  // Close the buffer
  void close();
  //! Clear all internal data
  void clear();
};

template<class T> LzmaBuffer<Out, T>::LzmaBuffer() {
}

template<class T> void LzmaBuffer<Out, T>::open(T* o) {
  output = o;
  // Initialising the internal LZMA encoder structure 
  strm = LZMA_STREAM_INIT;
  clear();
}

template<class T> void LzmaBuffer<Out,T>::clear() {
  output -> clear();
  // Initialising the internal LZMA encoder structure using default preset and 64 bit checksum
  lzma_ret ret = lzma_easy_encoder(&strm, LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC64);
  // Set output buffer for inflated data
  strm.next_out = output_buffer;
  strm.avail_out = capacity;
  // Check if an error occured
  if (ret != LZMA_OK) {
    cerr << "[LZMA error] " << message(ret) << endl;
  }
}
template<class T> void LzmaBuffer<Out, T>::write(Byte* buffer, size_t size) {
  // We assume that size is less than the input buffer capacity
  assert(size <= capacity);
  // Run LZMA algorithm until we reach the end of the input buffer
  lzma_action action = LZMA_RUN;
  // Set buffer as input buffer (inflate) of lzma algorithm
  strm.next_in = buffer;
  strm.avail_in = size;
  // Loop until input buffer is empty
  while (strm.avail_in) {
    // Call lzma defalte algorithm
    lzma_ret ret = lzma_code(&strm, action);
    // Test if an error occured
    if (ret != LZMA_OK) {
      cerr << "[LZMA error] " << message(ret) << endl;
      exit(0);
    }   
    // Test if the ouput buffer is full
    if (strm.avail_out == 0) {
      // If it the case, flush the output buffer in output object
      output -> write(output_buffer, capacity);
      // Reset the output buffer
      strm.next_out = output_buffer;
      strm.avail_out = capacity;
    }
  }
}

template<class T> void LzmaBuffer<Out, T>::close() {
  assert(strm.avail_in == 0);
  // Write the remaining data
  output -> write(output_buffer, capacity - strm.avail_out);
  strm.next_out = output_buffer;
  strm.avail_out = capacity;
  lzma_ret ret;
  do {
    ret = lzma_code(&strm, LZMA_FINISH);
    if (ret != LZMA_OK and ret != LZMA_STREAM_END) {
      cerr << "[LZMA error] " << message(ret) << endl;
    }
    output -> write(output_buffer, capacity - strm.avail_out);
  } while(ret != LZMA_STREAM_END);
  output -> close();
}

template<class T> LzmaBuffer<In, T>::LzmaBuffer(){
}

template<class T> void LzmaBuffer<In, T>::open(T* i) {
  input = i;
  strm = LZMA_STREAM_INIT;
  clear();
}

template<class T> void LzmaBuffer<In, T>::clear() {
  input -> clear();
  // Initialising the interal LZMA decoder
  lzma_ret ret = lzma_stream_decoder(&strm, UINT64_MAX, LZMA_CONCATENATED);
  // Check if an error occured
  if (ret != LZMA_OK) cerr << "[LZMA error] " << message(ret) << endl;
  // Set action to run LZMA alogrithm until end of input buffer is reached
  action = LZMA_RUN;
}
						      
template<class T> size_t LzmaBuffer<In, T>::read(Byte* buffer, size_t size) {
  // Set output buffer
  strm.next_out = buffer;
  strm.avail_out = size;
  // Loop until we can add data to output buffer
  while(strm.avail_out > 0) {
    // Test is there is available data in input buffer
    if(strm.avail_in == 0) {
      // If no, fill the input buffer
      strm.next_in = input_buffer;
      strm.avail_in = input->read(input_buffer, capacity);
      // Test is there is now available data in
      if(strm.avail_in == 0) {
	// If no, tell LZMA algorithm that we want to finish
	action = LZMA_FINISH;
      }
    }
    // Call the LZMA algoritm
    lzma_ret ret = lzma_code(&strm, action);
    // Check if an error occured
    if (ret == LZMA_STREAM_END ) {
      // Enf of the input stream reached, quit the loop
      break;
    }
    if (ret == LZMA_BUF_ERROR) {
      break;
    }
    
    // Test if an error occured
    if (ret != LZMA_OK ) {//and ret != LZMA_BUF_ERROR) {
      cerr << "[LZMA error] " << message(ret) << endl;
    }
  }
  // Compute the number of bytes actually added in output uffer
  size_t s =  size - strm.avail_out;
  // Return this value
  return s;
}

template<class T> void LzmaBuffer<In, T>::close() {
  input->close();
}

#endif

