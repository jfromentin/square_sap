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

#ifndef BIT_BUFFER_HPP
#define BIT_BUFFER_HPP

#include "buffer.hpp"

//! Class for Input/Output BitBuffer. This class is designed to handle
//! stream of bits.
template<IO D, class T> class BitBuffer;

//! Class for output BitBuffer. It take bits as input and write Bytes
//! in the specified ouput object.
template<class T> class BitBuffer<Out, T> {
private:
  //! Capacity of the internal buffer
  static const size_t capacity = 65536;
  //! The internal buffer
  Byte buffer[capacity];
  //! Actual size of the buffer
  size_t size;
  //! Current byte in construction
  Byte current_byte;
  //! Head is a power 2^i where i is the next bit to set in current_byte
  Byte head;
  //! Output where we flush the internal buffer
  T* output;
  //! Add the current byte to the internal buffer and flush the buffer if needed
  void write_byte();
public:
  //! Empty constructor
  BitBuffer();
  //! Open a BitBuffer<Out> with given output object
  void open(T* output);
  //! Write a single bit
  //! \param b is the bit to write
  void write_bit(bool b);
  //! Write bits from a given integer
  //! \param v an integer
  //! \param nb number of first bits of v to write
  void write_int(size_t v, size_t nb);

  void clear();
  // Close the BitBuffer
  void close();
};

//! Class for input BitBuffer. It can read bits from an input object containing
//! a stream of Bytes.
template<class T> class BitBuffer<In, T> {
private:
  //! Capacity of the internal buffer
  static const size_t capacity = 65536;
  //! The internal buffer
  Byte buffer[capacity];
  //! Actual size of the buffer
  size_t size;
  //! Position of byte we read in the buffer
  size_t pos;
  //! Byte we are actually reading
  Byte current_byte;
  //! Head is a power 2^i where i is the next bit to read in current_byte
  Byte head;
  //! Input from where we read byte
  T* input;
  //! Read bytes from the input object
  void read_bytes();
public:
  //! Empty constructo
  BitBuffer();
  //! Open a BitBuffer<In> with given input
  void open(T* input);
  //! Read a single bit
  bool read_bit();
  //! Read an integer from bits
  //! \param nb number of bits to read
  //! \return the integer whose binary decompositio corresponds of read bits
  size_t read_int(size_t nb);

  void clear();
  
  //! Close the BitBuffer
  void close();
};

template<class T> inline BitBuffer<Out, T>::BitBuffer(){
  // The internal buffer is empty
  size = 0;
  // Current byte is initialised to 0
  current_byte  = 0;
  // The next bit to set is this as position, and so head = 2^0 = 1
  head = 1;
}

template<class T> inline void BitBuffer<Out, T>::open(T* o) {
  output = o;
}

template<class T> inline void BitBuffer<Out, T>::write_bit(bool b) {
  // We add the value of head to current_byte if b is true
  if(b) current_byte |= head;
  // We multiply the head by 2, to design the next bit positoin
  head <<= 1;
  // If head = 0 = 2^8, we exceed the capacity of a byte and we call add_byte
  if (head == 0) write_byte();
}

template<class T> inline void BitBuffer<Out, T>::write_byte() {
  // Write Byte v in the next available place in buffer
  buffer[size ++] = current_byte;
  // Initialise v and head
  current_byte = 0;
  head = 1;
  // If the capacity of buffer is reached, we write it to output
  if (size == capacity) {

    // Flush current buffer to output object
    output->write(buffer, size);
    size = 0;
  }
}

template<class T> inline void BitBuffer<Out, T>::write_int(size_t v, size_t nb) {
  // Write nb bits accordinglyto the value of v
  for (size_t i = 0; i< nb; ++ i) {
    write_bit(v & (1 << i));
  }
}

template<class T> inline void BitBuffer<Out, T>::clear() {
    // The internal buffer is empty
  size = 0;
  // Current byte is initialised to 0
  current_byte  = 0;
  // The next bit to set is this as position, and so head = 2^0 = 1
  head = 1;
  output -> clear();
}

template<class T> inline void BitBuffer<Out, T>::close() {
  // If head is not equal to 1, we must add bit '0' to complete the current byte
  while (head != 1) {
    write_bit(false);
  }
  // If s is equal to capcity, wryte_byte writes the buffer
  // Otherwise, we do it now
  if (size < capacity) {
    output->write(buffer, size);
  }
   // Close the output object
  output->close();
}

template<class T> inline BitBuffer<In, T>::BitBuffer(){
  // Internal buffer is empty
  size = 0;
  pos = 0;
  // Head is set to be 0 to force the reading of bytes from the input
  head = 0;
}

template<class T> inline void BitBuffer<In, T>::open(T* i) {
  input = i;
}

template<class T> inline bool BitBuffer<In, T>::read_bit() {
  // If we can read nothing more in current_byte, head is 0 = 2^8
  if (head == 0) {
    // We want rad the next byte in the internal buffer
    ++ pos;
    // If it is not  possible
    if (pos >= size) {
      // Fill the internal buffer with at most capacity bytes from the input
      // size if then set to number of bytes actually read and pos is set to 0
      read_bytes();
      // Check we have read something
      assert(size != 0);
    }
    // Set current_byte to the new pointed byte in the internal buffer
    current_byte = buffer[pos];
    // Next bit to read is at position 0 and we set haed to 2^0 = 1
    head = 1;
  }
  // Read the value of the bit given by head
  bool b = (head & current_byte);
  // Shift the head (multiply it by 2)
  head <<= 1;
  // Return the read bit
  return b;
}

template<class T> inline void BitBuffer<In, T>::read_bytes() {
  // Try to fill the internal buffer with capacity bytes read from the input
  // the actual number of bytes read is stored in size
  size = input->read(buffer, capacity);
  // We point to the first byte of the internal buffer
  pos = 0;
}

template<class T> inline size_t BitBuffer<In, T>::read_int(size_t nb) {
  size_t res = 0;
  for (size_t i = 0; i < nb; ++ i) {
    if (read_bit()) res += (1 << i); 
  }
  return res;
}

template<class T> inline void BitBuffer<In, T>::clear() {
  // Internal buffer is empty
  size = 0;
  pos = 0;
  // Head is set to be 0 to force the reading of bytes from the input
  head = 0;
  
  input -> clear();
}

template<class T> inline void BitBuffer<In, T>::close() {
  input->close();
}

#endif
