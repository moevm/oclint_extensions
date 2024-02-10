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

int main()
{
    simple_ifs();
    complex_ifs();
}
