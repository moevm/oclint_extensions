#include <stdio.h>
#include <stdlib.h>

int intcmp(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return ia < ib;
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