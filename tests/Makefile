all: test_vsq.exe

../libvsq.a: ../*.cpp ../*.h
	cd ../ && ./configure && make

test_vsq.exe: ../libvsq.a *.cpp *.h
	g++ *.cpp -L"../" -ldl -lcppunit -lvsq -o test_vsq.exe

run: test_vsq.exe
	./test_vsq.exe

clean:
	rm -f *.o
	rm -f test_vsq.exe

