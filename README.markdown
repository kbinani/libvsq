# libvsq

libvsq is a library for reading/writing/editing VSQ file (*.vsq) fes for VOCALOID2 and VOCALOID1, written in C++.

## License

The BSD License.

## Library Reference

http://ci.kbinani.info/job/libvsq-devel/doxygen/

## Building

This project uses CMake. Do `cmake .` then `make` in usual manner.

## For developers

### Unit testing

The test codes are located at `tests` directory.
Build and run them with commands:
```
cd tests
cmake .
make
./libvsqtest
```

### Test code coverage

To check the coverage, rebuild `libvsq` and `libvsqtest` with `-DCOVERAGE=true` option.
```
cmake . -DCOVERAGE=true
make
cd tests
cmake . -DCOVERAGE=true
make
./libvsqtest
cd ../
gcovr -r . --exclude=tests
```

## Notice

VOCALOID is a trademark of YAMAHA Corporation.
