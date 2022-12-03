// начал заполнять комментарии на английском языке, решил продолжить именно на нём

#define _USE_MATH_DEFINES
#include <iostream>
#include <time.h>
#include <random>
#include <cmath>
#include <fstream>
#include <math.h>

using namespace std;

void StringSynthesisFun1(__int16 y[], float fr, float len, float fs, int size_n); // Simple Karplus-Strong algorithm for string synthesis

void Hp(double* x1, double* x2, double* y1, double* p); // Hp -  Lowpass Filter

void Hb(double y[], double x[], int pos, double* b, int* size_noise); // Hb - pick-position comb filter

void MergedFilter(double y[], double x[], int pos, double* S, double* C, int* size_noise); // z^-N * Hd(z) * Hp(z)

void Hs(double y[], double x[], int pos, double* p); // Hs - Allpass filter

void HLw(double y[], double x[], int pos, double* w, double* L); // HLw - Dynamic Level Lowpass Filter

void StringSynthesisFun2(__int16 y[], float fr, float len, float fs, int size_n); // Extensions of the Karplus - Strong Plucked - String Algorithm

void StringSynthesisFun1(__int16 y[], float fr, float len, float fs, int size_n) {
	int size_noise = int(fs / fr);
	float* noise = (float*)malloc(size_noise * sizeof(float));
	float* y1 = (float*)malloc(size_n * sizeof(float));
	memset(y1, 0, size_n);
	srand(time(NULL));
	for (int i = 0; i < size_noise; i++) {
		try {
			noise[i] = (float)(rand()) / RAND_MAX * 2.0f - 1.0f;
			//cout << noise[i] << endl;
		}
		catch (...) {
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

void StringSynthesisFun2(__int16 y[], float fr, float len, float fs, int size_n) {
	int size_noise = int(fs / fr);
	double p = 0.9; // p = 0 || 0.9 pole location
	double b = 0.5; // denotes normalized position along the string  β ∈ (0, 1)
	double S = 0.5; // “stretching factor”  S ∈ [0, 1]
	double C = 0.13; // is the only coefficient to be set
	double w = M_PI * fr/fs; // denotes the fundamental frequency in radians per second.
	double L = 0.9; 
	double a_allpass = 0.5;
	double* noise = (double*)malloc(size_noise * sizeof(double));
	double* y1 = (double*)malloc(size_n * sizeof(double)); 
	double* y2 = (double*)malloc(size_n * sizeof(double));
	memset(y1, 0, size_n); // Zeroing the array
	memset(y2, 0, size_n);
	srand(time(NULL));
	for (int i = 0; i < size_noise; i++) {
		try {
			noise[i] = (double)(rand()) / RAND_MAX * 2.0 - 1.0;
			//cout << noise[i] << endl;
			y1[i] = noise[i];
			
		}
		catch (...) {
			cout << "!C6386, str 22\n";
		}
		if (i >= 1) {
			Hp(&y1[i] , &noise[i], &y1[i - 1], &p);
		}
		else {
			y1[i] = (1 - p) * noise[i];
		}
		noise[i] = y1[i];
	}

	for (int i = 0; i < size_noise; i++) //using Hb
		Hb(y1, noise, i, &b, &size_noise);
	for (int i = size_noise; i < size_n; i++) // using z^-N * Hd(z) * Hp(z)
		MergedFilter(y1, y1, i, &S, &C, &size_noise);
	for (int i = 0; i < size_n; i++) // using lowpass after first filter loop, with other coefficient
		Hs(y2, y1, i, &a_allpass);
	for (int i = 0; i < size_n; i++) // copy 
		y1[i] = y2[i];
	for (int i = size_noise; i < size_n; i++) // using Dynamic Level Lowpass Filter
		HLw(y2, y1, i, &w, &L);
	double norm = -1; // Normalization of values
	double Lp = pow(L, 4.0 / 3.0); // L coefficient to the pow of 4/3
	for (int i = 0; i < size_n; i++) {
		y1[i] = Lp * y2[i] + (1.0 - L) * y1[i]; // lvl volume formule L = 1 max
		norm = max(norm, y1[i]); // Find max in y1[]
	}
	for (int i = 0; i < size_n; i++) // Write in __int16
		y[i] = __int16(double((y1[i]) / norm) * 32767.0); // MAX_INT16 = 32767
}

void Hp(double* x1, double* x2, double* y1, double*p) { 
	*x1 = (1 - *p) * (*x2) + (*p) * (*y1);
}

void Hb(double y[], double x[], int pos, double* b, int* size_noise) { 
	if (pos - int((*b) * *size_noise + 0.5f) >= 0)
		y[pos] = x[pos] - x[pos - int((*b) * *size_noise + 0.5f)];
	else
		y[pos] = x[pos];
}


void MergedFilter(double y[], double x[], int pos, double* S, double* C, int* size_noise) { 
	if (pos - 1 >= 0)
		y[pos] = -1.0 * *C * y[pos - 1];
	else
		y[pos] = 0;
	if (pos - *size_noise >= 0)
		y[pos] += *C * (1 - *S) * x[pos - *size_noise];
	if (pos - *size_noise - 1 >= 0)
		y[pos] += x[pos - *size_noise - 1] * ((1 - *S) + *C * *S);
	if (pos - *size_noise - 2 >= 0)
		y[pos] += x[pos - *size_noise - 2] * *S;
}

void Hs(double y[], double x[], int pos, double* p) { 
	if (pos - 1 >= 0)
		y[pos] = (1.0 / *p) * x[pos] + x[pos - 1] - y[pos - 1];
	else
		y[pos] = (1.0 / *p) * x[pos];
}

void HLw(double y[], double x[], int pos, double* w, double* L) { 
	y[pos] = (*w / (1 + *w))*(x[pos]);
	if (pos - 1 >= 0)
		y[pos] += (*w / (1 + *w)) * x[pos - 1] + (1 - *w) / (1 + *w) * y[pos - 1];
}
