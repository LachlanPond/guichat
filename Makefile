# Variables
CXX = g++
STD = -std=c++17
SRC = src/main.cpp src/guichat.cpp
DEP = src/guichat.h
OBJ = main.o guichat.o
EXE = guichat
PKG = `pkg-config --cflags --libs gtkmm-4.0`

# Rules
$(EXE): $(OBJ)
	$(CXX) -o $(EXE) $(OBJ) $(PKG) $(STD)

$(OBJ): $(SRC) $(DEP)
	$(CXX) -c $(SRC) $(PKG) $(STD)

# Phony targets
.PHONY: clean
clean:
	rm -rf $(OBJ) $(EXE)