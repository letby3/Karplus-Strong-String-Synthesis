#include <iostream>
#include <time.h>
#include <random>
#include <cmath>
#include <fstream>

using namespace std;

void StringSynthesisFun(__int16 y[], float fr, float len, float fs, int size_n);

ofstream out;

int main() {
	out.open("text.txt");
	int n = 50000 * 10; //44,1 к√ц - стандарт дискретизации звука
	__int16 *y = (__int16 *)malloc(n * sizeof(__int16));
	// 329,63 - 	e (ми первой октавы) гитары
	StringSynthesisFun(y, 329.63 , 10, 50000, n);
	for (int i = 0; i < n; i++) {
		out << y[i] << '\n';
	} 


}


void StringSynthesisFun(__int16 y[], float fr, float len, float fs, int size_n) {
	int size_noise = int(fs / fr);
	float *noise = (float *)malloc(size_noise * sizeof(float));
	float* y1 = (float*)malloc(size_n * sizeof(float));
	memset(y1, 0, size_n);
	srand(time(NULL));
	for (int i = 0; i < size_noise; i++) {
		try {
			noise[i] = (float)(rand()) / RAND_MAX * 2.0f - 1.0f;
			cout << noise[i] << endl;
		}
		catch(...) {
			cout << "!C6386, str 22\n";
		}
	}
	float mx = -1;
	for (int i = 0; i < size_n; i++) {
		if (i < size_noise)
			y1[i] = noise[i];
		else
			y1[i] = (y1[i - size_noise] + (i - size_noise - 1 >= 0 ? y1[i - size_noise - 1] : y1[0])) / 2;
		mx = max(mx, abs(y1[i]));
	}
	for (int i = 0; i < size_n; i++) {
		y1[i] /= mx;
		y[i] = __int16(y1[i] * 32767);
	}
}