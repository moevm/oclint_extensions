#include <stdio.h>
#include <stdlib.h>

int intcmp(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return ia < ib;
}

#define BLOCK_SIZE 8

int forgot_parens()
{
    size_t capacity = BLOCK_SIZE;
    size_t size = 0;

    char *str = (char *)malloc(capacity * sizeof(char));
    while (1) {
        char c = getchar();
        if (c == EOF) break;

        if (size == capacity) {
            str = (char *)realloc(str, capacity + BLOCK_SIZE * sizeof(char));
            capacity += BLOCK_SIZE;
        }

        str[size++] = c;
    }
}

int wrong_way()
{
    int len = 0;
    int *arr = NULL;

    scanf("%d", &len);
    arr = (int*)malloc(4 * len); // this is bad

    for (int i = 0; i < len; i++) {
        scanf("%d", &arr[i]);
    }
    
    qsort(arr, len, sizeof(int), intcmp);
    
    len /= 2;
    arr = (int*)realloc(arr, len * 4); // this is bad

    for (int i = 0; i < len; i++) {
        printf("%d", arr[i]);
    }
    putchar('\n');

    free(arr);
    return 0;
}

int right_way()
{
    int len = 0;
    int *arr = NULL;

    scanf("%d", &len);
    arr = (int*)malloc(sizeof(int) * len);

    for (int i = 0; i < len; i++) {
        scanf("%d", &arr[i]);
    }
    
    qsort(arr, len, sizeof(int), intcmp);
    
    len /= 2;
    arr = (int*)realloc(arr, len * sizeof(int));

    for (int i = 0; i < len; i++) {
        printf("%d", arr[i]);
    }
    putchar('\n');

    free(arr);
    return 0;
}

int main()
{
    int result = right_way();
    return right_way();
}