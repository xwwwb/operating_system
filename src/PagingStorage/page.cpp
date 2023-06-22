#include "page.h"
Page *mem;
vector<Job *> *Job::job_list = new vector<Job *>();// 为什么不放在page.h中
Page::Page(const vector<Block> &occupy) {

    // 首先将occupy按Block[0]排序
    vector<Block> occupy_copy = occupy;
    sort(occupy_copy.begin(), occupy_copy.end(), [](const Block &a, const Block &b) {
        return a.num < b.num;
    });
    // 依次写入MEM
    MEM = new vector<int>(occupy_copy.size());
    for (int i = 0; i < occupy_copy.size(); i++) {
        (*MEM)[i] = occupy_copy[i].status;
    }
    cout << "内存块初始化完成" << endl;
    // 输出内存总大小 KB
    int total = MEM->size() * BlockSize / 1024;
    int used = count(MEM->begin(), MEM->end(), 1) * BlockSize / 1024;
    cout << "内存总大小" << total << "KB" << endl;
    cout << "当前可用内存" << total - used << "KB" << endl;
    cout << "单个内存块大小1KB" << endl;
    mem = this;
}
int Page::findFreeBlock() {
    // 遍历MEM 找到第一个空闲的块
    for (int i = 0; i < MEM->size(); i++) {
        if ((*MEM)[i] == 0) {
            return i;
        }
    }
    return -1;
}
Job::Job(std::string name, int num, int size) {
    this->name = name;
    this->num = num;
    this->size = size;
    // 计算页数
    int page_num = size / BlockSize;
    if (size % BlockSize != 0) {
        page_num++;
    }
    // 生成页表
    for (int i = 0; i < page_num; i++) {
        PageItem item;
        item.pageNum = i;
        item.blockNum = mem->findFreeBlock();
        if (item.blockNum == -1) {

            throw "内存不足";
        }
        mem->MEM->at(item.blockNum) = 1;
        pageTable.push_back(item);
    }
}

void createJob() {
    cout << "请输入作业名" << endl;
    string name;
    cin >> name;
    cout << "请输入作业号" << endl;
    int num;
    cin >> num;
    // 检查作业号和作业名有无重复
    for (int i = 0; i < Job::job_list->size(); i++) {
        if ((*Job::job_list)[i]->name == name || (*Job::job_list)[i]->num == num) {
            cout << "作业名或作业号重复" << endl;
            return;
        }
    }

    cout << "请输入作业大小 单位KB" << endl;
    int size;
    cin >> size;
    size = size * 1024;
    try {
        Job *job = new Job(name, num, size);
        Job::job_list->push_back(job);
        cout << "作业创建成功" << endl;
    } catch (const char *msg) {
        cout << msg << endl;
    }
}
void jobDetail() {
    // 首先展示所有作业
    if (Job::job_list->empty()) {
        cout << "当前无作业" << endl;
        return;
    }
    cout << "可用空间：" << count(mem->MEM->begin(), mem->MEM->end(), 0) * BlockSize / 1024 << "KB" << endl;
    cout << "作业名\t作业号\t作业大小" << endl;
    for (auto i: *Job::job_list) {
        cout << i->name << "\t" << i->num << "\t" << i->size / 1024 << "KB" << endl;
    }
    cout << "请输入作业号查询作业详情" << endl;
    int num;
    cin >> num;
    // 输出分配表
    for (auto i: *Job::job_list) {
        if (i->num == num) {
            cout << "页号\t块号" << endl;
            for (auto j: i->pageTable) {
                cout << j.pageNum << "\t" << j.blockNum << endl;
            }
            return;
        }
    }
}
void deleteJob() {
    cout << "请输入作业号" << endl;
    int num;
    cin >> num;
    for (int i = 0; i < Job::job_list->size(); i++) {
        if ((*Job::job_list)[i]->num == num) {
            Job* temp = (*Job::job_list)[i];
            Job::job_list->erase(Job::job_list->begin() + i);
            for (auto j: temp->pageTable) {
                mem->MEM->at(j.blockNum) = 0;
            }
            delete temp;
            cout << "作业删除成功" << endl;
            return;
        }
    }
}