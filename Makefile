CXX = g++
CXXFLAG = -Wall -DGLEW_STATIC 
SRC = $(PWD)/src/*.cpp
OBJ = main.o timer.o functions.o 
INCLUDE = -I $(PWD)/include
LIBS = -lSDL2main -lSDL2 -lGLEW -lGL -lGLU

OUT_EXT = cube

$(OUT_EXT): $(OBJ)
	$(CXX) $(SRC) $(CXXFLAG) $(INCLUDE) $(LIBS) -g -o $@
	
%.o: $(PWD)/src/%.cpp 
	$(CXX) $< $(CXXFLAG) $(INCLUDE) $(LIBS) -c
