CC=g++
CFLAGS=-DNDEBUG -O3 -Wall -IDLoopDetector \
  $(shell pkg-config --cflags opencv)
LFLAGS=-lDBoW2 -lDVision -lDUtilsCV -lDUtils \
  $(shell pkg-config --libs opencv) -lstdc++  

DEPS=
TARGET=demo_surf demo_brief

all: demo_surf demo_brief

demo_surf: demo_surf.o $(DEPS)
	$(CC) $< $(LFLAGS) -o $@

demo_brief: demo_brief.o $(DEPS)
	$(CC) $< $(LFLAGS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f *.o $(TARGET); rm -f ./lib/*.so; \
	make -C DLoopDetector clean

install:
	make -C DLoopDetector install

