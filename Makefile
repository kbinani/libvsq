all: lib

SOURCES = Timesig.cpp TimesigList.cpp
OBJECTS = Timesig.o TimesigList.o

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	g++ -c -o "$@" "$<"

lib: $(OBJECTS) $(SOURCES)
	ar cq libvsq.a *.o

cppcheck:
	cppcheck . --enable=all -q --xml 2>cppcheck-result.xml

clean:
	rm -f *.o
	rm -f libvsq.a
