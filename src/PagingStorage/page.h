#ifndef PAGE
#define PAGE
#include <iostream>
#include <vector>

const int BlockSize = 1024;// 1KB
using namespace std;
struct Block {
    int num;
    int status;
};
// 页表
struct PageItem {
    int pageNum;
    int blockNum;
};
// 这里Page是单例 只有一个实例
class Page {
public:
    // 内存块 这里的int为0表示空闲 为1表示占用 实际上应该存储真实内存信息 这里仅做模拟
    vector<int> *MEM;
    Page(const vector<Block> &occupy);
    int findFreeBlock();
};


class Job {
public:
    static vector<Job *> *job_list;
    string name;
    int num;
    int size;
    // 页表
    vector<PageItem> pageTable;
    Job(string name, int num, int size);
};


void createJob();
void jobDetail();
void deleteJob();
#endif
