#include <stdio.h>

int main()
{
    int x = 0;
    scanf("%d", &x);

    if (x == 0) {
        puts("Zero");
    }
    else if (x == 1) {
        puts("One");
    }
    else if (x == 2) {
        puts("Two");
    }
    else if (x == 3) {
        puts("Three");
    }
    if (x == 4) {
        puts("Four");
    }
    else if (x == 5) {
        puts("Five");
    }
    if (x == 6) {
        puts("Six");
    }
    if (x == 7) {
        puts("Seven");
    }
    else if (x == 8) {
        puts("Eight");
    }
    else if (x == 9) {
        puts("Nine");
    }
    else {
        puts("Something else");
    }
}