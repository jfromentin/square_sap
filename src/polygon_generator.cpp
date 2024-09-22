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

#include "polygon_generator.hpp"

Array<string> PolygonGenerator::split_prefixs(string str) {
  list<string> res;
  string substr = "";
  for (size_t i = 0; i < str.size(); ++ i) {
    if (str[i] == '_') {
      res.push_back(substr);
      substr = "";
    }
    else {
      substr += str[i];
    }
    
  }
  res.push_back(substr);
  return res;
}

PolygonGenerator::PolygonGenerator(size_t l): gameboard(l), stack(3 * l) {
  length = l;
  word.set_length(length);
}

void PolygonGenerator::init(string prefix) {
  assert(stack.is_empty());
  size_t c = gameboard.base_cell();
  size_t k;
  for (k = 0; k < prefix.size() - 1; ++ k) {
    Step s = char_to_step(prefix[k]);
    apply(c, k, s);
    c = gameboard.move(c, s);
  }
  current_stack_info = new StackInfo;
  StackInfo* stack_info = stack.next();
  *stack_info = init_stack_info(c, k , char_to_step(prefix[k]));
  stack.push();
}

void PolygonGenerator::init_output() {
  output_data = ofstream(filename + ".dat", ios::trunc);
  output_data << "length " << length << endl;
  output_data << "prefixs " << prefixs << endl;
  output_data << "filename " << filename << endl;
  output_data << "split " << split << endl;
  output_data << "----------------------------" << endl;
  output_file.open(filename + ".sap");
  lzma_output.open(&output_file);
  polygon_buffer.open(&lzma_output, length);
  file_number = 0;
  file_size = 0;
  sap_number = 0;
}

void PolygonGenerator::close_output() {
  sap_number += file_size;
  output_data << file_number << '\t' << file_size << '\t' << sap_number << endl;
  output_data.close();
  polygon_buffer.close();
}

void PolygonGenerator::write_sap() {
  polygon_buffer.write(word);
  ++ file_size;
  if (split !=0 and (file_size % split) == 0) {
    polygon_buffer.close();
    ++ file_number;
    output_file.open(filename + '_' + to_string(file_number) + ".sap");
    lzma_output.clear();
    sap_number += file_size;
    output_data << file_number << '\t' << file_size << '\t' << sap_number << endl;
    file_size = 0;
  }
}

void PolygonGenerator::exec_prefix(string prefix) {
  init(prefix);
  while(not stack.is_empty()) {
    stack.pop(&current_stack_info);
    StackInfo cur = *current_stack_info;
    uint8_t k = get_k(cur);
    if(k == length - 2) {
      word[k] = get_s(cur);
      write_sap();

    }
    else {
      uint16_t c = get_c(cur);
      Step s = get_s(cur);
      apply(c, k , s);
      size_t cn;
      ++ k;
      switch(s) {
      case Right:
	cn = c + 1;
	try_add(cn, cn + 1, k, Right);
	try_add(cn, cn + max_width, k, Up);
	try_add(cn, cn - max_width, k, Down);
	break;
      case Left:
	cn = c - 1;
	try_add(cn, cn - 1, k, Left);
	try_add(cn, cn + max_width, k, Up);
	try_add(cn, cn - max_width, k, Down);
	break;
      case Up:
	cn = c + max_width;
	try_add(cn, cn - 1, k, Left);
	try_add(cn, cn + 1, k, Right);
	try_add(cn, cn + max_width, k, Up);
	break;
      case Down:
	cn = c - max_width;
	try_add(cn, cn - 1, k, Left);
	try_add(cn, cn + 1, k, Right);
	try_add(cn, cn - max_width, k, Down);
	break;
      }
    }
  }
}

size_t PolygonGenerator::exec(string p, string f, size_t s) {
  prefixs = p;
  filename = f;
  split = s;
  Array<string> prefix_array = split_prefixs(prefixs);
  init_output();
  for (size_t i = 0; i < prefix_array.size(); ++ i) {
    exec_prefix(prefix_array[i]);
  }
  close_output();
  return sap_number;
}

uint8_t PolygonGenerator::apply(uint16_t c, uint8_t k, Step s) {
  set_t(gameboard[c], k);
  word[k] = s;
  kappa[k] = c;
  kappa[k + 1] = 127;
  return k;
}

void PolygonGenerator::try_add(size_t c, size_t cn, size_t k, Step s) {
  if (k + get_d(gameboard[cn]) >= length) return; //Base cell is unreachable
  size_t t = get_t(gameboard[cn]);
  if (t <= k and kappa[t] == cn) return; // Self intersection
  StackInfo* stack_info = stack.next();
  *stack_info = init_stack_info(c, k , s);
  stack.push();
}
