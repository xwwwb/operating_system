#include "page.h"
#include <fstream>
#include <iostream>
#include <cstdio>
using namespace std;
int main() {
    system("python3 ../src/PagingStorage/test.py");
    cout << "正在从文件读取物理块占用情况" << endl;
    // 读取文件 occupy.txt
    ifstream infile;
    infile.open("occupy.txt");
    if (!infile) {
        cout << "文件读取失败" << endl;
        return 0;
    }
    string line;
    vector<Block> occupy;
    while (getline(infile, line)) {
        // 如果line的长度小于2 返回
        if (line.length() < 2) {
            return 0;
        }
        Block block;
        // 以空格分割字符
        int pos = line.find(' ');
        block.num = stoi(line.substr(0, pos));
        block.status = stoi(line.substr(pos + 1));

        occupy.push_back(block);
    }
    Page page(occupy);
    infile.close();
    while (true) {
        // 打印功能
        cout << "\n请输入功能编号\n1.创建作业 2.删除作业 3.查看作业的内存分配 4.退出" << endl;
        int num;
        cin >> num;
        switch (num) {
            case 1:
                createJob();
                break;
            case 2:
                deleteJob();
                break;
            case 3:
                jobDetail();
                break;
            case 4:
                return 0;
            default:
                cout << "输入错误" << endl;
                break;
        }
    }

    return 0;
}
