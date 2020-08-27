
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

CXX = g++
CXXFLAGS = -Wall -I $(INCLUDE_DIR)/


###################################
# Main and debug build rules

all: $(BIN_DIR)/main

debug: CXXFLAGS += -g -O0
debug: $(BIN_DIR)/main


###################################
# MAIN BINARY
$(BIN_DIR)/main: $(BUILD_DIR)/main.o $(BUILD_DIR)/state_iterator.o $(BUILD_DIR)/contingency_iterator.o $(BUILD_DIR)/block_rar_table.o
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $?


###################################
# OBJECT FILES
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/contingency_table.h $(INCLUDE_DIR)/contingency_iterator.h $(INCLUDE_DIR)/state_result.h $(INCLUDE_DIR)/block_rar_table.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $@

$(BUILD_DIR)/state_iterator.o: $(SRC_DIR)/state_iterator.cpp $(INCLUDE_DIR)/state_iterator.h $(INCLUDE_DIR)/contingency_iterator.h $(INCLUDE_DIR)/block_rar_table.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/state_iterator.cpp -o $@


$(BUILD_DIR)/contingency_iterator.o: $(SRC_DIR)/contingency_iterator.cpp $(INCLUDE_DIR)/contingency_iterator.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/contingency_iterator.cpp -o $@


$(BUILD_DIR)/block_rar_table.o: $(SRC_DIR)/block_rar_table.cpp $(INCLUDE_DIR)/block_rar_table.h $(INCLUDE_DIR)/contingency_table.h $(INCLUDE_DIR)/state_result.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/block_rar_table.cpp -o $@ 


##################################
# CLEAN UP
clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(BIN_DIR)/*
