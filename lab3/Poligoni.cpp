#include <iostream>
#include <limits>
#include "GL/freeglut.h"
#include <vector>
//struktura tocke
struct Point {
	int x;
	int y;
}typedef Point;
//struktura brida
struct Edge {
	Point start;
	Point end;
	int a;
	int b;
	int c;
	bool lijevi;
}typedef Edge;

//globalni parametri
GLuint window;
GLuint width = 500, height = 500;
std::vector<Point> tocke;
std::vector<Edge> bridovi;
Point ispitnaTocka;
int brojTocaka;

void myDisplay();
void myReshape(int width, int height);
void myMouse(int button, int state, int x, int y);


int main5(int argc, char** argv) {
	std::cout << "Unesite broj vrhova poligona: ";
	std::cin >> brojTocaka;
	tocke = std::vector<Point>();
	bridovi = std::vector<Edge>();
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);
	window = glutCreateWindow("Poligoni");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutMainLoop();
	return 0;
}

//odredivanje koeficijenata a,b i c nekog brida
void izracunajKoeficijente(Edge *b) {
	b->a = b->start.y - b->end.y; //a = y[i] - y[i + 1]
	b->b = -b->start.x + b->end.x; // b = -x[i] + x[i + 1]
	b->c = b->start.x * b->end.y - b->end.x * b->start.y; // c = x[i] * y[i + 1] - x[i + 1] * y[i]
	//odredivanje je li brid lijevi ili desni brid
	if (b->start.y < b->end.y) {
		b->lijevi = true;
	}
	else {
		b->lijevi = false;
	}
}

//funkcija koja testira poziciju ispitne tocke u odnosu na poligon i ispisuje njen polozaj
void testPointPosition() {
	bool izvan = false;
	for (int i = 0; i < bridovi.size(); i++) {
		// a * x + b * y + c  > 0 --> tocka je iznad brida. Normale brida su usmjerene prema van
		if (bridovi[i].a * ispitnaTocka.x + bridovi[i].b * ispitnaTocka.y + bridovi[i].c > 0) {
			izvan = true;
		}
	}
	if (izvan) {
		std::cout << "Tocka V("<< ispitnaTocka.x <<","<< ispitnaTocka.y << ") je izvan poligona!" << std::endl;
	}
	else {
		std::cout << "Tocka V(" << ispitnaTocka.x << "," << ispitnaTocka.y << ") je unutar poligona!" << std::endl;
	}
}

//Funkcija za bojanje poligona
void paintPolygon() {
	int ymin, ymax;
	int xmin, xmax;
	glColor3f(1, 0, 0);
	//odredivanje minimalnih i maksimalnih x i y vrijednosti
	for (int i = 0; i < tocke.size(); i++) {
		if (i == 0) {
			ymin = tocke[i].y;
			ymax = tocke[i].y;
			xmin = tocke[i].x;
			xmax = tocke[i].x;
		}
		else {
			if (tocke[i].y > ymax) {
				ymax = tocke[i].y;
			}
			else if (tocke[i].y < ymin) {
				ymin = tocke[i].y;
			}
			if (tocke[i].x > xmax) {
				xmax = tocke[i].x;
			}
			else if (tocke[i].x < xmin) {
				xmin = tocke[i].x;
			}
		}
	}
	//bojanje poligona
	for (int y = ymin; y <= ymax; y++) {
		double L = xmin, D = xmax;
		for (int i = 0; i < bridovi.size(); i++) {
			if (bridovi[i].a != 0) {
				double x = (-bridovi[i].b * y - bridovi[i].c) / (double)bridovi[i].a;
				if (bridovi[i].lijevi && x > L) {
					L = x;
				}
				else if (!bridovi[i].lijevi && x < D) {
					D = x;
				}
			}
			else {
				if (bridovi[i].start.x < bridovi[i].end.x) {
					L = bridovi[i].start.x;
					D = bridovi[i].end.x;
				}
				else {
					L = bridovi[i].end.x;
					D = bridovi[i].start.x;
				}
			}
		}
		if (L < D) {
			glBegin(GL_LINES);
			glVertex2i(std::round(L) - 1, y);
			glVertex2i(std::round(D), y);
			glEnd();
		}
	}
	glColor3f(0, 0, 0);
}

//iscrtavanje ispitne tocke
void drawTestPoint() {
	glBegin(GL_POINTS);
	glPointSize(3.0f);
	glColor3f(1, 0, 0);
	glVertex2i(ispitnaTocka.x, ispitnaTocka.y);
	glEnd();
	glColor3f(0, 0, 0);
}


//Funkcija kojom se obraduje klik misa. U ovoj funkciji se interaktivno zadaju tocke poligona i ispitne tocke.
//Nakon unosa broja tocaka poligona u prozoru je potrebno odabrati n tocaka koje definiraju poligon. Kada je odabrano n tocaka, poligon se iscrtava i pocinje bojanje poligona. 
//Nakon toga se klikom na prozor mogu odabrati tocke cija se pozicija u odnosu na poligon testira. Za svaku odabranu tocku se u konzoli ispisuje njen polozaj u odnosu na poligon
void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (tocke.size() < brojTocaka) {
			tocke.push_back(Point{ x, (int)height - y });
			if (tocke.size() > 1) {
				int index = tocke.size() - 2;
				bridovi.push_back(Edge{ tocke[index], tocke[index + 1] });
				izracunajKoeficijente(&bridovi[bridovi.size() - 1]);
				glBegin(GL_LINES);
				glVertex2i(bridovi[index].start.x, bridovi[index].start.y);
				glVertex2i(bridovi[index].end.x, bridovi[index].end.y);
				glEnd();
				
			}
			if (tocke.size() == brojTocaka) {
				bridovi.push_back(Edge{ tocke[tocke.size() - 1], tocke[0] });
				izracunajKoeficijente(&bridovi[bridovi.size()- 1]);
				glBegin(GL_LINES);
				int index = bridovi.size() - 1;
				glVertex2i(bridovi[index].start.x, bridovi[index].start.y);
				glVertex2i(bridovi[index].end.x, bridovi[index].end.y);
				glEnd();
				paintPolygon();
			}
		}
		else if (tocke.size() == brojTocaka) {
			ispitnaTocka.x = x;
			ispitnaTocka.y = (int)height - y;
			glColor3f(0, 1.0, 0.0);
			glPointSize(4);
			glBegin(GL_POINTS);
			glVertex2i(ispitnaTocka.x, ispitnaTocka.y);
			glEnd();
			glColor3f(0, 0, 0);
			glPointSize(1);
			testPointPosition();
		}
		glFlush();
	}
}

//Preuzeto iz bresenham.cpp
void myDisplay()
{
	glFlush();
}
//Preuzeto iz bresenham.cpp
void myReshape(int w, int h)
{
	width = w; height = h;               //promjena sirine i visine prozora
	glViewport(0, 0, width, height);	//  otvor u prozoru

	glMatrixMode(GL_PROJECTION);		//	matrica projekcije
	glLoadIdentity();					//	jedinicna matrica
	gluOrtho2D(0, width, 0, height); 	//	okomita projekcija
	glMatrixMode(GL_MODELVIEW);			//	matrica pogleda
	glLoadIdentity();					//	jedinicna matrica

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // boja pozadine
	glClear(GL_COLOR_BUFFER_BIT);		//	brisanje pozadine
	glPointSize(1.0);					//	postavi velicinu tocke za liniju
	glColor3f(0.0f, 0.0f, 0.0f);		//	postavi boju linije
}

