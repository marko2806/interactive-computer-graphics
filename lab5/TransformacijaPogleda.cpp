#include <iostream>
#include <vector>
#include "Vector.h"
#include "Matrix.h"
#include <GL/freeglut.h>
#include "Projection.h"
#include "ViewTransformation.h"
#include "Polygon.h"
#include "Utils.h"
#include "Transformation.h"
#include "Handlers.h"

Vector<double>* ociste;
Vector<double>* glediste;

std::vector<Poligon> *poligoni = new std::vector<Poligon>();
std::vector<Vector<double>>* tocke = new std::vector<Vector<double>>();
std::vector<PoligonIndeksi>* indeksiPoligona = new std::vector<PoligonIndeksi>();

void redrawObject() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //  boja pozadine
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //brisanje nakon svake iscrtane linije
	Vector<double> ocisteCopy = *ociste;
	Vector<double> gledisteCopy = *glediste;
	std::cout << "Ociste:" << ocisteCopy << std::endl;
	std::cout << "Glediste:" << gledisteCopy << std::endl;
	Matrix<double> t1 = getT1(&ocisteCopy);
	gledisteCopy = gledisteCopy * (t1);
	ocisteCopy = ocisteCopy * (t1);
	Matrix<double> t2 = getT2(&gledisteCopy);
	gledisteCopy = (gledisteCopy) * (t2);
	ocisteCopy = (ocisteCopy) * (t2);
	Matrix<double> t3 = getT3(&gledisteCopy);
	Matrix<double> matricaRotacije = t2 * t3 * getT4() * getT5();
	Vector<double> finalGlediste = gledisteCopy * t3 * getT4() * getT5();
	Matrix<double> matricaPogleda = t1 * matricaRotacije;
	Matrix<double> matricaProjekcije = getPerspectiveProjection(&ocisteCopy, &finalGlediste);

	std::vector<Vector<double>>* tockeTijelaCopy = new std::vector<Vector<double>>();
	for (int i = 0; i < tocke->size(); i++) {
		tockeTijelaCopy->push_back(tocke->at(i));
	}

	std::vector<Poligon>* poligoniCopy = new std::vector<Poligon>();
	std::cout << poligoni->size() << std::endl;
	std::cout << indeksiPoligona->size() << std::endl;
	for (int i = 0; i < poligoni->size(); i++) {
		Poligon poligon = poligoni->at(i);
		poligon.first = &(tockeTijelaCopy->at(indeksiPoligona->at(i).first));
		poligon.second = &(tockeTijelaCopy->at(indeksiPoligona->at(i).second));
		poligon.third = &(tockeTijelaCopy->at(indeksiPoligona->at(i).third));
		poligoniCopy->push_back(poligon);
	}
	transformAndProjectVertices(tockeTijelaCopy, matricaPogleda, matricaProjekcije);
	getPolygonCoeficients(poligoni);


	for (int i = 0; i < poligoniCopy->size(); i++) {
		glBegin(GL_LINE_LOOP);
		Vector<double> tocka1 = *(poligoniCopy->at(i).first);
		Vector<double> tocka2 = *(poligoniCopy->at(i).second);
		Vector<double> tocka3 = *(poligoniCopy->at(i).third);

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

void applyTransformations() {
	Matrix<double> matricaTranslacije = getT1(ociste);
	Matrix<double> matricaProjekcije = getPerspectiveProjection(ociste, glediste);

	std::cout << matricaProjekcije << std::endl;

	Vector<double> glediste1 = (*glediste) * (matricaTranslacije);
	*ociste = (*ociste) * (matricaTranslacije);

	Matrix<double> t2 = getT2(&glediste1);
	Vector<double> glediste2 = (glediste1) * (t2);
	*ociste = (*ociste) * (t2);
	Matrix<double> t3 = getT3(&glediste2);
	Matrix<double> matricaPogleda = t2 * t3
		* getT4() * getT5();
	matricaPogleda = matricaTranslacije * matricaPogleda;
	Vector<double> gledisteFinal = glediste2 * t3 * getT4() * getT5();
	Vector<double> ocisteFinal = *ociste * t3 * getT4() * getT5();

	getPolygonCoeficients(poligoni);
	transformAndProjectVertices(tocke, matricaPogleda, matricaProjekcije);
}

void displayHandler(int button, int state, int x, int y) {
	redrawObject();
}

void promijeniOcisteIGlediste(unsigned char key, int x, int y) {
	if (key == 'q') {
		ociste->vector[0] += 1;
	}
	else if (key == 'w') {
		ociste->vector[1] += 1;
	}
	else if (key == 'e') {
		ociste->vector[2] += 1;
	}
	else if (key == 'a') {
		ociste->vector[0] -= 1;
	}
	else if (key == 's') {
		ociste->vector[1] -= 1;
	}
	else if (key == 'd') {
		ociste->vector[2] -= 1;
	}
	else if (key == 'r') {
		glediste->vector[0] += 1;
	}
	else if (key == 't') {
		glediste->vector[1] += 1;
	}
	else if (key == 'z') {
		glediste->vector[2] += 1;
	}
	else if (key == 'f') {
		glediste->vector[0] -= 1;
	}
	else if (key == 'g') {
		glediste->vector[1] -= 1;
	}
	else if (key == 'h') {
		glediste->vector[2] -= 1;
	}
	//applyTransformations();
	
	redrawObject();
}

void myMouse(int button, int state, int x, int y)
{
	glBegin(GL_LINES);
	glVertex2i(0, 0);
	glVertex2i(60, 60);
	glEnd();
	glFlush();
}

int main(int argc, char** argv) {
	GLuint window;
	GLuint width = 500, height = 500;

	ociste = new Vector<double>({ 0,0,6,1 });
	glediste = new Vector<double>({ 0,0,0,1 });
	
	
	
	loadPredefinedObject("teddy.obj", tocke, poligoni, indeksiPoligona);
	initOpenGl(argc, argv, height, width, &window);
	glutMouseFunc(displayHandler);
	glutKeyboardFunc(promijeniOcisteIGlediste);
	glutMainLoop();
	return 0;
}