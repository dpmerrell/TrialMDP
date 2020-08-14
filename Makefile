
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

CXX = g++
CXXFLAGS = -Wall -g -I $(INCLUDE_DIR)/

$(BIN_DIR)/main: $(BUILD_DIR)/main.o $(BUILD_DIR)/contingency_iterator.o
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $?


$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/contingency_table.h $(INCLUDE_DIR)/contingency_iterator.h 
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $@


$(BUILD_DIR)/contingency_iterator.o: $(SRC_DIR)/contingency_iterator.cpp $(INCLUDE_DIR)/contingency_iterator.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/contingency_iterator.cpp -o $@
