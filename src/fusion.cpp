#include "fusion.hpp"

void Fusion::run() {
  n_total = 0;
  // Configure input
  lzma_input.open(&file_input);
  polygon_input.open(&lzma_input, length);
  // Configure output
  BinaryFile<Out> file_output;
  file_output.open(output + ".sap");
  LzmaBuffer<Out, BinaryFile<Out>> lzma_output;
  lzma_output.open(&file_output);
  polygon_output.open(&lzma_output, length);
  // Run on each prefix 
  for (size_t i = 0; i < 64; ++ i ) {
    run_on_prefix(i);
    cout << i + 1 << " of 64 : " << n_total << endl;
  }
  // Close ouput
  polygon_output.close();
}

void Fusion::run_on_prefix(size_t i) {
  string filename = input + "_" + to_string(i) + ".dat";
  data_file.open(filename.c_str());
  read_data_file();
  data_file.close();
  for (size_t j = 0; j < number_sap.size(); ++ j) {
    filename = input + "_" + to_string(i) + (j == 0 ? "" : "_" + to_string(j)) + ".sap";
    polygon_input.clear();
    file_input.open(filename);
    Polygon P;
    n_total += number_sap[j];
    for (size_t k = 0; k < number_sap[j]; ++ k) {
      polygon_input.read(P);
      polygon_output.write(P);
    }
    file_input.close();
  }
}

void Fusion::read_data_file() {
  string line;
  getline(data_file, line);
  getline(data_file, line);
  getline(data_file, line);
  getline(data_file, line);
  getline(data_file, line);
  list<size_t> temp;
  while(data_file.good()) {
    getline(data_file, line);
    if (line.size() > 0) {
      size_t n = get_number_sap(line);
      temp.push_back(n);
    }
  }
  number_sap = temp;
}

size_t Fusion::get_number_sap(string line) {
  string str = "";
  int step = 0;
  for (size_t i = 0; i < line.size(); ++ i) {
    char c = line[i];
    if (c == '\t') {
      if (step == 1) {
	return  atoi(str.c_str());
      }
      ++ step;
      str = "";
    }
    else {
      str += c;
    }
  }
  assert(false);
  return 0;
}
