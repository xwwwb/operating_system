cmake_minimum_required(VERSION 3.24)
project(operating_system)

# 下面的语句定义编译哪个目录
set(Class 4) # 1: JobSchedule 2: ProcessSchedule 0: demo 3: FirstFit 4: BestFit

if (Class EQUAL 1)
    set(CMAKE_CXX_STANDARD 17)
    add_executable(operating_system src/JobSchedule/job_schedule.cpp src/JobSchedule/job_schedule.h
            src/JobSchedule/main.cpp src/JobSchedule/some_job.cpp src/JobSchedule/some_job.h)
elseif (Class EQUAL 2)
    set(CMAKE_CXX_STANDARD 17)
    add_executable(operating_system src/ProcessSchedule/process_schedule.cpp src/ProcessSchedule/process_schedule.h
            src/ProcessSchedule/main.cpp src/ProcessSchedule/some_process.cpp src/ProcessSchedule/some_process.h)
elseif (Class EQUAL 0)
    set(CMAKE_CXX_STANDARD 17)
    add_executable(operating_system demo.cpp src/FirstFit/main.c)
elseif (Class EQUAL 3)
    set(CMAKE_C_STANDARD 11)
    add_executable(operating_system src/FirstFit/main.c)
elseif (Class EQUAL 4)
    set(CMAKE_C_STANDARD 11)
    add_executable(operating_system src/BestFit/main.c)
endif ()