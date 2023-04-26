cmake_minimum_required(VERSION 3.24)
project(operating_system)

set(CMAKE_CXX_STANDARD 17)

add_executable(operating_system job_schedule.cpp job_schedule.h main.cpp)
