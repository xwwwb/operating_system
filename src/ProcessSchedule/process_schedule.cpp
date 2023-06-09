#include "process_schedule.h"
#include "some_process.h"

vector<ProcessCtrlBlock *> ready_queue; // 作业队列
vector<ProcessCtrlBlock *> finish_queue;// 完成队列

int now_time = 0;

ProcessCtrlBlock *running_job;

vector<vector<int>> *time_slice;// 时间片

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
    *finish << "平均带权周转时间："
            << total_weighted_turnaround_time / double(finish_queue.size())
            << "\n"
            << endl;

    // 将finish_queue中的按id排序
    sort(finish_queue.begin(), finish_queue.end(),
         [](ProcessCtrlBlock *a, ProcessCtrlBlock *b) { return a->id < b->id; });

    *finish << "时间片分配情况：" << endl;
    *finish << setiosflags(ios::left) << setw(10) << "name"
            << "|";
    for (auto job: finish_queue) {
        *finish << setiosflags(ios::left) << setw(6) << job->name << "|";
    }
    *finish << endl;
    for (int i = 0; i < time_slice->size(); i++) {
        *finish << setiosflags(ios::left) << setw(10) << i + 1 << "|";
        for (auto job: finish_queue) {
            int is_run = (*time_slice)[i][job->id - 1];
            if (is_run == 1) {
                *finish << setiosflags(ios::left) << setw(6) << " ---- "
                        << "|";
            } else {
                *finish << setiosflags(ios::left) << setw(6) << ""
                        << "|";
            }
        }
        *finish << endl;
    }
}

void join_queue(ProcessCtrlBlock *job, fstream *join) {
    // 等待job->arrival_time秒后加入就绪队列
    this_thread::sleep_for(chrono::seconds(job->arrival_time));
    ready_queue.push_back(job);
    *join << "当前时间：" << now_time << "\t" << job->name << "加入就绪队列"
          << endl;
}

void init(fstream *join) {
    *join << "进程装入器：" << endl;
    // 新建一些作业
    auto *job1 = new ProcessCtrlBlock(1, "job1", 1, 0, 2, func1);
    auto *job2 = new ProcessCtrlBlock(2, "job2", 2, 5, 2, func2);
    auto *job3 = new ProcessCtrlBlock(3, "job3", 3, 3, 1, func3);
    auto *job4 = new ProcessCtrlBlock(4, "job4", 2, 8, 1, func4);
    auto *job5 = new ProcessCtrlBlock(5, "job5", 1, 2, 2, func5);
    auto *job6 = new ProcessCtrlBlock(6, "job6", 3, 0, 2, func6);
    auto *job7 = new ProcessCtrlBlock(7, "job7", 7, 25, 2, func7);
    auto *job8 = new ProcessCtrlBlock(8, "job8", 1, 7, 4, func8);
    auto *job9 = new ProcessCtrlBlock(9, "job9", 2, 6, 1, func9);
    auto *job10 = new ProcessCtrlBlock(10, "job10", 10, 0, 2, func10);
    vector<ProcessCtrlBlock *> jobs = {job1, job2, job3, job4, job5,
                                       job6, job7, job8, job9, job10};
    for (int i = 0; i < 10; i++) {
        thread t(join_queue, jobs[i], join);
        t.detach();
    }
    time_slice = new vector<vector<int>>();
}

void clean() {
    for (auto job: finish_queue) {
        delete job;
    }
}

void Priority() {
    if (running_job != nullptr) {
        if (running_job->round == running_job->service_time) {
            // 运行完毕
            running_job->end_time = now_time;
            cout << "终止" << running_job->name << endl;
            running_job->status = 3;// 终止
            finish_queue.push_back(running_job);
            // 从就绪队列中删除
            ready_queue.erase(
                    remove(ready_queue.begin(), ready_queue.end(), running_job),
                    ready_queue.end());
            running_job = nullptr;
        } else {
            cout << "暂停" << running_job->name << endl;
            running_job->status = 2;                              // 挂起
            SuspendThread(running_job->m_thread->native_handle());// 调用win32 api 实现进程的挂起
        }
    }
    // 按照优先级排序
    sort(ready_queue.begin(), ready_queue.end(),
         [](ProcessCtrlBlock *a, ProcessCtrlBlock *b) {
             return a->priority > b->priority;
         });
    // 取队列中的第一个作业运作
    if (!ready_queue.empty()) {
        ProcessCtrlBlock *job = ready_queue.front();
        job->priority--;                    // 运行的作业优先级降低
        job->status = 1;                    // 运行
        time_slice->back()[job->id - 1] = 1;// 修改时间片
        if (job->round == 0) {
            job->start_time = now_time;
            cout << "创建" << job->name << endl;
            job->m_thread = new thread(job->func, &job->status);
        } else {
            ResumeThread(job->m_thread->native_handle());// 调用win32 api 实现进程的恢复
        }
        job->round = job->round + 1;// 轮转次数加一
        running_job = job;          // 修改当前运行的作业
    }
    // 除了第一个队列中其余作业优先级提高
    for (int i = 1; i < ready_queue.size(); i++) {
        ready_queue[i]->priority++;
    }
}

int schedule(fstream *run) {
    while (true) {
        if (ready_queue.empty()) {
            if (finish_queue.size() == 10) {
                break;
            }
        }
        // 每运行一次 就推入一个时间片
        time_slice->push_back(vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
        Priority();
        // 当前两个队列 输出到run
        *run << "当前时间：" << now_time << endl;
        if (running_job != nullptr) {
            *run << "正在运行：" << running_job->name << endl;
        } else {
            *run << "无作业" << endl;
        }
        *run << "就绪队列：" << endl;
        for (auto job: ready_queue) {
            *run << job->name << " | " << job->priority << endl;
        }
        *run << "完成队列：" << endl;
        for (auto job: finish_queue) {
            *run << job->name << "\t";
        }
        *run << "\n"
             << endl;
        this_thread::sleep_for(chrono::seconds(1));
        now_time++;
    }
    return 0;
}