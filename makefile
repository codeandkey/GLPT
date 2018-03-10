CC = g++
CFLAGS = -std=c++11 -g
LDFLAGS = -lBox2D -lglfw -ldl -lGL

OUTPUT = out

SOURCES = $(wildcard GLPT/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS) GLPT/glxw.o GLPT/stb_image.o
	$(CC) $(OBJECTS) GLPT/glxw.o GLPT/stb_image.o $(LDFLAGS) -o $(OUTPUT)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

GLPT/glxw.o: GLPT/glxw.c
	gcc -std=gnu99 -c $< -o $@

GLPT/stb_image.o: GLPT/stb_image.c
	gcc -std=gnu99 -c $< -o $@
