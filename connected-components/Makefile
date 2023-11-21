CC=g++ # Compiler
TARGET=findcomp
OBJECTS=CommandLineUtils.o ConnectedComponent.o PGMimageProcessor.o findcomp.o # Object files to build into exe
TEST_OBJECTS=CommandLineUtils.o ConnectedComponent.o PGMimageProcessor.o driver_test.o
TEST_TARGET=test.exe

# Linking Rule
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) -std=c++2a

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) catch.hpp
	$(CC) $(TEST_OBJECTS) -o $(TEST_TARGET) -std=c++2a	
# Macro: automatically associate *.o with *.cpp
.cpp.o:
	$(CC) -c $< -std=c++2a
clean:
	@rm -f *.o *.exe catch.hpp.gch


