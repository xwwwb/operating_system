#ifndef JOB_SCHEDULE
#define JOB_SCHEDULE

#include <iostream>
#include <queue>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iomanip>
#include <string>
#include <utility>
#include <windows.h>
#include <fstream>

using namespace std;

class ProcessCtrlBlock {
public:
    int id;
    int status; // 就绪 0，运行 1，挂起 2，完成 3
    int priority; // 优先级
    int arrival_time; // 到达时间
    int service_time; // 需要执行的时间
    int start_time{}; // 开始执行的时间
    int end_time{}; // 结束执行的时间
    int round; // 已经执行的时间片
    double response_ratio; // 响应比
    string name; // 作业名字
    thread *m_thread; // 作业的线程
    void (*func)(const int *); // 作业执行的函数
    ProcessCtrlBlock(int id, string name, int priority, int arrival_time, int service_time, void (*func)(const int *)) {
        this->id = id;
        this->name = std::move(name);
        this->priority = priority;
        this->arrival_time = arrival_time;
        this->service_time = service_time;
        this->status = 0;
        this->func = func;
        this->response_ratio = 0.0;
        this->round = 0;
        this->m_thread = nullptr;
    };
};

void init(fstream *join);

void print(fstream *finish);

void join_queue(ProcessCtrlBlock *job,fstream *join);

void clean();

void Priority();

int schedule(fstream *run);

#endif
