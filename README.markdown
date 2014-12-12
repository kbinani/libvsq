# libvsq

libvsq is a library for reading/writing/editing VSQ file (*.vsq) fes for VOCALOID2 and VOCALOID1, written in C++.

## Tested compilers

 OS     | Compiler                                   | Build Status
:-------|:-------------------------------------------|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------
 Linux  | g++-4.8 (Ubuntu 4.8.1-2ubuntu1~12.04) 4.8.1| [![Build Status](https://travis-ci.org/kbinani/libvsq.svg)](https://travis-ci.org/kbinani/libvsq)
 OSX    | g++-4.8 (Homebrew gcc48 4.8.3) 4.8.3       | [![Build Status](https://travis-ci.org/kbinani/libvsq.svg)](https://travis-ci.org/kbinani/libvsq)
 Windows| MSVC 18.0.31101.0                          | [![Build status](https://ci.appveyor.com/api/projects/status/2ye0j0o9t2tb6sua/branch/master?svg=true)](https://ci.appveyor.com/project/kbinani/libvsq/branch/master)

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
