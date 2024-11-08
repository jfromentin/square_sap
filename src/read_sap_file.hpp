#ifndef READ_SAP_FILE_HPP
#define READ_SAP_FILE_HPP

#include "array.hpp"
#include "binary_file.hpp"
#include "polygon.hpp"
#include "polygon_buffer.hpp"
#include "lzma_buffer.hpp"

class ReadSapFile{
private:
  string filename;
  size_t length;
  size_t number;
  size_t current;
  Polygon P;
  BinaryFile<In> file_input;
  LzmaBuffer<In, BinaryFile<In>> lzma_input;
  PolygonBuffer<In, LzmaBuffer<In, BinaryFile<In>>> polygon_input;
public:
  ReadSapFile(size_t length, string input, size_t number);
  const Polygon& read_polygon();
  ~ReadSapFile();
};

inline ReadSapFile::ReadSapFile(size_t l, string i, size_t n) {
  length = l;
  filename = i;
  number = n;
  current = 0;
  file_input.open(filename);
  lzma_input.open(&file_input);
  polygon_input.open(&lzma_input, length);
}

inline const Polygon& ReadSapFile::read_polygon() {
  assert (current < number);
  polygon_input.read(P);
  ++ current;
  return P;
}


inline ReadSapFile::~ReadSapFile() {
  polygon_input.close();
}
#endif
