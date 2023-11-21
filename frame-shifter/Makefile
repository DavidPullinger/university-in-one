CC=g++ # Compiler
TARGET=extractor # Name of executable
OBJECTS=FileUtils.o CommandLineUtils.o FrameSequence.o extractor.o # Object files to build into exe

# Linking Rule
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) -std=c++2a
# Macro: automatically associate *.o with *.cpp
.cpp.o:
	$(CC) -c $< -std=c++2a
clean:
	@rm -f *.o *.exe