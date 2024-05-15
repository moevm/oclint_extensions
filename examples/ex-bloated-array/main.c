#include <stdio.h>
#include <stdlib.h>

// don`t try to figure out what this code does
// it`s just for the sake of testing

int mylib_do_something(int ***arr, size_t width, size_t height);
int mylib_do_something_else(int ***arr, size_t width, size_t height);
int mylib_do_something_with_char(char ***arr, size_t width, size_t height);
int ***mylib_get_magic_pointer(int magic_number);
void mylib_dummy_func();

#define WIDTH 4
#define HEIGHT 4

int ***magic_tricks(int **arr, int magic_number) {
    int ***new_arr;
    new_arr = mylib_get_magic_pointer(magic_number);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            (*new_arr)[i][j] += arr[i][j] * 12345;
        }
    }

    return new_arr;
}

int main() {
    int **arr;

    arr = (int**)malloc(sizeof(int*) * HEIGHT);
    for (int i = 0; i < HEIGHT; i++) {
        arr[i] = (int*)malloc(sizeof(int) * WIDTH);
        for (int j = 0; j < WIDTH; j++) {
            arr[i][j] = i*j;
        }
    }

    mylib_do_something(&arr, 4, 4);

    for (int i = 0; i < HEIGHT; i++)
        arr[i][0] += 3;

    int arr2[2][3][4] = {
        { { 1, 2, 3, 4 }, { 1, 2, 3, 4 }, { 1, 2, 3, 4 } },
        { { 1, 2, 3, 4 }, { 1, 2, 3, 4 }, { 1, 2, 3, 4 } },
    };

    (void)arr2;

    int n;
    scanf("%d", &n);

    int arr3[n];
    (void)arr3;

    return 0;
}

int main_but_this_time_its_char() {
    char **arr;

    arr = (char**)malloc(sizeof(char*) * HEIGHT);
    for (int i = 0; i < HEIGHT; i++) {
        arr[i] = (char*)malloc(sizeof(char) * WIDTH);
        for (int j = 0; j < WIDTH; j++) {
            arr[i][j] = i*j;
        }
    }

    mylib_do_something_with_char(&arr, 4, 4);

    for (int i = 0; i < HEIGHT; i++)
        arr[i][0] += 3;

    return 0;
}