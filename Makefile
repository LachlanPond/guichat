# Variables
CXX = g++
STD = -std=c++20

C_SRC = src/client/client.cpp src/client/guichat.cpp src/client/msgworker.cpp
C_DEP = src/client/guichat.h src/client/msgworker.h
C_OBJ = client.o guichat.o msgworker.o
C_EXE = guichat
C_PKG = `pkg-config --cflags --libs gtkmm-4.0`

S_SRC = src/server/server.cpp
S_DEP = src/server/server.h
S_OBJ = server.o
S_EXE = server

# Client Rules
all: $(C_EXE) $(S_EXE)

$(C_EXE): $(C_OBJ)
	$(CXX) -o $(C_EXE) $(C_OBJ) $(C_PKG) $(STD)

$(C_OBJ): $(C_SRC) $(C_DEP)
	$(CXX) -c $(C_SRC) $(C_PKG) $(STD)

# Server Rules
$(S_EXE): $(S_OBJ)
	$(CXX) -o $(S_EXE) $(S_OBJ) $(STD)

$(S_OBJ): $(S_SRC) $(S_DEP)
	$(CXX) -c $(S_SRC) $(STD)

# Phony targets
.PHONY: clean
clean:
	rm -rf $(C_OBJ) $(C_EXE) $(S_OBJ) $(S_EXE)