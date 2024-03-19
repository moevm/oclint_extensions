#include <iostream>
using namespace std;

int bad_glob_var = 0;

template<class T>
struct vec3 {
	T x, y, z;
};

int main()
{
	static int bad_static_var1 = 0, bad_static_var2 = 0;
	static const int bad_static_const = 13;
	static vec3<float> bad_vec { 0.f, 0.f, 0.f };

	cin >> bad_static_var1 >> bad_static_var2 >> bad_glob_var;
	cin >> bad_vec.x >> bad_vec.y >> bad_vec.z;
	
	cout << bad_static_var1 * 2 << bad_static_var2 + bad_static_const << bad_glob_var / 4 << endl;
	cout << bad_vec.x * 2 << bad_vec.y * 2 << bad_vec.z * 2 << endl;

	return 0;
}
