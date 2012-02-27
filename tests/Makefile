all: test_vsq

libvsq:
	cd ../ && make

test_vsq: libvsq
	g++ *.cpp ../*.cpp -L"../" -lcppunit -lvsq -o test_vsq.exe
	./test_vsq.exe
