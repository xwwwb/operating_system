#include "some_process.h"
#include <thread>
using namespace std;
void func1(const int *state) {
    string text = "func1正在运行";
    cout << text << endl;
    while (*state != 3) {
    }
}

void func2(const int *state) {
    string text = "func2正在运行";
    cout << text << endl;
    while (*state != 3) {
    }
}

void func3(const int *state) {
    string text = "func3正在运行";
    cout << text << endl;
    while (*state != 3) {
    }
}

void func4(const int *state) {
    string text = "func4正在运行";
    cout << text << endl;
    while (*state != 3) {
        this_thread::sleep_for(chrono::milliseconds(100));
        cout << text << endl;
    }
}

void func5(const int *state) {
    string text = "func5正在运行";
    cout << text << endl;
    while (*state != 3) {
    }
}

void func6(const int *state) {
    string text = "func6正在运行";
    cout << text << endl;
    while (*state != 3) {
    }
}

void func7(const int *state) {
    string text = "func7正在运行";
    cout << text << endl;
    while (*state != 3) {
    }
}

void func8(const int *state) {
    string text = "func8正在运行";
    cout << text << endl;
    while (*state != 3) {
        this_thread::sleep_for(chrono::milliseconds(100));
        cout << text << endl;
    }
}

void func9(const int *state) {
    string text = "func9正在运行";
    cout << text << endl;
    while (*state != 3) {
    }
}

void func10(const int *state) {
    string text = "func10正在运行";
    cout << text << endl;
    while (*state != 3) {
    }
}