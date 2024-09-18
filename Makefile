SAPGEN 	= bin/sapgen
CPP 	= g++
CFLAGS 	= --std=c++11 -g -O3 -D_NODEBUG -I ./tclap/include
LIBS	= -llzma
EXE 	= $(SAPGEN)

obj/%.o: src/%.cpp src/%.hpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(SAPGEN): obj/gameboard.o obj/polygon_generator.o obj/lzma_buffer.o src/sapgen.cpp
	$(CPP) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	-$(RM) obj/*.o $(EXE) src/*~ 
