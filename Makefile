CXX = g++
CXXFLAG = -Wall
SRC = $(PWD)/src/*.cpp
OBJ = *.o
INCLUDE = -I $(PWD)/include
LIBS = -lSDL -lGL -lGLU

OUT_EXT = cube

$(OUT_EXT): $(OBJ)
	$(CXX) $(SRC) $(CXXFLAG) $(INCLUDE) $(LIBS) -g -o $@
	
%.o: $(PWD)/src/%.cpp 
	$(CXX) $< $(CXXFLAG) $(INCLUDE) $(LIBS) -c
