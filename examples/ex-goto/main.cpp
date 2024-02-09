#include <iostream>
using namespace std;

int main()
{
	int x;
	cin >> x;

	if (x != 2) goto failed;
	cout << "Congrats! You typed 2!" << endl;

failed:
	return 0;
}
