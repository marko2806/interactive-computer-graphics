#include <stdio.h>
#include <GL/glut.h>
#include "Vector.h"
#include "Polygon.h"
#include <vector>
#include <string>
#include "Utils.h"

struct Complex {
	double real;
	double complex;
}typedef Complex;


GLuint window3;
GLuint width3 = 1000, height3 = 1000;

double eps = 1000;
int m = 100;
double umin = -1.4, umax = 0.0, vmin = -1.4, vmax = 0.0;
char type = 'm';
double creal = -0.4, cimag = 0.6;

void myDisplay3();
void myReshape3(int width, int height);
void myObject3();
void updatePerspective3();

int main(int argc, char** argv)
{
	std::cout << "Unesite eps: ";
	std::cin >> eps;
	std::cout << "Unesite broj itericija: ";
	std::cin >> m;
	std::cout << "Unesite umin i umax: ";
	std::cin >> umin >> umax;
	std::cout << "Unesite vmin i vmax: ";
	std::cin >> vmin >> vmax;
	std::cout << "Mandelbrotov skup (m) / Julijev skup (j): ";
	std::cin >> type;

	if (type == 'j') {
		std::cout << "Unesite c: ";
		std::cin >> creal >> cimag;
	}

	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(width3, height3);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);
	window3 = glutCreateWindow("Fraktal");
	glutReshapeFunc(myReshape3);
	glutDisplayFunc(myDisplay3);
	glutMainLoop();
	return 0;
}

void myDisplay3(void)
{
	myObject3();
	glFlush();
}

void myReshape3(int w, int h)
{
	width3 = w; height3 = h;
	glViewport(0, 0, width3, height3);
}

void updatePerspective3()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width3, 0, height3); 
	glMatrixMode(GL_MODELVIEW);         
	glLoadIdentity();
}


Complex preslikavanje(Complex zn) {
	int c = 0;
	//return multiplyComplex(zn * zn) + c;
	//return 0;
	return Complex{ 0,0 };
}
Complex addComplex(Complex c1, Complex c2) {
	return Complex{ c1.real + c2.real, c1.complex + c2.complex };
}
Complex multiplyComplex(Complex c1, Complex c2) {
	return Complex{ c1.real * c2.real - c1.complex * c2.complex, c1.real * c2.complex + c1.complex * c2.real };
}

void mandelbrot() {
	for (int y0 = 0; y0 < 2 * height3; y0++) {
		for (int x0 = 0; x0 < 2 * width3; x0++) {
			double u0 = (umax - umin) / width3 * x0 + umin;
			double v0 = (vmax - vmin) / height3 * y0 + vmin;
			double creal = u0, cimag = v0, z0 = 0;
			int k = -1;
			Complex c{ creal, cimag };
			int n = 0;
			Complex zn{ 0,0 };
			int r;
			do {
				k += 1;
				zn = addComplex(multiplyComplex(zn, zn), c);
				r = std::sqrt(std::pow(zn.real, 2) + std::pow(zn.complex, 2));
			} while (r < eps && k < m);

			glBegin(GL_POINTS);
			if (k != m) {
				glColor3f((double)k / m, 1.0 - (double)k / m / 2.0, 0.8 - k / m / 3.0);
			}
			else {
				glColor3f(0, 0, 0);
			}
			glVertex2f(x0 / (double)width3 - 1, y0 / (double)height3 - 1);
			glEnd();
		}
	}
}

void julia() {
	for (int y0 = 0; y0 < 2 * height3; y0++) {
		for (int x0 = 0; x0 < 2 * width3; x0++) {
			double u0 = (umax - umin) / width3 * x0 + umin;
			double v0 = (vmax - vmin) / height3 * y0 + vmin;
			double z0 = 0;
			int k = -1;
			Complex c{ creal, cimag };
			int n = 0;
			Complex zn{ u0, v0 };
			int r;
			do {
				k += 1;
				zn = addComplex(multiplyComplex(zn, zn), c);
				r = std::sqrt(std::pow(zn.real, 2) + std::pow(zn.complex, 2));
			} while (r < eps && k < m);
			glBegin(GL_POINTS);
			if (k != m) {
				glColor3f((double)k / m, 0.5 + (double)k / m / 2.0, 1.0 - k / m / 3.0);
			}
			else {
				glColor3f(0, 0, 0);
			}

			glVertex2f(x0 / (double)width3 - 1, y0 / (double)height3 - 1);
			glEnd();
		}
	}
}

void myObject3()
{
	if (type == 'm') {
		mandelbrot();
	}
	else if (type == 'j') {
		julia();
	}
}

