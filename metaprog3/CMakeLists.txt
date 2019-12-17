cmake_minimum_required(VERSION 3.15)
project(metaprog3)

set(CMAKE_CXX_STANDARD 11)

add_executable(metaprog3 main.cpp functor.h typelist.h visit_factory.h shared.h)