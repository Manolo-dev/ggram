lib: sources
	g++ -shared lib.o -o lib.so

sources: lib.cpp
	g++ -c -fPIC lib.cpp -o lib.o -I../include 