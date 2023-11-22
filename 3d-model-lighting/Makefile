CXX=g++
CXXFLAGS= -c `sdl2-config --cflags` -std=c++11
INCLUDES= -Iinclude
# for macOS:
# LFLAGS= `sdl2-config --libs` -lGLEW -framework OpenGL
LFLAGS= `sdl2-config --libs` -lGLEW -lGL
BUILDDIR=build
SRCDIR=src
SRC=$(wildcard $(SRCDIR)/*.cpp)
_OBJ=$(SRC:.cpp=.o)
OBJ=$(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(_OBJ))
TARGET=prac1
TARGETPATH=$(BUILDDIR)/$(TARGET)
MODEL?=watermelon
ALBEDO?=watermelon_albedo.jpg
NORMAL?=watermelon_normal.jpg

build: $(OBJ) $(TARGET)

run:
	cd $(BUILDDIR); ./$(TARGET) $(MODEL) $(ALBEDO) $(NORMAL)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGETPATH) $(LFLAGS)


$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(TARGETPATH)
	rm -f $(OBJ)

