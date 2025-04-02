# Configuración para MSYS2/MinGW
CXX := g++
CXXFLAGS := -std=c++17 -O3 -Wall -Wextra -I/mingw64/include
LDFLAGS := -L/mingw64/lib -lstxxl_debug -lpthread

SRCS := main.cpp operations.cpp
OBJS := $(SRCS:.cpp=.o)
HEADERS := data_types.h operations.h
TARGET := diabetes_analysis.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean