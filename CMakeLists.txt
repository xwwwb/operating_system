cmake_minimum_required(VERSION 3.24)
project(operating_system)

set(CMAKE_CXX_STANDARD 17)

# 下面的语句定义编译哪个目录
set(Class 2) # 1: JobSchedule 2: ProcessSchedule 0: demo

if (Class EQUAL 1)
    add_executable(operating_system src/JobSchedule/job_schedule.cpp src/JobSchedule/job_schedule.h
            src/JobSchedule/main.cpp src/JobSchedule/some_job.cpp src/JobSchedule/some_job.h)
elseif (Class EQUAL 2)
    add_executable(operating_system src/ProcessSchedule/process_schedule.cpp src/ProcessSchedule/process_schedule.h
            src/ProcessSchedule/main.cpp src/ProcessSchedule/some_process.cpp src/ProcessSchedule/some_process.h)
elseif (Class EQUAL 0)
    add_executable(operating_system demo.cpp)
endif ()