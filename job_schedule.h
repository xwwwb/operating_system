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
    double response_ratio; // 响应比
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
        this->response_ratio = 0.0;
    };
};

void init(fstream *join);

void print(fstream *finish);

void join_queue(JobCtrlBlock *job,fstream *join);

void clean();

void FCFS();

int schedule(fstream *run);

#endif
