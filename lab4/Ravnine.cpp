#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <GL/freeglut.h>



GLuint window1;
GLuint width1 = 500, height1 = 500;

//struktira tocke
struct Vertex {
	double x;
	double y;
	double z;
	double h = 1.0;
}typedef Vertex;
//struktura poligona
struct Poligon {
	Vertex* first;
	Vertex* second;
	Vertex* third;
	float a;
	float b;
	float c;
	float d;
}typedef Poligon;


void getPolygonCoeficients(std::vector<Poligon> *polygons) {
	for (int i = 0; i < polygons->size(); i++) {
		float x1 = (*polygons)[i].first->x / (*polygons)[i].first->h;
		float x2 = (*polygons)[i].second->x / (*polygons)[i].second->h;
		float x3 = (*polygons)[i].third->x / (*polygons)[i].third->h;

		float y1 = (*polygons)[i].first->y / (*polygons)[i].first->h;
		float y2 = (*polygons)[i].second->y / (*polygons)[i].second->h;
		float y3 = (*polygons)[i].third->y / (*polygons)[i].third->h;

		float z1 = (*polygons)[i].first->z / (*polygons)[i].first->h;
		float z2 = (*polygons)[i].second->z / (*polygons)[i].second->h;
		float z3 = (*polygons)[i].third->z / (*polygons)[i].third->h;
		(*polygons)[i].a = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
		(*polygons)[i].b = -(x2 - y1) * (z3 - z1) + (z2 - z1) * (x3 - x1);
		(*polygons)[i].c = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
		(*polygons)[i].d = -x1 * (*polygons)[i].a - y1 *
			(*polygons)[i].b - z1 * (*polygons)[i].c;
	}
}

void getMinCoordiantes(float* xmin, float* ymin, float* zmin, std::vector<Vertex>* vertices) {
	for (int i = 0; i < vertices->size(); i++) {
		Vertex current = vertices->at(i);
		if (i == 0) {
			*xmin = current.x;
			*ymin = current.y;
			*zmin = current.z;
		}
		
		if (current.x < *xmin) {
			*xmin = current.x;
		}
		if (current.y < *ymin) {
			*ymin = current.y;
		}
		if (current.z < *zmin) {
			*zmin = current.z;
		}
	}
}

void getMaxCoordiantes(float* xmax, float* ymax, float* zmax, std::vector<Vertex>* vertices) {
	for (int i = 0; i < vertices->size(); i++) {
		Vertex current = vertices->at(i);
		if (i == 0) {
			*xmax = current.x;
			*ymax = current.y;
			*zmax = current.z;
		}

		if (current.x > *xmax) {
			*xmax = current.x;
		}
		if (current.y > *ymax) {
			*ymax = current.y;
		}
		if (current.z > *zmax) {
			*zmax = current.z;
		}
	}
}

void getCenter(float* xmin, float* xmax, float* ymin, float* ymax,
	float* zmin, float* zmax, float* xcenter, float* ycenter, float* zcenter) {
	*xcenter = (*xmax + *xmin) / 2;
	*ycenter = (*ymax + *ymin) / 2;
	*zcenter = (*zmax + *zmin) / 2;
}

float getScalingFactor(float* xmin, float* xmax, float* ymin, float* ymax,
	float* zmin, float* zmax) {
	float x = (*xmax - *xmin) / 2;
	float y = (*ymax - *ymin) / 2;
	float z = (*zmax - *zmin) / 2;
	float max = std::max(x, y);
	max = std::max(max, z);
	return 1.0 / max;
}

//Funkcija koja provjerava i ispisuje polozaj ispitne tocke u odnosu na tijelo. Funkcija ispisuje ispravan rezultat samo za konveksna tijela
void ispisiPolozajTocke(Vertex tocka, std::vector<Poligon>* polygons) {
	bool izvan = false;
	//ispitivanje odnosa tocke i svakog poligona. Ako se tocka nalazi ispod svih poligona, onda je unutar tijela.
	for (int i = 0; i < polygons->size(); i++) {
		float sum = (*polygons)[i].a * tocka.x + (*polygons)[i].b * tocka.y
			+ (*polygons)[i].c * tocka.z + (*polygons)[i].d;
		if (sum > 0) {
			izvan = true;
			break;
		}
	}
	//ispis polozaja tocke
	if (izvan) {
		printf("Tocka je izvan tijela\n");
	}
	else {
		printf("Tocka je unutar tijela\n");
	}
}

//primjena matrice transformaicije nad jednom tockom
void transformVertex(Vertex* vertex, double** transformationMatrix) {
	vertex->x = vertex->x * transformationMatrix[0][0]
		+ vertex->y * transformationMatrix[1][0]
		+ vertex->z * transformationMatrix[2][0]
		+ vertex->h * transformationMatrix[3][0];
	vertex->y = vertex->x * transformationMatrix[0][1]
		+ vertex->y * transformationMatrix[1][1]
		+ vertex->z * transformationMatrix[2][1]
		+ vertex->h * transformationMatrix[3][1];
	vertex->z = vertex->x * transformationMatrix[0][2]
		+ vertex->y * transformationMatrix[1][2]
		+ vertex->z * transformationMatrix[2][2]
		+ vertex->h * transformationMatrix[3][2];
	vertex->h = vertex->x * transformationMatrix[0][3]
		+ vertex->y * transformationMatrix[1][3]
		+ vertex->z * transformationMatrix[2][3]
		+ vertex->h * transformationMatrix[3][3];
}

//transforimiranje svih tocaka odredenom matricom transformacije
void transformVertices(std::vector<Vertex>& vertices, double** transformationMatrix) {
	for (int i = 0; i < vertices.size(); i++) {
		transformVertex(&vertices[i], transformationMatrix);
	}
}

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


//"lista" svih poligona tijela
std::vector<Poligon>* poligoni;
void displayHandler();
void reshapeHandler(GLsizei width, GLsizei height);

int main(int argc, char** argv) {
	//datoteka s podacima se zadaje kao argument prilikom pokretanja programa
	if (argc >= 2) {

		std::ifstream ObjectFile(argv[1]);
		std::string line;
		std::vector<Vertex>* tocke = new std::vector<Vertex>();
		poligoni = new std::vector<Poligon>();
		if (ObjectFile.is_open()) {
			//iscitavanje podataka o tockama i poligonima
			while (ObjectFile.good()) {
				std::string name;
				ObjectFile >> name;
				if (name == "v") {
					double x, y, z;
					ObjectFile >> x >> y >> z;
					tocke->push_back(Vertex{ x,y,z });
				}
				else if (name == "f") {
					float a, b, c;
					ObjectFile >> a >> b >> c;
					Poligon poligon;
					poligon.first = &(tocke->at(a - 1));
					poligon.second = &(tocke->at(b - 1));
					poligon.third = &(tocke->at(c - 1));
					poligoni->push_back(poligon);
				}
			}
			
			float xMin, xMax, yMin, yMax, zMin, zMax;
			float xcenter, ycenter, zcenter;
			getMinCoordiantes(&xMin, &yMin, &zMin, tocke);
			getMaxCoordiantes(&xMax, &yMax, &zMax, tocke);

			//postavljanje sredista tijela u ishodiste
			getCenter(&xMin, &xMax, &yMin, &yMax, &zMin, &zMax, &xcenter, &ycenter, &zcenter);
			double** translacija = new double* [4]();
			translacija[0] = new double[4]{ 1,0,0,0 };
			translacija[1] = new double[4]{ 0,1,0,0 };
			translacija[2] = new double[4]{ 0,0,1,0 };
			translacija[3] = new double[4]{ -xcenter,-ycenter,-zcenter,1 };
			transformVertices(*tocke, translacija);

			//skaliranje tijela tako da se tijelo nalazi unutar raspona [-1,1]x[-1,1]x[-1,1]
			float faktorSkaliranja = getScalingFactor(&xMin, &xMax, &yMin, &yMax, &zMin, &zMax);
			double** skaliranje = new double* [4]();
			skaliranje[0] = new double[4]{ faktorSkaliranja,0,0,0 };
			skaliranje[1] = new double[4]{ 0, faktorSkaliranja,0,0 };
			skaliranje[2] = new double[4]{ 0,0, faktorSkaliranja,0 };
			skaliranje[3] = new double[4]{ 0,0,0,1 };

			transformVertices(*tocke, skaliranje);
			getPolygonCoeficients(poligoni);
			//Unos ispitne tocke
			double x, y, z;
			std::cout << "Unesite koordinate ispitne tocke" << std::endl;
			std::cin >> x;
			std::cin >> y;
			std::cin >> z;
			//Ispis polazaja tocke u odnosu na tijelo
			Vertex tocka;
			tocka.x = x;
			tocka.y = y;
			tocka.z = z;
			ispisiPolozajTocke(tocka, poligoni);

			//Graficki prikaz tijela
			glutInit(&argc, argv);
			glutInitDisplayMode(GLUT_DOUBLE);
			glutInitWindowSize(height1, width1);
			glutInitWindowPosition(50, 50);
			window1 = glutCreateWindow("Objekt");
			glutDisplayFunc(displayHandler);
			glutReshapeFunc(reshapeHandler);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClearDepth(1.0f);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glShadeModel(GL_SMOOTH);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glFlush();
			glutMainLoop();

		}
	}
	return 0;
}

//iscrtavanje tijela
void drawPolygon(Poligon polygon) {
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(polygon.first->x, polygon.first->y, polygon.first->z);
	glVertex3f(polygon.second->x, polygon.second->y, polygon.second->z);
	glVertex3f(polygon.third->x, polygon.third->y, polygon.third->z);
	glEnd();
}

void displayHandler() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);
	//glRotatef(-15, 0.0f, 1.0f, 0.0f);
	//glRotatef(-15, .0f, 1.0f, 1.0f);
	drawCoordinates();
	for (int i = 0; i < poligoni->size(); i++) {
		drawPolygon(poligoni->at(i));
	}

	glutSwapBuffers();
}

void reshapeHandler(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

