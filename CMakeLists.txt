cmake_minimum_required(VERSION 2.6)

#Declaring projet
project(turbgen)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE ON)


#including script for handling find_package on FFTW3 OpenMP and TINYCC
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

#find_package(OpenMP REQUIRED) //not available with clang

#comment one
set(GCC_FLAGS "-std=c++1y")#libc++ must be installed (clang standard lib)
#or
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++-3.5")
#set(CLANG_FLAGS "-std=c++14 -stdlib=libc++")#libc++ must be installed (clang standard lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS} ${CLANG_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -O0 -fno-inline -DDEBUG  -Wall -Wextra  -Wno-missing-braces")#-ggdb3 plutot que -g genere et integre les info de debug pour les macros
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")# -ffast-math

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")


include_directories(
	./
	)

find_library(M_LIB m)
find_library(DL_LIB dl)

add_executable(
	ndarray_test
	./ndarray_test.cpp
	./ndarray.c
	)

add_executable(
        indexer_test
        ./indexer_test.cpp
	)

add_executable(
        interp_test
        ./interp_test.cpp
	)


#fake target
FILE(GLOB_RECURSE INC_ALL "./*.hpp")
add_custom_target(
    make_qtcreator_happy_and_able_to_find_my_headers
    SOURCES ${INC_ALL}
)