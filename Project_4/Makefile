CC = g++  
CXX = g++
LD=g++
CFLAGS = -g -Wall -Werror
LDFLAGS =$(CFLAGS)  

TARGET = proj4

all: $(TARGET)

$(TARGET): proj4.cpp
	$(CXX) $(CFLAGS) -o $(TARGET) proj4.cpp $(LDFLAGS)

clean:
	rm -f $(TARGET)
	rm -rf *.dSYM

distclean: clean