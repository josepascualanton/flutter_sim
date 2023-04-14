#include <cmath>
#include <iostream>

using namespace std;

void rotacion(double phi, double theeta, double psi, double rotacion[][3]) {
	rotacion[0][0] = cos(theeta) * cos(psi);
	rotacion[0][1] = sin(phi) * sin(theeta) * cos(psi) - cos(phi) * sin(psi);
	rotacion[0][2] = cos(phi) * sin(theeta) * cos(psi) + sin(phi) * sin(psi);

	rotacion[1][0] = cos(theeta) * sin(psi);
	rotacion[1][1] = sin(phi) * sin(theeta) * sin(psi) + cos(phi) * cos(psi);
	rotacion[1][2] = cos(phi) * sin(theeta) * sin(psi) - sin(phi) * cos(psi);

	rotacion[2][0] = -sin(theeta);
	rotacion[2][1] = sin(phi) * cos(theeta);
	rotacion[2][2] = cos(phi) * cos(theeta);
}

void dot_product(double A[][3], double B[], double C[3]) {

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			C[i] += A[i][j] * B[j];
		}
	}
}

void f(double x[], double t, double out[]) {
	for (int i = 0; i < 12; i++) {
		out[i] = 0;
	}
	double uvw[3] = { x[3], x[4], x[5] };
	double phitheetapsi[3] = { x[6], x[7], x[8] };

	double fxyz[3] = { 0, 0, 0 };
	double lmn[3] = { 0, 0, 0 };
	double m_rot[3][3];

	double dpndpedpd[3] = { 0, 0, 0 };

	rotacion(phitheetapsi[0], phitheetapsi[1], phitheetapsi[2], m_rot);
	dot_product(m_rot, uvw, dpndpedpd);

	for (int i = 0; i < 3; i++) {
		out[i] = dpndpedpd[i];
	}

	out[5] = out[5] - 9.81 * t;
}

using namespace std;

void Crank_Nicolson(double x0[], double xn[], int m, double t0, double tn);


void print_vector(double x[], int m) {
	cout << "[";
	bool first = true;
	for (int i = 0; i < m; i++) {
		if (first) {
			first = false;
		}
		else {
			cout << ", " << endl;
		}

		cout << x[i];
	}

	cout << "]";
}

double distancia2(double a[], double b[], int m) {
	double res = 0;

	for (int i = 0; i < m; i++) {
		double d = a[i] - b[i];
		res += d * d;
	}

	return res;
}


void Crank_Nicolson(double x0[], double xn[], int m, double t0, double tn) {

	int i = 0;
	double dt = 0.01, eps = 0.1, t = t0;

	double* const xi = new double[m];

	double* const fx0 = new double[m];
	double* const fxn = new double[m];

	while (t <= tn) {

		i = i + 1;
		eps = 1;

		//cout << "Iteracion: " << i << endl;
		while (eps >= 0.01)
		{
			f(x0, t, fx0);
			f(xn, t + dt, fxn);

			for (int j = 0; j < m; ++j) {
				xi[j] = xn[j];
			}

			for (int j = 0; j < m; j++) {
				xn[j] = x0[j] + dt * (fx0[j] + fxn[j]) / 2;
			}

			eps = distancia2(xi, xn, m) / 4.0;

			//cout << "tn: " << t << "        xn: " << endl;
			print_vector(xn, m);
			//cout << endl << "error:        " << eps << endl;

		}

		for (int j = 0; j < m; j++) {
			x0[j] = xn[j];
		}

		t = t + dt;
	}

	delete[] xi;
	delete[] fx0;
	delete[] fxn;
}


