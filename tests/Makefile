TEST_FILES=$(shell ls *Test.cpp)
TARGETS=$(TEST_FILES:.cpp=.exe)

all: $(TARGETS)

$(TARGETS): %.exe: %.cpp
	g++ main.cpp $< -w -g -ldl -lcppunit -o $@
	./$@

run: $(TARGETS)
	for TARGET in $(TARGETS); do \
	  ./$$TARGET;\
	done

clean:
	rm -f *.exe
	rm -rf *.exe.dSYM

