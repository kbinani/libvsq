all: test_vsq

libvsq:
	cd ../ && make

test_vsq: libvsq
	g++ *.cpp ../*.cpp -L"../" -ldl -lcppunit -lvsq -o test_vsq.exe
