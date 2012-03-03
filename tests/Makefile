all: test_vsq.exe

../libvsq.a: ../*.cpp ../*.h
	cd ../ && ./configure && make

test_vsq.exe: ../libvsq.a *.cpp *.h
	g++ *.cpp ../*.cpp -L"../" -ldl -lcppunit -lvsq -o test_vsq.exe

run: test_vsq.exe
	./test_vsq.exe

