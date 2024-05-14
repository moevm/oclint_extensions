#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define INITIAL_SIZE 8

void wide_strings()
{
    wchar_t str1[32] = L"Hello";
    wchar_t *str2 = wcsdup(L"World");
    
    if (str2 == NULL) {
        abort();
    }
    
    if (str1 == str2) {
        puts("One");
    }
    if (str1 == L"String") {
        puts("Two");
    }
    if (L"String" == str2) {
        puts("Three");
    }
    if (L"Str1" == L"Str2") {
        puts("Four");
    }

    free(str2);
}

void compare_const_and_const()
{
    if ("String1" == "String2") {
        puts("It never happens");
    }
    else if ("String3" != "String4") {
        puts("It always happens");
    }
}

void compare_const_and_not_const()
{
    char *str = (char*)malloc(1024 * sizeof(char));

    if (str == NULL) {
        abort();
    }

    scanf("%1023s", str);
    if (str == "Bad!") {
        puts("Something");
    }
    else if ("Awful!" == str) {
        puts("Something else");
    }

    free(str);
}

int main()
{
    char buf[128];
    int arr_size = 0;
    char **arr = NULL;
    
    scanf("%d", &arr_size);
    arr = (char**)malloc(arr_size * sizeof(char*));
    
    for (int i = 0; i < arr_size; i++) {
        scanf("%127s", buf);
        arr[i] = strdup(buf);
    }

    int cnt = 1;
    for (int i = 1; i < arr_size; i++) {
        if (arr[0] == arr[i])               // bad
            cnt++;
    }

    printf("%d\n", cnt);
    
    for (int i = 0; i < arr_size; i++) {
        free(arr[i]);
    }
    free(arr);
    
    return 0;
}