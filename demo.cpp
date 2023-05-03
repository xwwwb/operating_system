////
//// Created by xwwwb on 2023/5/3.
////
//#include <chrono>
//#include <iostream>
//#include <string>
//#include <thread>
//#include <windows.h>
//using namespace std;
//
//void demo() {
//    string text = "this is demo";
//    while (true) {
//        cout << text << endl;
//        this_thread::sleep_for(chrono::seconds(1));
//    }
//}
//int main() {
//    // 创建线程
//    thread t(demo);
//    HANDLE hThread = t.native_handle();
//    this_thread::sleep_for(chrono::seconds(2));
////    // 挂起线程
////    cout<<"挂起线程"<<endl;
////    SuspendThread(t.native_handle());
////    this_thread::sleep_for(chrono::seconds(5));
////    // 恢复线程
////    cout<<"恢复线程"<<endl;
////    ResumeThread(t.native_handle());
////    this_thread::sleep_for(chrono::seconds(5));
//    // 终止线程
//    cout<<"终止线程"<<endl;
//
//    TerminateThread(hThread, 0);
//
//    this_thread::sleep_for(chrono::seconds(2));
//    return 0;
//}

#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>

void thread_func()
{
    while (true)
    {
        std::cout << "Thread is running..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main()
{
    std::thread my_thread(thread_func);

    // 让主线程等待一定时间
    std::this_thread::sleep_for(std::chrono::seconds(3));

    my_thread.~thread();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    return 0;
}
