SOURCEDIR=source
SOURCES=$(wildcard $(SOURCEDIR)/*.cpp)
HEADERS=$(wildcard $(SOURCEDIR)/*.hpp) $(wildcard $(SOURCEDIR)/*.inl)

BUILDROOT=build
BUILDDIR=$(BUILDROOT)/rel
BUILDDIRDEV=build/dev
OBJECTS=$(SOURCES:$(SOURCEDIR)/%.cpp=$(BUILDDIR)/%.o)
OBJECTSDEV=$(SOURCES:$(SOURCEDIR)/%.cpp=$(BUILDDIRDEV)/%.o)
EXEC=as2
EXECDEV=as2-dev

CC=g++
CFLAGS=-Wall -Wextra -std=c++11 -O0
LDFLAGS=-O0
CFLAGSDEV=$(CFLAGS) -DGLFW_OUTPUT
LDFLAGSDEV=$(LDFLAGS) -lglfw -lGL


all: $(EXEC)

dev: $(EXECDEV)

clean:
	rm -rf $(BUILDROOT)
	rm -f $(EXEC) $(EXECDEV)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

$(EXECDEV): $(OBJECTSDEV)
	$(CC) -o $@ $(OBJECTSDEV) $(LDFLAGSDEV)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $(CLFAGS) $<

$(BUILDDIRDEV)/%.o: $(SOURCEDIR)/%.cpp $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $(CFLAGSDEV) $<
