#include <stdio.h>
#include <GL/glut.h>
#include "Vector.h"
#include "Polygon.h"
#include <vector>
#include <string>
#include "Utils.h"

GLuint window; 
GLuint width = 1000, height = 1000;

typedef struct _Ociste {
	GLdouble	x;
	GLdouble	y;
	GLdouble	z;
} Ociste;


Vector<double> izvor({ 2.0, 2.0, 2.0 });
Ociste	ociste = {0.0, 0.0 , 5.0};
char typeShade = 'g';

void myDisplay2		();
void myReshape2		(int width, int height);
void myMouse2		(int button, int state, int x, int y);
void myKeyboard		(unsigned char theKey, int mouseX, int mouseY);
void myObject		();
void updatePerspective ();


std::vector<Vector<double>>* tocke1 = new std::vector<Vector<double>>();
std::vector<Poligon>* poligoni1 = new std::vector<Poligon>();
std::vector<PoligonIndeksi>* indeksiPoligona1 = new std::vector<PoligonIndeksi>();

int main(int argc, char** argv)
{
	std::cout << "Konstantno (k) / Gouraudovo (g) sjencanje: ";
	std::cin >> typeShade;

	std::cout << "Koordinate izvora: ";
	std::cin >> izvor.vector[0] >> izvor.vector[1] >> izvor.vector[2];

	std::cout << "Koordinate ocista: ";
	std::cin >> ociste.x >> ociste.y >> ociste.z;

	loadPredefinedObject("teddy.obj", tocke1, poligoni1, indeksiPoligona1);
	getPolygonCoeficients(poligoni1);
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize (width, height); 
	glutInitWindowPosition (100, 100);
	glutInit(&argc, argv);
	window = glutCreateWindow ("Sjencanje");
	glutReshapeFunc(myReshape2);
	glutDisplayFunc(myDisplay2);
	glutMouseFunc(myMouse2);
	glutKeyboardFunc(myKeyboard);
	printf("Tipka: l - pomicanje ocista po x os +\n");
	printf("Tipka: k - pomicanje ocista po x os -\n");
	printf("Tipka: r - pocetno stanje\n");
	printf("esc: izlaz iz programa\n");

	glutMainLoop();
	return 0;
}

void myDisplay2(void)
{
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	myObject ();
	glutSwapBuffers();
}

void myReshape2 (int w, int h)
{
	width=w; height=h;
	glViewport (0, 0, width, height); 
	updatePerspective();
}

void updatePerspective()
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(45.0, (float)width/height, 0.5, 15.0);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	gluLookAt (ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void gouraud() {
	std::vector<Vector<double>> normale;
	for (int i = 0; i < tocke1->size(); i++) {
		normale.push_back(Vector<double>({ 0.0,0.0,0.0 }));
	}
	for (int i = 0; i < indeksiPoligona1->size(); i++) {
		normale[indeksiPoligona1->at(i).first] =  normale[indeksiPoligona1->at(i).first]  + Vector<double>({ poligoni1->at(i).a,poligoni1->at(i).b, poligoni1->at(i).c }).normalized();
		normale[indeksiPoligona1->at(i).second] = normale[indeksiPoligona1->at(i).second] + Vector<double>({ poligoni1->at(i).a,poligoni1->at(i).b, poligoni1->at(i).c }).normalized();
		normale[indeksiPoligona1->at(i).third] =  normale[indeksiPoligona1->at(i).third]  + Vector<double>({ poligoni1->at(i).a,poligoni1->at(i).b, poligoni1->at(i).c }).normalized();
	}
	for (int i = 0; i < normale.size(); i++) {
		normale[i] = normale[i].normalized();
	}


	for (int i = 0; i < poligoni1->size(); i++) {
		Vector<double> normala({ poligoni1->at(i).a,poligoni1->at(i).b, poligoni1->at(i).c });
		Vector<double> srediste = (1.0 / 3.0) * (*poligoni1->at(i).first)
			+ (1.0 / 3.0) * (*poligoni1->at(i).second)
			+ (1.0 / 3.0) * (*poligoni1->at(i).third);
		Vector<double> ociste({ ociste.x, ociste.y, ociste.z });
		Vector<double> b = ociste - srediste;
		Vector<double> l = izvor - srediste;
		if (normala.normalized() * ociste.normalized() >= 0) {
			double ia = 0.0;
			double ka = 0.0;
			double ii = 255.0;
			double kd = 1.0;
			Vector<double> na1 = normale.at(indeksiPoligona1->at(i).first);
			Vector<double> na2 = normale.at(indeksiPoligona1->at(i).second);
			Vector<double> na3 = normale.at(indeksiPoligona1->at(i).third);
			Vector<double> la1 = (izvor - tocke1->at(indeksiPoligona1->at(i).first)).normalized();
			Vector<double> la2 = (izvor - tocke1->at(indeksiPoligona1->at(i).second)).normalized();
			Vector<double> la3 = (izvor - tocke1->at(indeksiPoligona1->at(i).third)).normalized();

			double id1 = ia * ka + std::max(ii * kd * la1 * na1, 0.0);
			double id2 = ia * ka + std::max(ii * kd * la2 * na2, 0.0);
			double id3 = ia * ka + std::max(ii * kd * la3 * na3, 0.0);
			glBegin(GL_TRIANGLES);
			glColor3ub(id1, 0, 0);	glVertex3f(poligoni1->at(i).first->vector[0], poligoni1->at(i).first->vector[1], poligoni1->at(i).first->vector[2]);
			glColor3ub(id2, 0, 0);	glVertex3f(poligoni1->at(i).second->vector[0], poligoni1->at(i).second->vector[1], poligoni1->at(i).second->vector[2]);
			glColor3ub(id3, 0, 0);	glVertex3f(poligoni1->at(i).third->vector[0], poligoni1->at(i).third->vector[1], poligoni1->at(i).third->vector[2]);
			glEnd();
		}
	}
}

void constantShading() {
	//getPolygonCoeficients(poligoni1);
	for (int i = 0; i < poligoni1->size(); i++) {
		Vector<double> normala({ poligoni1->at(i).a,poligoni1->at(i).b, poligoni1->at(i).c });
		Vector<double> srediste = (1.0 / 3.0) * (*poligoni1->at(i).first)
			+ (1.0 / 3.0) * (*poligoni1->at(i).second)
			+ (1.0 / 3.0) * (*poligoni1->at(i).third);
		Vector<double> ociste({ ociste.x, ociste.y, ociste.z });
		Vector<double> b = ociste - srediste;
		Vector<double> l = izvor - srediste;

		if (normala * b >= 0.0) {
			double ia = 50.0;
			double ka = 0.5;
			Vector<double> la = l.normalized();
			Vector<double> na = normala.normalized();

			double ii = 200.0;
			double kd = 0.5;
			double id = ia * ka + ii * kd * std::max(la * na, 0.0);

			glBegin(GL_TRIANGLES);
			glColor3ub(id, 0, 0);
			glVertex3d(poligoni1->at(i).first->vector[0], poligoni1->at(i).first->vector[1], poligoni1->at(i).first->vector[2]);
			glColor3ub(id, 0, 0);
			glVertex3d(poligoni1->at(i).second->vector[0], poligoni1->at(i).second->vector[1], poligoni1->at(i).second->vector[2]);
			glColor3ub(id, 0, 0);
			glVertex3d(poligoni1->at(i).third->vector[0], poligoni1->at(i).third->vector[1], poligoni1->at(i).third->vector[2]);
			glEnd();
		}
	}
}

void myObject ()
{
	if (typeShade == 'g') {
		gouraud();
	}
	else {
		constantShading();
	}
}

void myMouse2(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		ociste.x=0;
		updatePerspective();
		glutPostRedisplay();
	}
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
	switch (theKey) 
	{
		case 'l': ociste.x = ociste.x+0.1;
        break;

		case 'k': ociste.x =ociste.x-0.1;
        break;

		case 'r': ociste.x=0.0;
        break;

		case 27:  exit(0);
		break;
	}
	updatePerspective();
	glutPostRedisplay();
}

