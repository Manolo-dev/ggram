CPP=g++
CFLAGS=-Wall -std=c++17 -g -Iinclude
BUILD_DIR=build
CPP_FILES := $(wildcard src/*.cpp)
OBJ=$(foreach file,$(CPP_FILES),$(BUILD_DIR)/$(file:.cpp=.o))
TEST_DIR=test
TEST_FILE=test
SOFTWARE_NAME=ggram
RM=rm

$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(BUILD_DIR)/src)

$(BUILD_DIR)/src/%.o: src/%.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(SOFTWARE_NAME): $(OBJ)
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)/$(SOFTWARE_NAME) $(OBJ)

release: $(BUILD_DIR)/$(SOFTWARE_NAME)
	$(CPP) $(CFLAGS) -O3 -o $(BUILD_DIR)/$(SOFTWARE_NAME) $(OBJ)

test: $(BUILD_DIR)/$(SOFTWARE_NAME) $(TEST_DIR)/$(TEST_FILE).gg
	./$(BUILD_DIR)/$(SOFTWARE_NAME) -f $(TEST_DIR)/$(TEST_FILE).gg -o $(TEST_DIR)/$(TEST_FILE).cpp
	$(CPP) $(CFLAGS) -o $(TEST_DIR)/$(TEST_FILE) $(TEST_DIR)/$(TEST_FILE).cpp
	./$(TEST_DIR)/$(TEST_FILE)

clean:
	$(RM) -r $(BUILD_DIR)