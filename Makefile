main: src/main.cpp src/options.cpp src/Error.cpp
	g++ -std=c++11 src/main.cpp src/options.cpp src/Error.cpp -o ggram -lstdc++fs -Iinclude

test: src/test.cpp src/options.cpp src/Error.cpp
	g++ -std=c++11 src/test.cpp src/options.cpp src/Error.cpp -o test -lstdc++fs -Iinclude

clean:
	rm -f ggram test/*.cpp