#include <stdio.h>
#include <stdlib.h>

struct MyStruct {
    int a;
    float b;
};

int main() {
    struct MyStruct* ptr = (struct MyStruct*)malloc(sizeof(struct MyStruct));  // 动态分配内存
    if (ptr == NULL) {
        // 处理内存分配失败的情况
        return -1;
    }

    // 使用 ptr 指向的结构体
    ptr->a = 10;
    ptr->b = 20.5;

    // 使用完后释放内存
    free(ptr);

    return 0;
}
