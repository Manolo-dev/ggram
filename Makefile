CPP=g++
CFLAGS=-Wall -std=c++11 -g -Iinclude -lstdc++fs
BUILD_DIR=build
CPP_FILES := $(wildcard src/*.cpp)
OBJ=$(foreach file,$(CPP_FILES),$(BUILD_DIR)/$(file:.cpp=.o))
SOFTWARE_NAME=ggram
RM=rm

$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(BUILD_DIR)/src)

$(BUILD_DIR)/src/%.o: src/%.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(SOFTWARE_NAME): $(OBJ)
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)/$(SOFTWARE_NAME) $(OBJ)

clean:
	$(RM) -r $(BUILD_DIR) test/*.cpp