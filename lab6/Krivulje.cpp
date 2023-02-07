#include <iostream>
#include <fstream>
#include <vector>
#include "Vector.h"
#include <math.h>
#include <GL/freeglut.h>
#include "Polygon.h"
#include "Utils.h"
#include "Handlers.h"
#include "Transformation.h"
#include "ViewTransformation.h"
#include "Projection.h"

std::vector<Vector<double>*>* tockeKrivulje = new std::vector<Vector<double>*>();

std::vector<Vector<double>>* tockeTijela = new std::vector<Vector<double>>();
std::vector<Poligon>* poligoniTijela = new std::vector<Poligon>();
std::vector<PoligonIndeksi>* indeksiPoligonaTijela = new std::vector<PoligonIndeksi>();

//Funkcija koja iscrtava "koordinatne osi" radi lakseg snalazenja u 3D prostoru
void drawCoordinates() {
	glBegin(GL_LINES);
	glColor3f(1.0, 0, 0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);

	glColor3f(0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);

	glColor3f(0, 0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10.0);

	glEnd();
}


std::vector<Vector<double>>* kontrolniPoligon = new std::vector<Vector<double>>();


void loadControlPolygon(std::string name) {
	std::ifstream ObjectFile(name);
	std::string line;
	std::vector<Vector<double>>* tocke = new std::vector<Vector<double>>();
	if (ObjectFile.is_open()) {
		while (ObjectFile.good()) {
			std::string name;
			ObjectFile >> name;
			if (name == "v") {
				double x, y, z;
				ObjectFile >> x >> y >> z;
				kontrolniPoligon -> push_back(Vector<double>({ x,y,z,1 }));
			}
		}
	}
}

int globalCounter = 0;

void drawObjectFromCurve() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //  boja pozadine
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //brisanje nakon svake iscrtane linije
	Vector<double> ociste = *(tockeKrivulje->at(globalCounter % tockeKrivulje->size()));
	Vector<double> glediste({ 0,0,0,1 });
	Vector<double> vektorGledatelja({ (ociste.vector[0] - glediste.vector[0]) ,
		(ociste.vector[1] - glediste.vector[1]) ,
		(ociste.vector[2] - glediste.vector[2]) });

	
	Matrix<double> t1 = getT1(&ociste);
	glediste = (glediste) * (t1);
	ociste = (ociste) * (t1);
	Matrix<double> t2 = getT2(&glediste);
	glediste = (glediste) * (t2);
	ociste = (ociste) * (t2);
	Matrix<double> t3 = getT3(&glediste);
	Matrix<double> matricaRotacije = t2 * t3 * getT4() * getT5();
	Vector<double> finalGlediste = glediste * t3 * getT4() * getT5();
	Matrix<double> matricaPogleda = t1 * matricaRotacije;
	Matrix<double> matricaProjekcije = getPerspectiveProjection(&ociste, &finalGlediste);

	std::vector<Vector<double>>* tockeTijelaCopy = new std::vector<Vector<double>>();
	for (int i = 0; i < tockeTijela->size(); i++) {
		tockeTijelaCopy->push_back(tockeTijela->at(i));
	}

	std::vector<Poligon>* poligoni = new std::vector<Poligon>();
	for (int i = 0; i < poligoniTijela->size(); i++) {
		Poligon poligon = poligoniTijela -> at(i);
		poligon.first = &(tockeTijelaCopy->at(indeksiPoligonaTijela->at(i).first));
		poligon.second = &(tockeTijelaCopy->at(indeksiPoligonaTijela->at(i).second));
		poligon.third = &(tockeTijelaCopy->at(indeksiPoligonaTijela->at(i).third));
		poligoni->push_back(poligon);
	}
	transformAndProjectVertices(tockeTijelaCopy, matricaPogleda, matricaProjekcije);
	getPolygonCoeficients(poligoni);
	std::vector<Poligon>* poligoniNew = new std::vector<Poligon>();
	for (int i = 0; i < poligoni->size(); i++) {
		Poligon poligon = poligoni->at(i);
		Vector<double> normala({ poligon.a, poligon.b, poligon.c });


		if (normala * vektorGledatelja >= 0.0) {
			poligoniNew->push_back(poligon);
		}
	}


	for (int i = 0; i < poligoniNew->size(); i++) {
		glBegin(GL_LINE_LOOP);
		Vector<double> tocka1 = *(poligoniNew->at(i).first);
		Vector<double> tocka2 = *(poligoniNew->at(i).second);
		Vector<double> tocka3 = *(poligoniNew->at(i).third);

		glVertex2d((tocka1.vector[0] / tocka1.vector[3]) * 200 + 250,
			(tocka1.vector[1] / tocka1.vector[3]) * 200 + 250);
		glVertex2d((tocka2.vector[0] / tocka2.vector[3]) * 200 + 250,
			(tocka2.vector[1] / tocka2.vector[3]) * 200 + 250);
		glVertex2d((tocka3.vector[0] / tocka3.vector[3]) * 200 + 250,
			(tocka3.vector[1] / tocka3.vector[3]) * 200 + 250);
		glEnd();
	}
	glFlush();
}

void keyPressed(unsigned char key, int x, int y) {
	if (key == 'a') {
		if (globalCounter > 0) {
			globalCounter--;
		}
	}
	else if (key == 'd') {
		globalCounter++;
	}
	drawObjectFromCurve();
}

void drawShapes(int button, int state, int x, int y) {
	glLoadIdentity();
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		drawObjectFromCurve();
		//delete poligoni;
		std::cout << "Finished" << std::endl;
	}
}

//https://www.geeksforgeeks.org/binomial-coefficient-dp-9/
int getBinomialCoefficient(int n, int k) {
	// Base Cases
	if (k > n)
		return 0;
	if (k == 0 || k == n)
		return 1;

	// Recur
	return getBinomialCoefficient(n - 1, k - 1)
		+ getBinomialCoefficient(n - 1, k);
}

double getBernsteinFunction(int i, int n, double t) {
	return getBinomialCoefficient(n, i) * pow(t, i) * pow(1.0 - t, n - i);
}

Vector<double>* calculateCurveVertex(double t) {
	Vector<double>* vertex = new Vector<double>(4);
	for (int i = 0; i < kontrolniPoligon->size(); i++) {
		std::vector<double> riTemp;
		riTemp.push_back(kontrolniPoligon->at(i).vector[0]);
		riTemp.push_back(kontrolniPoligon->at(i).vector[1]);
		riTemp.push_back(kontrolniPoligon->at(i).vector[2]);
		riTemp.push_back(1);
		Vector<double>* ri = new Vector<double>(riTemp);
		double bernstein = getBernsteinFunction(i, kontrolniPoligon->size() - 1, t);
		*ri = (bernstein * (*ri));
		*vertex = *vertex + *ri;
	}
	return vertex;
}

void calculateCurveVertices() {
	for (int i = 0; i <= 100; i++) {
		double t = 0.01 * (double)i;
		tockeKrivulje->push_back(calculateCurveVertex(t));
	}
}

int main(int argc, char** argv) {
	GLuint window;
	GLuint width = 500, height = 500;
	loadPredefinedObject("dragon.obj", tockeTijela, poligoniTijela, indeksiPoligonaTijela);
	loadControlPolygon("polygon.obj");
	calculateCurveVertices();
	
	initOpenGl(argc,argv, height, width, &window);
	glutMouseFunc(drawShapes);
	glutKeyboardUpFunc(keyPressed);
	glutMainLoop();
	return 0;
}

