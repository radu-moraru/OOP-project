cmake_minimum_required(VERSION 3.19)
project(oop_proj)

set(CMAKE_CXX_STANDARD 14)

add_executable(oop_proj main.cpp Ptr/Ptr.h Table/Table.cpp Table/Table.h Pager/pager.cpp Value/value.cpp TableException.h Value/ValueFactory.cpp Value/ValueFactory.h
        Value/string_val.cpp Value/int.cpp Value/float_val.cpp)