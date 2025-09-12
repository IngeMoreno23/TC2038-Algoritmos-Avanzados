CXX = g++
CXXFLAGS = -std=c++23 -Wall
TARGET = output/main
SOURCES = act1.6/greedyCoinCollecting.cpp

$(TARGET): $(SOURCES)
	@if not exist output mkdir output
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
clean:
	@if exist output\main.exe del output\main.exe