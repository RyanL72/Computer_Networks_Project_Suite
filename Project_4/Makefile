CXX = g++  # Specify the C++ compiler
CFLAGS = -g -Wall -Werror -std=c++17  # Add -std=c++17 to use C++17 standard
LDFLAGS = -lstdc++fs  # Add this flag if your compiler needs it to link filesystem

TARGET = proj4

all: $(TARGET)

$(TARGET): proj4.cpp
	$(CXX) $(CFLAGS) -o $(TARGET) proj4.cpp $(LDFLAGS)

clean:
	rm -f $(TARGET)
	rm -rf *.dSYM

distclean: clean