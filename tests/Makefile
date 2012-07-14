all: test_vsq.exe

test_vsq.exe: ../*.hpp *.cpp *.hpp
	g++ *.cpp -w -ldl -lcppunit -o test_vsq.exe

run: test_vsq.exe
	./test_vsq.exe

clean:
	rm -f *.o
	rm -f test_vsq.exe

