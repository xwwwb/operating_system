#ifndef JOB_SCHEDULE
#define JOB_SCHEDULE

#include <iostream>
#include <queue>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iomanip>
#include <string>
using namespace std;

class JobCtrlBlock {
public:
    int id;
    int status; // 就绪 0，运行 1，阻塞 2
    int priority; // 优先级
    int arrival_time; // 到达时间
    int service_time; // 需要执行的时间
    int start_time; // 开始执行的时间
    int end_time; // 结束执行的时间
    string name; // 作业名字
    void (*func)(void); // 作业执行的函数
    JobCtrlBlock(int id, string name, int priority, int arrival_time, int service_time, void (*func)(void)) {
        this->id = id;
        this->name = name;
        this->priority = priority;
        this->arrival_time = arrival_time;
        this->service_time = service_time;
        this->status = 0;
        this->func = func;
    }
};

void init();

void print();

void join_queue(JobCtrlBlock *job);

void clean();

void FCFS();

void schedule();

void func1();
void func2();
void func3();
void func4();
void func5();
void func6();
void func7();
void func8();
void func9();
void func10();


#endif
