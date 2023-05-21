#include "firstfit.h"
// 头结点
struct Node *head = NULL;
int *name_list;
void init(int size) {
    // 执行初始化语句
    head = malloc(sizeof(struct Node));
    head->size = size;
    head->start = 0;
    head->end = size;
    head->next = NULL;
    head->prev = NULL;
    head->name = -1;// -1表示空闲分区
    // 根据size创建一个数组，用于存储作业号
    name_list = malloc(sizeof(int) * size);
    // 循环赋值为-1
    for (int i = 0; i < size; i++) {
        name_list[i] = -1;
    }
}
// -1 没找到符合条件的空间 -2 作业号重复
int allocate(int num, int size) {
    for (int i = 0; i < head->size; i++) {
        if (name_list[i] == num) {
            return -2;
        }
    }
    // 执行分配语句
    struct Node *p = head;
    while (p != NULL) {
        if (p->size >= size && p->name == -1) {
            struct Node *newNode = malloc(sizeof(struct Node));
            newNode->size = size;
            newNode->start = p->start;
            newNode->end = p->start + size;
            newNode->next = p;
            newNode->prev = p->prev;
            newNode->name = num;
            if (p->prev != NULL) {
                p->prev->next = newNode;
            } else {
                head = newNode;
            }
            p->start = newNode->end;
            p->size -= size;
            p->prev = newNode;

            // 将num存入name_list
            for (int i = 0; i < head->size; i++) {
                if (name_list[i] == -1) {
                    name_list[i] = num;
                    break;
                }
            }

            return 0;
        }
        p = p->next;
    }
    return -1;
}
int recycle(int num) {
    int flag = 0;
    // 查找num是否存在 存在的话从列表中删除num 不存在的话返回-1
    for (int i = 0; i < head->size; i++) {
        if (name_list[i] == num) {
            // 后面的结果向前移
            for (int j = i; j < head->size - 1; j++) {
                name_list[j] = name_list[j + 1];
            }
            flag = 1;
            break;
        }
    }
    if (flag == 0) {
        return -1;
    }
    // 执行回收语句
    struct Node *p = head;
    while (p != NULL) {
        if (p->name == num) {
            if (p->prev != NULL && p->prev->name == -1) {
                p->prev->size += p->size;
                p->prev->end = p->end;
                p->prev->next = p->next;
                if (p->next != NULL) {
                    p->next->prev = p->prev;
                }
                free(p);
                p = p->prev;
            }
            if (p->next != NULL && p->next->name == -1) {
                p->size += p->next->size;
                p->end = p->next->end;
                // 保存p->next
                struct Node *temp = p->next;
                p->next = p->next->next;
                if (p->next != NULL) {
                    p->next->prev = p;
                }
                free(temp);
            }
            // 这里存在问题 当两次均为空的时候会报错
            p->name = -1;
            return 0;
        }
        p = p->next;
    }
    return -1;
}
void show() {
    // 执行展示语句
    struct Node *p = head;
    while (p != NULL) {
        if (p->name == -1) {
            printf("start: %d, end: %d, size: %d, free\n", p->start, p->end, p->size);
        } else {
            printf("start: %d, end: %d, size: %d, name: %d\n", p->start, p->end, p->size, p->name);
        }
        p = p->next;
    }
}