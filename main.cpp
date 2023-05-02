#include "job_schedule.h"
#include <fstream>

int main() {
    // 创建三个文件 用于读写log
    fstream join;
    join.open("join.txt",  ios::out);
    fstream run;
    run.open("run.txt",  ios::out);
    fstream finish;
    finish.open("finish.txt", ios::out);
    // 一个线程将任务定时push到就绪队列中
    thread push_to_queue(init, &join);
    push_to_queue.detach();

    // 等待20毫秒
    this_thread::sleep_for(chrono::milliseconds(20));
    if (schedule(&run) == 0) {
        print(&finish);
    }
    // 清理JCB
    clean();
    join.close();
    run.close();
    finish.close();

    return 0;
}
