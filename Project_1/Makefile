CXX = g++
CXXFLAGS = -Wall -Werror -g -std=c++11
LDFLAGS =

TARGETS = proj1

all: $(TARGETS)

proj1: proj1.o
	$(CXX) $(CXXFLAGS) -o $@ $< 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGETS)

.PHONY: clean distclean
