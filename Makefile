# Variables
CXX = g++
STD = -std=c++17
SRC = src/guichat.cpp
OBJ = guichat.o
EXE = guichat
PKG = `pkg-config --cflags --libs gtkmm-4.0`

# Rules
$(EXE): $(OBJ)
	$(CXX) -o $(EXE) $(OBJ) $(PKG) $(STD)

$(OBJ): $(SRC)
	$(CXX) -c $(SRC) $(PKG) $(STD)

# Phony targets
.PHONY: clean
clean:
	rm -rf $(OBJ) $(EXE)