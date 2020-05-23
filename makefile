# the compiler
CC = g++

# compiler flags
# -g	add debugging info to the executable
# -Wall	turn on (most) compiler warnings
CFLAGS = -g -Wall

# open gl flags
GLFLAGS =  -lglut -lGLU -lGL -lGLEW -lm # -sync -gldebug # -lglfw3 -lXinerama -lXcursor

TARGET = main
OUTFILE = main.o

SRC = src/*.cpp

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) $(SRC) $(TARGET).cpp -o $(OUTFILE) $(GLFLAGS)

run:
	./$(OUTFILE)

.PHONY: clean
clean:
	$(RM) $(OUTFILE)
