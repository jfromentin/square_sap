UNAME = $(shell uname)

SAPGEN = bin/sapgen
FUSION = bin/fusion
POLYFP = bin/polyfp

CPP 	= g++
CFLAGS 	= --std=c++11 -mavx2 -mfma -g -O3 -D_NODEBUG -I ./tclap/include 
ifeq ($(UNAME), Darwin) 
CFLAGS += -I /opt/homebrew/Cellar/xz/5.4.5/include # Mac brew path to lzma library
endif
LIBS	= -llzma

EXE 	= $(SAPGEN) $(FUSION) $(POLYFP)

all: $(EXE)

obj/%.o: src/%.cpp src/%.hpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(SAPGEN): obj/gameboard.o obj/polygon_generator.o obj/lzma_buffer.o src/main_sapgen.cpp
	$(CPP) $(CFLAGS) $^ -o $@ $(LIBS)

$(FUSION): obj/lzma_buffer.o obj/fusion.o src/main_fusion.cpp
	$(CPP) $(CFLAGS) $^ -o $@ $(LIBS)

$(POLYFP): obj/lzma_buffer.o src/avx_matrix.cpp src/main_polyfp.cpp src/fp_calculator.cpp
	$(CPP) $(CFLAGS) $^ -o $@ $(LIBS)	

clean:
	-$(RM) obj/*.o $(EXE) src/*~ 
