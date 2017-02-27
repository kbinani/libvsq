IF(MSVC)
  ADD_DEFINITIONS("/wd4018")
ELSE()
  SET(CMAKE_CXX_FLAGS "-std=c++11 -Wno-conversion-null")
ENDIF()

INCLUDE_DIRECTORIES(ext/gtest/include
                    ext/gtest
                    /usr/local/include)
IF((NOT APPLE) AND (NOT WIN32))
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
ENDIF()

SET(COVERAGE false CACHE BOOL "Enable gcov")

IF(COVERAGE)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -coverage")
  IF((NOT APPLE))
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
  ENDIF()
ENDIF()

FILE(GLOB_RECURSE _libvsqtest_src
    *.cpp
    *.hpp)

ADD_EXECUTABLE(libvsqtest
    ${_libvsqtest_src}
    ext/gtest/src/gtest-all.cc
    ext/gtest/src/gtest_main.cc)

TARGET_LINK_LIBRARIES(libvsqtest vsq)

ADD_TEST(test libvsqtest)
ENABLE_TESTING()

