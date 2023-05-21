#ifndef FF
#define FF
#include <stdlib.h>
#include <stdio.h>
struct Node {
    int size;
    int start;
    int end;
    int name; // 作业号
    struct Node *next;
    struct Node *prev;
};
// 初始化语句
void init(int size);
int allocate(int num, int size);
int recycle(int num);
void show();
#endif
