CXX = g++
CXXFLAGS = -std=c++23 -Wall
TARGET = output/main
SOURCES = act2.1/suffix-array.cpp

$(TARGET): $(SOURCES)
	@if not exist output mkdir output
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
clean:
	@if exist output\main.exe del output\main.exe