CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17 `sdl2-config --cflags` -I include
LDFLAGS = `sdl2-config --libs`
SRC = $(wildcard src/*.cpp)
OUT = renderer

all:
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(LDFLAGS)
