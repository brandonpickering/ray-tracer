SOURCEDIR=source
SOURCES=$(wildcard $(SOURCEDIR)/*.cpp)
HEADERS=$(wildcard $(SOURCEDIR)/*.hpp) $(wildcard $(SOURCEDIR)/*.inl)

BUILDDIR=build
OBJECTS=$(SOURCES:$(SOURCEDIR)/%.cpp=$(BUILDDIR)/%.o)
EXEC=as2

CC=g++
CFLAGS=-Wall -Wextra -std=c++11 -O0 -DDEBUG_MODE
LDFLAGS=-O0


all: $(EXEC)

clean:
	rm -rf $(BUILDDIR)
	rm -f $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $(CLFAGS) $<
