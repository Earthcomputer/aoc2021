cmake_minimum_required(VERSION 3.21)
project(aoc2021)

set(CMAKE_CXX_STANDARD 20)

add_executable(aoc2021 main.cpp util.cpp registration.cpp
        day1.cpp
        day2.cpp
        day3.cpp
        day4.cpp
        day5.cpp
        day6.cpp
        )
