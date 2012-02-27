all: test_vsq

libvsq:
	cd ../ && make

test_vsq: libvsq
	g++ *.cpp ../*.cpp -L"../" -ldl -lcppunit -lvsq -o test_vsq.exe

run: test_vsq
	./test_vsq.exe

