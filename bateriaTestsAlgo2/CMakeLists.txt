cmake_minimum_required(VERSION 2.8)
project(tp2)

set(CMAKE_CXX_STANDARD 11)

# Algunos flags para pasar al compilador (gnu++11 en vez de c++11 para que sea cross-plat)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -ggdb3 -g")

# Leemos todos los archivos fuentes en ./src
file(GLOB SOURCE_FILES src/*.cpp src/*.hpp src/*.h)

# Leemos todos los archivos de test en ./tests
file(GLOB TEST_SOURCES tests/string_map_test.cpp)

add_subdirectory(tests/google-test)


# Creamos el ejecutable para correr los tests
add_executable(correrLartests ${TEST_SOURCES} ${SOURCE_FILES})

# Necesitamos asociar los archivos del framework de testing
target_link_libraries(correrLartests gtest gtest_main)

add_test(correrLartests correrLartests)
