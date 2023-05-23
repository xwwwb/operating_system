#include <stdio.h>
#include <stdlib.h>
struct Node {
    int size;
    int start;
    int end;
    int name;// 作业号
    struct Node *next;
    struct Node *prev;
};
struct FreeNode {
    int size;
    struct Node *node;
    struct FreeNode *next;
    struct FreeNode *prev;
};

// 头结点
struct Node *head = NULL;
struct FreeNode *free_head = NULL;
// 作业号列表
int *name_list;
// 初始化语句
void init(int size);
int allocate(int num, int size);
int recycle(int num);
void show();

int main() {
    printf("动态分区分配算法：最佳适应算法\n");
    printf("请指定内存大小：");
    int memorySize = 200;
    scanf("%d", &memorySize);
    init(memorySize);
    while (1) {
        printf("请选择操作：\t");
        printf("1.给作业分配内存\t");
        printf("2.回收作业内存\t");
        printf("3.查看当前分区情况\t");
        printf("4.退出\n");
        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("请输入作业号和需要大小，空格隔开：");
                int num_1;
                int size;
                scanf("%d %d", &num_1, &size);
                int result_1 = allocate(num_1, size);
                if (result_1 == 0) {
                    printf("分配成功\n");
                }
                else if (result_1 == -1) {
                    printf("分配失败，未找到满足条件的分区\n");
                }
                else if (result_1 == -2) {
                    printf("分配失败，作业号重复\n");
                }
                else {
                    printf("分配失败，未知错误\n");
                }
                break;
            case 2:
                printf("请输入作业号：");
                int num_2;
                scanf("%d", &num_2);
                int result_2 = recycle(num_2);
                if (result_2 == 0) {
                    printf("回收成功\n");
                }
                else if (result_2 == -1) {
                    printf("回收失败，未找到作业号\n");
                }
                else {
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
    // 创建一个空闲分区链表
    free_head = malloc(sizeof(struct FreeNode));
    free_head->size = size;
    free_head->node = head;
    free_head->next = NULL;
    free_head->prev = NULL;
}
// -1 没找到符合条件的空间 -2 作业号重复
int allocate(int num, int size) {
    // 作业号的查找
    for (int i = 0; i < head->size; i++) {
        if (name_list[i] == num) {
            return -2;
        }
    }
    // 查询空闲链表
    struct FreeNode *p = free_head;
    while (p != NULL) {
        if (p->size == size) {
            // 这种情况下，空闲分区大小正好等于需要分配的大小
            struct Node *node = p->node;
            // 修改node的信息 更改作业号 但是开始地址结束地址不修改
            node->name = num;
            // 将num存入name_list
            for (int i = 0; i < head->size; i++) {
                if (name_list[i] == -1) {
                    name_list[i] = num;
                    break;
                }
            }
            // 将空闲链表中的当前空闲块删除
            if (p->prev != NULL) {
                p->prev->next = p->next;
            }
            else {
                free_head = p->next;
            }
            if (p->next != NULL) {
                p->next->prev = p->prev;
            }


            free(p);
            return 0;
        }
        else if (p->size > size) {
            // 这种情况下，空闲分区的大小大于需要分配的大小
            struct Node *newNode = malloc(sizeof(struct Node));
            struct Node *node = p->node;
            newNode->size = size;
            newNode->start = node->start;
            newNode->end = size + newNode->start;
            newNode->prev = node->prev;
            newNode->name = num;
            newNode->next = node;// 原本的空块放到新块后面
            if (node->prev != NULL) {
                node->prev->next = newNode;
            }
            else {
                head = newNode;
            }
            node->prev = newNode;// node的后驱不变
            node->size -= size;
            node->start = newNode->end;
            // num放入name_list
            for (int i = 0; i < head->size; i++) {
                if (name_list[i] == -1) {
                    name_list[i] = num;
                    break;
                }
            }
            // 修改空闲块大小
            p->size = node->size;
            // 冒泡排序 按size大小从小到大排序FreeNode
            struct FreeNode *q = free_head;
            while (q->next != NULL) {
                if (q->size > q->next->size) {
                    struct FreeNode *temp = q->next;
                    q->next = temp->next;
                    temp->next->prev = q;
                    temp->prev = q->prev;
                    if (q->prev) {
                        q->prev->next = temp;
                    }
                    else {
                        free_head = temp;
                    }
                    q->prev = temp;
                    temp->next = q;
                }
                else {
                    q = q->next;
                }
            }
            return 0;
        }
        else {
            p = p->next;
        }
    }
    // 循环执行完了还没有分配到 没找到空间
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
            // 清除原本的空闲链表
            struct FreeNode *q = free_head;
            while (q != NULL) {
                struct FreeNode *temp = q;
                q = q->next;
                free(temp);
            }
            free_head = NULL;
            // 重新建立空闲链表
            struct Node *node = head;
            while (node != NULL) {
                if (node->name == -1) {
                    struct FreeNode *newNode = malloc(sizeof(struct FreeNode));
                    newNode->size = node->size;
                    newNode->node = node;
                    newNode->next = NULL;
                    newNode->prev = NULL;
                    if (free_head == NULL) {
                        free_head = newNode;
                    }
                    else {
                        struct FreeNode *p = free_head;
                        while (p->next != NULL) {
                            p = p->next;
                        }
                        p->next = newNode;
                        newNode->prev = p;
                    }
                }

                node = node->next;
            }
            // 排序
            q = free_head;
            while (q->next != NULL) {
                if (q->size > q->next->size) {
                    struct FreeNode *temp = q->next;
                    q->next = temp->next;
                    temp->next->prev = q;
                    temp->prev = q->prev;
                    if (q->prev) {
                        q->prev->next = temp;
                    }
                    else {
                        free_head = temp;
                    }
                    q->prev = temp;
                    temp->next = q;
                }
                else {
                    q = q->next;
                }
            }
            return 0;
        }
        p = p->next;
    }
}
void show() {
    // 执行展示语句
    struct Node *p = head;
    while (p != NULL) {
        if (p->name == -1) {
            printf("start: %d, end: %d, size: %d, free\n", p->start, p->end, p->size);
        }
        else {
            printf("start: %d, end: %d, size: %d, name: %d\n", p->start, p->end, p->size, p->name);
        }
        p = p->next;
    }
}