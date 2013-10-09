CXX = g++
CXXFLAG = -Wall
SRC = $(PWD)/src/*.cpp
OBJ = main.o timer.o func.o
INCLUDE = -I $(PWD)/include
LIBS = -lSDL2main -lSDL2 -lGL -lGLU

OUT_EXT = cube

$(OUT_EXT): $(OBJ)
	$(CXX) $(SRC) $(CXXFLAG) $(INCLUDE) $(LIBS) -g -o $@
	
%.o: $(PWD)/src/%.cpp 
	$(CXX) $< $(CXXFLAG) $(INCLUDE) $(LIBS) -c
