#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 8

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