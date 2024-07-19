#include <stdio.h>

void simple_ifs()
{
	int option = 0;
    scanf("%d", &option);
	
	if (option == 0) {
        puts("Zero");
    }
    if (option == 1) {
        puts("One");
    }
    if (option == 2) {
        puts("Two");
    }
    if (option == 3) {
        puts("Three");
    }
    if (option == 4) {
        puts("Four");
    }
    if (option == 5) {
        puts("Five");
    }
    if (option == 6) {
        puts("Six");
    }
    if (option == 7) {
        puts("Seven");
    }
    if (option == 8) {
        puts("Eight");
    }
    if (option == 9) {
        puts("Nine");
    }
    else {
        puts("Something else");
    }
}

void complex_ifs()
{
	int x = 0;
    int param1 = 0;
    float param2 = 0.0;
    float param3 = 0.0;
    float param4 = 0.0;
    int opt = 0;
    int value = -1;
    scanf("%d %d %f %f %f %d %d", &x, &param1, &param2, &param3, &param4, &opt, &value);

    if (x == 0) {
        puts("Zero");
    }
    else if (param1 > 100){
        puts("Too large");
    }
    else if (param2 < 0.0){
        puts("Negative");
    }
    else if( param3 * param2 > 1e+5){
        puts("Invalid");
    }
    else if (param4 / 2 < param1){
        puts("Unique case");
    }
    else if (opt < 0 || opt > 6){
        puts("Invalid option");
    }
    else if (value == -1){
        puts("By default");
    }
}

int gcd(int a, int b);
int lcm(int a, int b);

void almost_complex_ifs()
{
    int x, y;
    scanf("%d %d", &x, &y);

    // different function names
    if (gcd(x, y) == 1) printf("GCD is 1\n");
    else if (lcm(x, y) == 100) printf("LCM is 100\n");
    else if (gcd(x, y) == 1337) printf("GCD is 1337\n");
    else if (gcd(x, y) == 300) printf("GCD is 300\n");
    if (lcm(x, y) == 512) printf("LCM is 512\n");
    if (gcd(x, y) == 64)  printf("GCD is 64\n");

    // different binary operators
    if (x > 100) printf("x is greater than 100\n");
    else if (x < 0) printf("x is less than 0\n");
    else if (x == 7) printf("x is 7\n");
    else if (x == 9) printf("x is 9\n");
    else if (x == 33) printf("x is 33\n");
    else if (x == 6) printf("x is 6\n");
}

int main()
{
    simple_ifs();
    complex_ifs();
}
