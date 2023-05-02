#include "job_schedule.h"
#include "some_job.h"
#include <fstream>
#include <strstream>

vector<JobCtrlBlock *> job_queue; // 作业队列
vector<JobCtrlBlock *> finish_queue; // 完成队列

int now_time = 0;
bool is_running = false;

thread *running;
JobCtrlBlock *running_job;


void print(fstream *finish) {
    *finish << "作业完成结果：" << endl;

    double total_weighted_turnaround_time = 0;
    for (auto job: finish_queue) {
        // 依次计算 到达时间 开始时间 运行时间 完成时间 周转时间 带权周转时间
        // 打印输出 作业名字长度固定为10
        int arrival_time = job->arrival_time;
        int start_time = job->start_time;
        int service_time = job->service_time;
        int end_time = job->end_time;
        int turnaround_time = end_time - arrival_time;
        double weighted_turnaround_time = (double) turnaround_time / service_time;

        *finish << "作业名字：" << job->name << "\t到达时间：" << arrival_time
                << "\t开始时间：" << start_time << "\t运行时间：" << service_time
                << "\t完成时间：" << end_time << "\t周转时间：" << turnaround_time
                << "\t带权周转时间：" << weighted_turnaround_time << endl;
        total_weighted_turnaround_time += weighted_turnaround_time;
    }
    *finish << "平均带权周转时间：" << total_weighted_turnaround_time / double(finish_queue.size()) << endl;

}

void join_queue(JobCtrlBlock *job, fstream *join) {
    // 等待job->arrival_time秒后加入就绪队列
    this_thread::sleep_for(chrono::seconds(job->arrival_time));
    job_queue.push_back(job);
    *join << "当前时间：" << now_time << "\t" << job->name <<
          "加入就绪队列" << endl;
}

void init(fstream *join) {
    *join << "进程装入器："<<endl;
    // 新建一些作业
    auto *job1 = new JobCtrlBlock(1, "job1", 1, 0, 2, func1);
    auto *job2 = new JobCtrlBlock(2, "job2", 2, 5, 2, func2);
    auto *job3 = new JobCtrlBlock(3, "job3", 3, 3, 1, func3);
    auto *job4 = new JobCtrlBlock(4, "job4", 4, 8, 1, func4);
    auto *job5 = new JobCtrlBlock(5, "job5", 5, 2, 2, func5);
    auto *job6 = new JobCtrlBlock(6, "job6", 6, 0, 2, func6);
    auto *job7 = new JobCtrlBlock(7, "job7", 7, 25, 2, func7);
    auto *job8 = new JobCtrlBlock(8, "job8", 8, 7, 4, func8);
    auto *job9 = new JobCtrlBlock(9, "job9", 9, 6, 1, func9);
    auto *job10 = new JobCtrlBlock(10, "job10", 10, 0, 2, func10);
    vector<JobCtrlBlock *> jobs = {job1, job2, job3, job4, job5,
                                   job6, job7, job8, job9, job10};
    for (int i = 0; i < 10; i++) {
        thread t(join_queue, jobs[i], join);
        t.detach();
    }
}

void clean() {
    for (auto job: finish_queue) {
        delete job;
    }
}

void FCFS() {
    if (running_job != nullptr) {
        if (now_time == running_job->end_time) {
            // 结束当前线程
            running->~thread();
            // 将当前线程加入完成队列
            finish_queue.push_back(running_job);
            // 重置运行状态
            is_running = false;
        }
    }
    if (is_running) {
        return;
    }
    // 取队列中的第一个作业运作
    if (!job_queue.empty()) {
        JobCtrlBlock *job = job_queue.front();
        job_queue.erase(job_queue.begin());
        // 计算开始时间
        job->start_time = now_time;
        job->end_time = now_time + job->service_time;
        is_running = true;
        running = new thread(job->func);
        running->detach();
        running_job = job;
    }
}

void SJF() {
    if (running_job != nullptr) {
        if (now_time == running_job->end_time) {
            // 结束当前线程
            running->~thread();
            // 将当前线程加入完成队列
            finish_queue.push_back(running_job);
            // 重置运行状态
            is_running = false;
        }
    }
    if (is_running) {
        return;
    }
    // 按照服务时间排序
    sort(job_queue.begin(), job_queue.end(), [](JobCtrlBlock *a, JobCtrlBlock *b) {
        return a->service_time < b->service_time;
    });
    // 取队列中的第一个作业运作
    if (!job_queue.empty()) {
        JobCtrlBlock *job = job_queue.front();
        job_queue.erase(job_queue.begin());
        // 计算开始时间
        job->start_time = now_time;
        job->end_time = now_time + job->service_time;
        is_running = true;
        running = new thread(job->func);
        running->detach();
        running_job = job;
    }

}

void HRRN() {
    // 检查是否有程序在运行
    if (running_job != nullptr) {
        if (now_time == running_job->end_time) {
            // 结束当前线程
            running->~thread();
            // 将当前线程加入完成队列
            finish_queue.push_back(running_job);
            // 重置运行状态
            is_running = false;
        }
    }
    if (is_running) {
        return;
    }
    // 计算响应比
    for (auto job: job_queue) {
        job->response_ratio = double(now_time - job->arrival_time + job->service_time) / job->service_time;
    }
    // 按照响应比排序
    sort(job_queue.begin(), job_queue.end(), [](JobCtrlBlock *a, JobCtrlBlock *b) {
        return a->response_ratio > b->response_ratio;
    });
    // 打印队列中全部元素的响应比和函数名字
    for (auto job: job_queue) {
        cout << job->response_ratio << " " << job->name << endl;
    }
    // 取队列中的第一个作业运作
    if (!job_queue.empty()) {
        JobCtrlBlock *job = job_queue.front();
        job_queue.erase(job_queue.begin());
        // 计算开始时间
        job->start_time = now_time;
        job->end_time = now_time + job->service_time;
        is_running = true;
        running = new thread(job->func);
        running->detach();
        running_job = job;
    }
}

int schedule(fstream *run) {
    while (true) {
        if (job_queue.empty()) {
            if (finish_queue.size() == 10) {
                break;
            }
        }
//        FCFS();
SJF();
        // 当前两个队列 输出到run
        *run << "当前时间：" << now_time << endl;
        *run << "当前运行中："<< running_job->name << endl;
        *run << "就绪队列：" << endl;
        for (auto job: job_queue) {
            *run << job->name << "\t";
        }
        *run << endl;
        *run << "完成队列：" << endl;
        for (auto job: finish_queue) {
            *run << job->name << "\t";
        }
        *run << "\n" << endl;

        this_thread::sleep_for(chrono::seconds(1));
        now_time++;
    }
    return 0;
}
