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

static bool
init_decoder(lzma_stream *strm)
{
	// Initialize a .xz decoder. The decoder supports a memory usage limit
	// and a set of flags.
	//
	// The memory usage of the decompressor depends on the settings used
	// to compress a .xz file. It can vary from less than a megabyte to
	// a few gigabytes, but in practice (at least for now) it rarely
	// exceeds 65 MiB because that's how much memory is required to
	// decompress files created with "xz -9". Settings requiring more
	// memory take extra effort to use and don't (at least for now)
	// provide significantly better compression in most cases.
	//
	// Memory usage limit is useful if it is important that the
	// decompressor won't consume gigabytes of memory. The need
	// for limiting depends on the application. In this example,
	// no memory usage limiting is used. This is done by setting
	// the limit to UINT64_MAX.
	//
	// The .xz format allows concatenating compressed files as is:
	//
	//     echo foo | xz > foobar.xz
	//     echo bar | xz >> foobar.xz
	//
	// When decompressing normal standalone .xz files, LZMA_CONCATENATED
	// should always be used to support decompression of concatenated
	// .xz files. If LZMA_CONCATENATED isn't used, the decoder will stop
	// after the first .xz stream. This can be useful when .xz data has
	// been embedded inside another file format.
	//
	// Flags other than LZMA_CONCATENATED are supported too, and can
	// be combined with bitwise-or. See lzma/container.h
	// (src/liblzma/api/lzma/container.h in the source package or e.g.
	// /usr/include/lzma/container.h depending on the install prefix)
	// for details.
	lzma_ret ret = lzma_stream_decoder(
			strm, UINT64_MAX, LZMA_CONCATENATED);

	// Return successfully if the initialization went fine.
	if (ret == LZMA_OK)
		return true;

	// Something went wrong. The possible errors are documented in
	// lzma/container.h (src/liblzma/api/lzma/container.h in the source
	// package or e.g. /usr/include/lzma/container.h depending on the
	// install prefix).
	//
	// Note that LZMA_MEMLIMIT_ERROR is never possible here. If you
	// specify a very tiny limit, the error will be delayed until
	// the first headers have been parsed by a call to lzma_code().
	const char *msg;
	switch (ret) {
	case LZMA_MEM_ERROR:
		msg = "Memory allocation failed";
		break;

	case LZMA_OPTIONS_ERROR:
		msg = "Unsupported decompressor flags";
		break;

	default:
		// This is most likely LZMA_PROG_ERROR indicating a bug in
		// this program or in liblzma. It is inconvenient to have a
		// separate error message for errors that should be impossible
		// to occur, but knowing the error code is important for
		// debugging. That's why it is good to print the error code
		// at least when there is no good error message to show.
		msg = "Unknown error, possibly a bug";
		break;
	}

	fprintf(stderr, "Error initializing the decoder: %s (error code %u)\n",
			msg, ret);
	return false;
}


static bool
lzma_decompress(lzma_stream *strm, const char *inname, FILE *infile, FILE *outfile)
{
	// When LZMA_CONCATENATED flag was used when initializing the decoder,
	// we need to tell lzma_code() when there will be no more input.
	// This is done by setting action to LZMA_FINISH instead of LZMA_RUN
	// in the same way as it is done when encoding.
	//
	// When LZMA_CONCATENATED isn't used, there is no need to use
	// LZMA_FINISH to tell when all the input has been read, but it
	// is still OK to use it if you want. When LZMA_CONCATENATED isn't
	// used, the decoder will stop after the first .xz stream. In that
	// case some unused data may be left in strm->next_in.
	lzma_action action = LZMA_RUN;

	uint8_t inbuf[BUFSIZ];
	uint8_t outbuf[BUFSIZ];

	strm->next_in = NULL;
	strm->avail_in = 0;
	strm->next_out = outbuf;
	strm->avail_out = sizeof(outbuf);

	while (true) {
		if (strm->avail_in == 0 && !feof(infile)) {
			strm->next_in = inbuf;
			strm->avail_in = fread(inbuf, 1, sizeof(inbuf),
					infile);

			if (ferror(infile)) {
				fprintf(stderr, "%s: Read error: %s\n",
						inname, strerror(errno));
				return false;
			}

			// Once the end of the input file has been reached,
			// we need to tell lzma_code() that no more input
			// will be coming. As said before, this isn't required
			// if the LZMA_CONATENATED flag isn't used when
			// initializing the decoder.
			if (feof(infile))
				action = LZMA_FINISH;
		}

		lzma_ret ret = lzma_code(strm, action);

		if (strm->avail_out == 0 || ret == LZMA_STREAM_END) {
			size_t write_size = sizeof(outbuf) - strm->avail_out;

			if (fwrite(outbuf, 1, write_size, outfile)
					!= write_size) {
				fprintf(stderr, "Write error: %s\n",
						strerror(errno));
				return false;
			}

			strm->next_out = outbuf;
			strm->avail_out = sizeof(outbuf);
		}

		if (ret != LZMA_OK) {
			// Once everything has been decoded successfully, the
			// return value of lzma_code() will be LZMA_STREAM_END.
			//
			// It is important to check for LZMA_STREAM_END. Do not
			// assume that getting ret != LZMA_OK would mean that
			// everything has gone well or that when you aren't
			// getting more output it must have successfully
			// decoded everything.
			if (ret == LZMA_STREAM_END)
				return true;

			// It's not LZMA_OK nor LZMA_STREAM_END,
			// so it must be an error code. See lzma/base.h
			// (src/liblzma/api/lzma/base.h in the source package
			// or e.g. /usr/include/lzma/base.h depending on the
			// install prefix) for the list and documentation of
			// possible values. Many values listen in lzma_ret
			// enumeration aren't possible in this example, but
			// can be made possible by enabling memory usage limit
			// or adding flags to the decoder initialization.
			const char *msg;
			switch (ret) {
			case LZMA_MEM_ERROR:
				msg = "Memory allocation failed";
				break;

			case LZMA_FORMAT_ERROR:
				// .xz magic bytes weren't found.
				msg = "The input is not in the .xz format";
				break;

			case LZMA_OPTIONS_ERROR:
				// For example, the headers specify a filter
				// that isn't supported by this liblzma
				// version (or it hasn't been enabled when
				// building liblzma, but no-one sane does
				// that unless building liblzma for an
				// embedded system). Upgrading to a newer
				// liblzma might help.
				//
				// Note that it is unlikely that the file has
				// accidentally became corrupt if you get this
				// error. The integrity of the .xz headers is
				// always verified with a CRC32, so
				// unintentionally corrupt files can be
				// distinguished from unsupported files.
				msg = "Unsupported compression options";
				break;

			case LZMA_DATA_ERROR:
				msg = "Compressed file is corrupt";
				break;

			case LZMA_BUF_ERROR:
				// Typically this error means that a valid
				// file has got truncated, but it might also
				// be a damaged part in the file that makes
				// the decoder think the file is truncated.
				// If you prefer, you can use the same error
				// message for this as for LZMA_DATA_ERROR.
				msg = "Compressed file is truncated or "
						"otherwise corrupt";
				break;

			default:
				// This is most likely LZMA_PROG_ERROR.
				msg = "Unknown error, possibly a bug";
				break;
			}

			fprintf(stderr, "%s: Decoder error: "
					"%s (error code %u)\n",
					inname, msg, ret);
			return false;
		}
	}
}




bool
init_encoder(lzma_stream *strm, uint32_t preset)
{
	// Initialize the encoder using a preset. Set the integrity to check
	// to CRC64, which is the default in the xz command line tool. If
	// the .xz file needs to be decompressed with XZ Embedded, use
	// LZMA_CHECK_CRC32 instead.
	lzma_ret ret = lzma_easy_encoder(strm, preset, LZMA_CHECK_CRC64);

	// Return successfully if the initialization went fine.
	if (ret == LZMA_OK)
		return true;

	// Something went wrong. The possible errors are documented in
	// lzma/container.h (src/liblzma/api/lzma/container.h in the source
	// package or e.g. /usr/include/lzma/container.h depending on the
	// install prefix).
	const char *msg;
	switch (ret) {
	case LZMA_MEM_ERROR:
		msg = "Memory allocation failed";
		break;

	case LZMA_OPTIONS_ERROR:
		msg = "Specified preset is not supported";
		break;

	case LZMA_UNSUPPORTED_CHECK:
		msg = "Specified integrity check is not supported";
		break;

	default:
		// This is most likely LZMA_PROG_ERROR indicating a bug in
		// this program or in liblzma. It is inconvenient to have a
		// separate error message for errors that should be impossible
		// to occur, but knowing the error code is important for
		// debugging. That's why it is good to print the error code
		// at least when there is no good error message to show.
		msg = "Unknown error, possibly a bug";
		break;
	}

	fprintf(stderr, "Error initializing the encoder: %s (error code %u)\n",
			msg, ret);
	return false;
}

bool
lzma_compress(lzma_stream *strm, FILE *infile, FILE *outfile)
{
  size_t total=0;
  // This will be LZMA_RUN until the end of the input file is reached.
  // This tells lzma_code() when there will be no more input.
  lzma_action action = LZMA_RUN;
  
  // Buffers to temporarily hold uncompressed input
  // and compressed output.
  uint8_t inbuf[BUFSIZ];
  uint8_t outbuf[BUFSIZ];
  
  // Initialize the input and output pointers. Initializing next_in
  // and avail_in isn't really necessary when we are going to encode
  // just one file since LZMA_STREAM_INIT takes care of initializing
  // those already. But it doesn't hurt much and it will be needed
  // if encoding more than one file like we will in 02_decompress.c.
  //
  // While we don't care about strm->total_in or strm->total_out in this
  // example, it is worth noting that initializing the encoder will
  // always reset total_in and total_out to zero. But the encoder
  // initialization doesn't touch next_in, avail_in, next_out, or
  // avail_out.
  strm->next_in = NULL;
  strm->avail_in = 0;
  strm->next_out = outbuf;
  strm->avail_out = sizeof(outbuf);
  
  // Loop until the file has been successfully compressed or until
  // an error occurs.
  while (true) {
    // Fill the input buffer if it is empty.
    if (strm->avail_in == 0 && !feof(infile)) {
      strm->next_in = inbuf;
      strm->avail_in = fread(inbuf, 1, sizeof(inbuf),
			     infile);
      
      if (ferror(infile)) {
	fprintf(stderr, "Read error: %s\n",
		strerror(errno));
	return false;
      }
      
      // Once the end of the input file has been reached,
      // we need to tell lzma_code() that no more input
      // will be coming and that it should finish the
      // encoding.
      if (feof(infile))
	action = LZMA_FINISH;
    }
    
    // Tell liblzma do the actual encoding.
    //
    // This reads up to strm->avail_in bytes of input starting
    // from strm->next_in. avail_in will be decremented and
    // next_in incremented by an equal amount to match the
    // number of input bytes consumed.
    //
    // Up to strm->avail_out bytes of compressed output will be
    // written starting from strm->next_out. avail_out and next_out
    // will be incremented by an equal amount to match the number
    // of output bytes written.
    //
    // The encoder has to do internal buffering, which means that
    // it may take quite a bit of input before the same data is
    // available in compressed form in the output buffer.
    lzma_ret ret = lzma_code(strm, action);
    
    // If the output buffer is full or if the compression finished
    // successfully, write the data from the output bufffer to
    // the output file.
    if (strm->avail_out == 0 || ret == LZMA_STREAM_END) {
      // When lzma_code() has returned LZMA_STREAM_END,
      // the output buffer is likely to be only partially
      // full. Calculate how much new data there is to
      // be written to the output file.
      size_t write_size = sizeof(outbuf) - strm->avail_out;
      total += write_size;
      //      cout << "Write " << write_size << endl;
      if (fwrite(outbuf, 1, write_size, outfile)
	  != write_size) {
	fprintf(stderr, "Write error: %s\n",
		strerror(errno));
	return false;
      }
      
      // Reset next_out and avail_out.
      strm->next_out = outbuf;
      strm->avail_out = sizeof(outbuf);
    }
    
    // Normally the return value of lzma_code() will be LZMA_OK
    // until everything has been encoded.
    if (ret != LZMA_OK) {
      // Once everything has been encoded successfully, the
      // return value of lzma_code() will be LZMA_STREAM_END.
      //
      // It is important to check for LZMA_STREAM_END. Do not
      // assume that getting ret != LZMA_OK would mean that
      // everything has gone well.
      if (ret == LZMA_STREAM_END) {
	cout << "Write : " << total << endl;
	return true;

      }
      // It's not LZMA_OK nor LZMA_STREAM_END,
      // so it must be an error code. See lzma/base.h
      // (src/liblzma/api/lzma/base.h in the source package
      // or e.g. /usr/include/lzma/base.h depending on the
      // install prefix) for the list and documentation of
      // possible values. Most values listen in lzma_ret
      // enumeration aren't possible in this example.
      const char *msg;
      switch (ret) {
      case LZMA_MEM_ERROR:
	msg = "Memory allocation failed";
	break;
	
      case LZMA_DATA_ERROR:
	// This error is returned if the compressed
	// or uncompressed size get near 8 EiB
	// (2^63 bytes) because that's where the .xz
	// file format size limits currently are.
	// That is, the possibility of this error
	// is mostly theoretical unless you are doing
	// something very unusual.
	//
	// Note that strm->total_in and strm->total_out
	// have nothing to do with this error. Changing
	// those variables won't increase or decrease
	// the chance of getting this error.
	msg = "File size limits exceeded";
	break;
	
      default:
	// This is most likely LZMA_PROG_ERROR, but
	// if this program is buggy (or liblzma has
	// a bug), it may be e.g. LZMA_BUF_ERROR or
	// LZMA_OPTIONS_ERROR too.
	//
	// It is inconvenient to have a separate
	// error message for errors that should be
	// impossible to occur, but knowing the error
	// code is important for debugging. That's why
	// it is good to print the error code at least
	// when there is no good error message to show.
	msg = "Unknown error, possibly a bug";
	break;
      }
      
      fprintf(stderr, "Encoder error: %s (error code %u)\n",
	      msg, ret);
      return false;
    }
  }
}


/*void PolygonGenerator::compress(string filename) {
  cout << "************" << endl;
  cout << "* compress *" << endl; 
  cout << "************" << endl;
  string input = filename + ".sap";
  string output = filename + ".sapz";
  string output2 = filename + ".sapdz";
  cout << "Input " << input << endl;
  
  FILE *infile,*outfile,*outfile2;
  infile = fopen(input.c_str(), "rb");
  outfile = fopen(output.c_str(), "wb");
  outfile2 = fopen(output2.c_str(), "wb");
  lzma_stream strm = LZMA_STREAM_INIT;
  init_encoder(&strm, LZMA_PRESET_DEFAULT);
  lzma_compress(&strm, infile, outfile);
  fclose(infile);
  fclose(outfile);
  outfile = fopen(output.c_str(), "rb");
  init_decoder(&strm);
  lzma_decompress(&strm, output.c_str(), outfile, outfile2);
  fclose(outfile);
  fclose(outfile2);
    lzma_stream strm = LZMA_STREAM_INIT;
  if (lzma_easy_encoder(&strm, LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC64) != LZMA_OK) {
    std::cerr << "Erreur lors de l'initialisation de l'encodeur LZMA." << std::endl;
    return;
  }
  ifstream file_in(input.c_str(), ios::binary);
  ofstream file_out(output.c_str(), ios::binary | ios::trunc);
  char inbuf[BUFFER_SIZE];
  char outbuf[BUFFER_SIZE];
  size_t total_in = 0;
  size_t total_out = 0;
  do{
    file_in.read(inbuf, BUFFER_SIZE);
    size_t n = file_in.gcount();
    total_in += n;
    //action = LZMA_RUN;
    do {
      cout << "ici" << endl;
      strm.next_out = (uint8_t*) outbuf;
      strm.avail_out = BUFFER_SIZE;
      lzma_ret ret = lzma_code(&strm, LZMA_RUN);
      if (ret == 10) break;
      if (ret != LZMA_OK && ret != LZMA_STREAM_END) {
	std::cerr << "Erreur pendant la compression: " << ret << std::endl;
	const char *msg;
	switch (ret) {
	case LZMA_MEM_ERROR:
	  msg = "Memory allocation failed";
	  break;
	  
	case LZMA_DATA_ERROR:
	  // This error is returned if the compressed
	  // or uncompressed size get near 8 EiB
	  // (2^63 bytes) because that's where the .xz
	  // file format size limits currently are.
	  // That is, the possibility of this error
	  // is mostly theoretical unless you are doing
	  // something very unusual.
	  //
	  // Note that strm->total_in and strm->total_out
	  // have nothing to do with this error. Changing
	  // those variables won't increase or decrease
	  // the chance of getting this error.
	  msg = "File size limits exceeded";
	  break;
	  
	default:
	  // This is most likely LZMA_PROG_ERROR, but
	  // if this program is buggy (or liblzma has
	  // a bug), it may be e.g. LZMA_BUF_ERROR or
	  // LZMA_OPTIONS_ERROR too.
	  //
	  // It is inconvenient to have a separate
	  // error message for errors that should be
	  // impossible to occur, but knowing the error
	  // code is important for debugging. That's why
	  // it is good to print the error code at least
	  // when there is no good error message to show.
	  msg = "Unknown error, possibly a bug";
	  break;
	}
	cout << msg << endl;
	//lzma_end(&strm);
	break;
      }
      // Ajout des données compressées à la sortie
      size_t n_out= BUFFER_SIZE - strm.avail_out;
      cout << "Write " << n_out << " bytes" << endl;
      file_out.write(outbuf, n_out);
      total_out += n_out;
    }while(strm.avail_out == 0);
  }while(file_in);
  lzma_ret ret = lzma_code(&strm, LZMA_FINISH);
  if (ret != LZMA_OK && ret != LZMA_STREAM_END) {
    std::cerr << "Erreur pendant la fin de compression: " << ret << std::endl;
    lzma_end(&strm);
    return;
  }
  cout << "We have read " << total_in << " bytes" << endl;
  cout << "Wich give " << total_out << " bytes" << endl;
  file_in.close();
  file_out.close();
  decompress(filename);
}*/

/*void PolygonGenerator::decompress(string filename) {
  cout << "**************" << endl;
  cout << "* decompress *" << endl; 
  cout << "**************" << endl;
  string input = filename + ".sapz";
  string output = filename + ".sap";
  cout << "Input " << input << endl;
  lzma_stream strm = LZMA_STREAM_INIT;
   if (lzma_auto_decoder(&strm, UINT64_MAX, 0) != LZMA_OK) {
        std::cerr << "Erreur lors de l'initialisation du décodeur LZMA." << std::endl;
        return;
   }
   ifstream file_in(input.c_str(), ios::binary);
   ofstream file_out(output.c_str(), ios::binary | ios::trunc);
  char inbuf[BUFFER_SIZE];
  char outbuf[BUFFER_SIZE];
  size_t total_in = 0;
  size_t total_out = 0;
  do{
    file_in.read(inbuf, BUFFER_SIZE);
    size_t n = file_in.gcount();
    total_in += n;
    do {

      strm.next_out = (uint8_t*) outbuf;
      strm.avail_out = BUFFER_SIZE;
      lzma_ret ret = lzma_code(&strm, LZMA_RUN);
      if (ret != LZMA_OK && ret != LZMA_STREAM_END) {
	std::cerr << "Erreur pendant la décompression: " << ret << std::endl;
	lzma_end(&strm);
	return;
      }
      // Ajout des données compressées à la sortie
      size_t n_out= BUFFER_SIZE - strm.avail_out;
      file_out.write(outbuf, n_out);
      total_out += n_out;
    }while(strm.avail_out == 0);
  }while(file_in);
  cout << "We have read " << total_in << " bytes" << endl;
  cout << "Wich give " << total_out << " bytes" << endl;
  file_in.close();
  file_out.close();

  }*/


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

PolygonGenerator::PolygonGenerator(size_t length): gameboard(length), stack(3 * length) {
  l = length;
  word.set_length(length);
  cout << "Word = ";
  word.display();
  cout << endl;
}

void PolygonGenerator::init(string prefix) {
  size_t c = gameboard.base_cell();
  size_t k;
  for (k = 0; k < prefix.size() - 1; ++ k) {
    Step s = char_to_step(prefix[k]);
    apply(c, k, s);
    switch(s) {
    case Left :
      c -= 1;
      break;
    case Right:
      c += 1;
      break;
    case Up:
      c += max_width;
      break;
    case Down:
      c -= max_width;
      break;
    default:
      assert(false);
      break;
    }
  }
  current_stack_info = new StackInfo;
  StackInfo* stack_info = stack.next();
  *stack_info = init_stack_info(c, k , char_to_step(prefix[k]));
  stack.push();
}

size_t PolygonGenerator::exec(string prefixs, string filename, size_t split) {
  size_t np = 0;
  size_t total = 0;
  size_t nf = 0;
  Array<string> prefix_array = split_prefixs(prefixs);
  
  BinaryFile<Out> output("test.data");
  LzmaBuffer<Out, BinaryFile<Out>> lzma_output(output);
  //PolygonBuffer<Out, BinaryFile<Out>> polygon_buffer(output, l);
  PolygonBuffer<Out, LzmaBuffer<Out, BinaryFile<Out>>> polygon_buffer(lzma_output, l);
  //PolygonStreamOut output;
  //output.open(filename + '_' + to_string(nf) + ".sap", l);
  /*ofstream output_data(filename + ".dat", ios::trunc);
  output_data << "length " << l << endl;
  output_data << "prefixs " << prefixs << endl;
  output_data << "filename " << filename << endl;
  output_data << "split " << split << endl;
  output_data << "----------------------------" << endl;*/
  for (size_t i = 0; i < prefix_array.size(); ++ i) {
    init(prefix_array[i]);
    while(not stack.is_empty()) {
      stack.pop(&current_stack_info);
      StackInfo cur = *current_stack_info;
      uint8_t k = get_k(cur);
      if(k == l - 2) {
	word[k] = get_s(cur);
	//word.display();
	//cout << endl;
	//cout << " > ";
	polygon_buffer.write(word);
	//cout << endl;
	++ np;
	/*if (split !=0 and (np % split) == 0) {
	  output.close();
	  compress(filename + '_' + to_string(nf));
	  ++ nf;
	  output.open(filename + '_' + to_string(nf) + ".sap", l);
	  output_data << nf << '\t' << np << endl;
	  total += np;
	  np = 0;
	  }*/
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
  polygon_buffer.close();
  //output.close();
  /*compress(filename + '_' + to_string(nf));
  output_data << nf << '\t' << np << endl;
  output_data << "----------------------------" << endl;*/
  total += np;
  //output_data << "total " << total << endl;
  //output_data.close();
  return total;
}

uint8_t PolygonGenerator::apply(uint16_t c, uint8_t k, Step s) {
  set_t(gameboard[c], k);
  word[k] = s;
  kappa[k] = c;
  kappa[k + 1] = 127;
  return k;
}

void PolygonGenerator::try_add(size_t c, size_t cn, size_t k, Step s) {
  if (k + get_d(gameboard[cn]) >= l) return; //Base cell is unreachable
  size_t t = get_t(gameboard[cn]);
  if (t <= k and kappa[t] == cn) return; // Self intersection
  StackInfo* stack_info = stack.next();
  *stack_info = init_stack_info(c, k , s);
  stack.push();
}
