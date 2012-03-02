all: lib

SOURCES = PhoneticSymbol.cpp TimesigList.cpp Lyric.cpp Tempo.cpp VibratoBP.cpp MeasureLine.cpp TextStream.cpp VibratoBPList.cpp MeasureLineIterator.cpp Timesig.cpp
OBJECTS = PhoneticSymbol.o TimesigList.o Lyric.o Tempo.o VibratoBP.o MeasureLine.o TextStream.o VibratoBPList.o MeasureLineIterator.o Timesig.o

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
