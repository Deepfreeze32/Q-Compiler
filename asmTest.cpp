//Calling code for the 6 assembly programs built by test harness. 
//
// By Travis Cramer

#include <iostream>

using namespace std;

extern "C" int kinsmen(char a, short d[]);
extern "C" int x();
extern "C" int adding(int b);
extern "C" int adding2(int b);
extern "C" int foo(char b, short s, int l);
extern "C" bool bar();

int main()
{
	short arr[10] = {1,2,3,4,5,6,7,8,9,10};
	char alpha = 'a';
	cout << "Result of kinsmen: " << kinsmen(alpha,arr) << endl;

	cout << "Result of x: " << x() << endl;

	int c = 256;
	cout << "Result of adding: " << adding(c) << endl;

	cout << "Result of adding2: " << adding2(c) << endl;

	cout << "Result of foo: " << foo(alpha,arr[0],c) << endl;

	cout << "Result of bar: " << bar() << endl;
}