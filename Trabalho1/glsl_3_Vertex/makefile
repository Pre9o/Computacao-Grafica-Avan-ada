modules = glsl
files = $(addprefix ./.src/, $(modules))
objs = $(addsuffix .o, $(modules))
libs = libopengl32.a libglu32.a glfw3dll.a libglew32.a libglew32.dll.a
libpaths = $(addprefix ../Lib/, $(libs))
err = -Wall -Wpedantic -Wextra #-Werror
opt = -O3

all: main
	./main

main: $(modules)
	g++ ./src/main.cpp -o main.exe $(objs) $(libpaths) $(err) $(opt) -I./include

$(modules): %: src/%.cpp
	g++ -g -c src/$@.cpp -o $@.o $(libpaths) $(opt) $(err) -I./include

.PHONY: clean
clean:
	rm *.o *.out *.exe main.exe -rf
