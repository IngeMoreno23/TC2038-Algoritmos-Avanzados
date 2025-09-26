CXX = g++
CXXFLAGS = -std=c++23 -Wall
TARGET = output/main
SOURCES = act2.2/main.cpp act2.2/App.cpp act2.2/HuffmanTree.cpp act2.2/MenuStrategies.cpp act2.2/MenuUtils.cpp

$(TARGET): $(SOURCES)
	@if not exist output mkdir output
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
clean:
	@if exist output\main.exe del output\main.exe