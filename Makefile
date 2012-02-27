all: lib

SOURCES = Timesig.cpp TimesigList.cpp
OBJECTS = Timesig.o TimesigList.o

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	g++ -c -o "$@" "$<"

lib: $(OBJECTS) $(SOURCES)
	ar cq libvsq.a *.o

clean:
	rm -f *.o
	rm -f libvsq.a
