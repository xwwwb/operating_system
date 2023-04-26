//
// Created by 徐文博 on 2023/4/25.
//
#include "job_schedule.h"

int main() {
    // 一个线程将任务定时push到就绪队列中
    thread push_to_queue(init);
    push_to_queue.detach();
    // 等待100毫秒
    this_thread::sleep_for(chrono::milliseconds(20));
    schedule();

    print();
    clean();
    return 0;
}
