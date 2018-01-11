CC=g++
CFLAGS=-c -Wall
LDLIBS=-lglut -lGL -lGLEW
OBJECTS=$(SOURCES:.cpp=.o) ../shader_loader/GLShader.o
TARGET=$(shell basename `pwd`)
SOURCES=$(wildcard *.cpp)

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LDLIBS)

clean:
	$(RM) $(OBJECTS) $(TARGET)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
