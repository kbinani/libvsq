all: test_vsq.exe

test_vsq.exe: ../*.cpp ../*.hpp *.cpp *.hpp
	g++ *.cpp ../*.cpp -ldl -lcppunit -o test_vsq.exe

run: test_vsq.exe
	./test_vsq.exe

clean:
	rm -f *.o
	rm -f test_vsq.exe

