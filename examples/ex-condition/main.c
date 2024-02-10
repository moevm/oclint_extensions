#include <stdio.h>

int main()
{
    int a, b, c, d, e, f;
    scanf("%d %d %d %d %d %d", &a, &b, &c, &d, &e, &f);

    if ((((a*d == b || b == c*f) && (d == e || (e - a) == f)) || !(d/(e*e + 1) == c)) && b != e) {
        puts("Congrats! You found the secret code.");
    }
    else {
        puts("Better luck next time.");
    }

    return 0;
}
