cmake_minimum_required(VERSION 3.5)

project(libcontemporary_tests LANGUAGES CXX)

cntp_test(
    NAME trange
    FILES
        main.cpp
        tst_trange.cpp
        tst_trange.h
)
