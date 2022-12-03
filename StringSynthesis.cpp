#include <iostream>
#include <time.h>
#include <random>
#include <cmath>
#include <fstream>
#include "StringSynthesisFun.h"

using namespace std;

ofstream out;

int main() {
	out.open("text.txt");
	int len = 5; // sec
	int n = 50000 * len; //44,1 к√ц - стандарт дискретизации звука
	__int16 *y = (__int16 *)malloc(n * sizeof(__int16));
	// 329,63 - e (ми первой октавы) гитары
	StringSynthesisFun2(y, 329.63, len, 50000, n);
	for (int i = 0; i < n; i++) {
		out << y[i] << '\n';
	} 
	system("python main.py");
}


