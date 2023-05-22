#include <stdlib.h>
#include <stdio.h>
struct Node {
    int size;
    int start;
    int end;
    int name;// 作业号
    struct Node *next;
    struct Node *prev;
};
// 头结点
struct Node *head = NULL;
int *name_list;
// 初始化语句
void init(int size);
int allocate(int num, int size);
int recycle(int num);
void show();

int main() {
    printf("动态分区分配算法：首次适应算法\n");
    printf("请指定内存大小：");
    int memorySize;
    scanf_s("%d", &memorySize);
    init(memorySize);
    while (1) {
        printf("请选择操作：\t");
        printf("1.给作业分配内存\t");
        printf("2.回收作业内存\t");
        printf("3.查看当前分区情况\t");
        printf("4.退出\n");
        int choice;
        scanf_s("%d", &choice);
        switch (choice) {
            case 1:
                printf("请输入作业号和需要大小，空格隔开：");
                int num_1;
                int size;
                scanf_s("%d %d", &num_1, &size);
                int result_1 = allocate(num_1, size);
                if (result_1 == 0) {
                    printf("分配成功\n");
                } else if (result_1 == -1) {
                    printf("分配失败，未找到满足条件的分区\n");
                } else if (result_1 == -2) {
                    printf("分配失败，作业号重复\n");
                } else {
                    printf("分配失败，未知错误\n");
                }
                break;
            case 2:
                printf("请输入作业号：");
                int num_2;
                scanf_s("%d", &num_2);
                int result_2 = recycle(num_2);
                if (result_2 == 0) {
                    printf("回收成功\n");
                } else if (result_2 == -1) {
                    printf("回收失败，未找到作业号\n");
                } else {
                    printf("回收失败，未知错误\n");
                }
                break;
            case 3:
                show();
                break;
            case 4:
                return 0;
            default:
                printf("输入错误，请重新输入\n");
                break;
        }
    }
    return 0;
}
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
                struct Node *temp = p->prev;
                temp->size += p->size;
                temp->end = p->end;
                temp->next = p->next;
                if (p->next != NULL) {
                    p->next->prev = temp;
                }
                free(p);
                p = temp;
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