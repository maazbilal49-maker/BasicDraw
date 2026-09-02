CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
LIBFLAGS := -lraylib -lopengl32 -lgdi32 -lwinmm

FILES := main.cpp
TARGET := BasicDraw.exe

$(TARGET): $(FILES)
	$(CXX) $(CXXFLAGS) $(FILES) -o $(TARGET) $(LIBFLAGS)

clean:
	rm -f $(TARGET)