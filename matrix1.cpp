#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include<cblas.h>
using namespace std;

struct m {
	int r;
	int c;
	float** a;
};

int main() {
	m m1, m2, m3;
	m1.r = 10000;
	m1.c = 10000;
	m2.r = 10000;
	m2.c = 10000;
	m1.a = new float* [m1.r];
	for (int i = 0; i < m1.r; i++) {
		m1.a[i] = new float[m1.c];
	}
	for (int i = 0; i < m1.r; i++) {
		for (int j = 0; j < m1.c; j++) {
			m1.a[i][j] = rand() % 100000 / 100000 + rand() % 100;
		}
	}
	m2.a = new float* [m2.r];
	for (int i = 0; i < m2.r; i++) {
		m2.a[i] = new float[m2.c];
	}
	for (int i = 0; i < m2.r; i++) {
		for (int j = 0; j < m2.c; j++) {
			m2.a[i][j] = rand() % 100000 / 100000 + rand() % 100;
		}
	}
	m3.r = m1.r;
	m3.c = m2.c;
	m3.a = new float* [m3.r];
	for (int i = 0; i < m3.r; i++) {
		m3.a[i] = new float[m3.c];
	}

	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < m3.r; i++) {
		for (int j = 0; j < m3.c; j++) {
			m3.a[i][j] = 0;
			for (int k = 0; k < m1.c; k++) {
				m3.a[i][j] += m1.a[i][k] * m2.a[k][j];
			}
		}
	}
	cout << m3.a[0][0] << endl;
	auto end = std::chrono::steady_clock::now();
	cout << "use time "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << endl;

	for (int i = 0; i < m3.r; i++) {
		for (int j = 0; j < m3.c; j++) {
			m3.a[i][j] = 0;
		}
	}
	start = std::chrono::steady_clock::now();
	for (int i = 0; i < m3.r; i++) {
		for (int j = 0; j < m3.c; j++) {
			for (int k = 0; k < m1.c; k += 8) {
				m3.a[i][j] += m1.a[i][k] * m2.a[k][j];
				m3.a[i][j] += m1.a[i][k + 1] * m2.a[k + 1][j];
				m3.a[i][j] += m1.a[i][k + 2] * m2.a[k + 2][j];
				m3.a[i][j] += m1.a[i][k + 3] * m2.a[k + 3][j];
				m3.a[i][j] += m1.a[i][k + 4] * m2.a[k + 4][j];
				m3.a[i][j] += m1.a[i][k + 5] * m2.a[k + 5][j];
				m3.a[i][j] += m1.a[i][k + 6] * m2.a[k + 6][j];
				m3.a[i][j] += m1.a[i][k + 7] * m2.a[k + 7][j];
			}
		}
	}
	cout << m3.a[0][0] << endl;
	end = std::chrono::steady_clock::now();
	cout << "use time "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << endl;
	
	for (int i = 0; i < m3.r; i++) {
		for (int j = 0; j < m3.c; j++) {
			m3.a[i][j] = 0;
		}
	}
	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < m3.r; i++) {
		for (int k = 0; k < m1.c; k++) {
			float s = m1.a[i][k];
			for (int j = 0; j < m3.c; j++) {
				m3.a[i][j] += s * m2.a[k][j];
			}
		}
	}
	cout << m3.a[0][0] << endl;
	auto end = std::chrono::steady_clock::now();
	cout << "use time "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << endl;
	
	for (int i = 0; i < m3.r; i++) {
		for (int j = 0; j < m3.c; j++) {
			m3.a[i][j] = 0;
		}
	}
	start = std::chrono::steady_clock::now();
	for (int i = 0; i < m3.r; i++) {
		for (int k = 0; k < m1.c; k += 4) {
			float s1 = m1.a[i][k];
			float s2 = m1.a[i][k + 1];
			float s3 = m1.a[i][k + 2];
			float s4 = m1.a[i][k + 3];
			for (int j = 0; j < m3.c; j += 4) {
				m3.a[i][j] += s1 * m2.a[k][j];
				m3.a[i][j + 1] += s1 * m2.a[k][j + 1];
				m3.a[i][j + 2] += s1 * m2.a[k][j + 2];
				m3.a[i][j + 3] += s1 * m2.a[k][j + 3];

				m3.a[i][j] += s2 * m2.a[k + 1][j];
				m3.a[i][j + 1] += s2 * m2.a[k + 1][j + 1];
				m3.a[i][j + 2] += s2 * m2.a[k + 1][j + 2];
				m3.a[i][j + 3] += s2 * m2.a[k + 1][j + 3];

				m3.a[i][j] += s3 * m2.a[k + 2][j];
				m3.a[i][j + 1] += s3 * m2.a[k + 2][j + 1];
				m3.a[i][j + 2] += s3 * m2.a[k + 2][j + 2];
				m3.a[i][j + 3] += s3 * m2.a[k + 2][j + 3];

				m3.a[i][j] += s4 * m2.a[k + 3][j];
				m3.a[i][j + 1] += s4 * m2.a[k + 3][j + 1];
				m3.a[i][j + 2] += s4 * m2.a[k + 3][j + 2];
				m3.a[i][j + 3] += s4 * m2.a[k + 3][j + 3];
			}
		}
	}
	cout << m3.a[0][0] << endl;
	end = std::chrono::steady_clock::now();
	cout << "use time "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << endl;

	float* v1 = new float[m1.r*m1.c];
	float* v2 = new float[m2.r*m2.c];
	float* v3 = new float[m3.r*m3.c];
	for (int i = 0; i < m1.r; i++) {
		for (int j = 0; j < m1.c; j++) {
			v1[i * m1.c + j] = m1.a[i][j];
		}
	}
	for (int i = 0; i < m2.r; i++) {
		for (int j = 0; j < m2.c; j++) {
			v2[i * m2.c + j] = m2.a[i][j];
		}
	}
	auto start = std::chrono::steady_clock::now();
	cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.r, m2.c, m1.c, 1, v1, m1.c, v2, m2.c, 0, v3, m3.c);
	cout << v3[0]*10 << endl;
	auto end = std::chrono::steady_clock::now();
	cout << "use time "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;
	return 0;
}
