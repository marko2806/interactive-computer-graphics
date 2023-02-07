//*********************************************************************************
//	Iscrtavanje linije 0-45, usporedba s glLine naredbom.
//	Pocetna i krajnja koordinata se zadaju lijevom tiplom misa, desnom se brise.
//	r, g, b, k s tastature mijenja boju.
//
//	Zadatak: Treba preraditi void myLine(GLint xa, GLint ya, GLint xb, GLint yb)
//	tako da radi ispravno za sve kutove
//
//  Potreban je glut - http://freeglut.sourceforge.net/
//  Za MSVC skinuti: 
//  http://files.transmissionzero.co.uk/software/development/GLUT/freeglut-MSVC.zip
//  postaviti - Properties - Configuration properties VC++Directories - Include Dir
//                                                                    - Library Dir
//*********************************************************************************

#include <stdio.h>
#include <GL/freeglut.h>

//*********************************************************************************
//	Pocetna tocka Lx[1], Ly[1] Krajnja tocke Lx[2], Ly[2] linije.
//	Ix - stanje (zadana 1 ili 2 tocka)
//*********************************************************************************

GLdouble Lx[2], Ly[2];
GLint Ix;

//*********************************************************************************
//	Pokazivac na glavni prozor i pocetna velicina.
//*********************************************************************************

GLuint window;
GLuint width = 300, height = 300;

//*********************************************************************************
//	Function Prototypes.
//*********************************************************************************

void myDisplay();
void myReshape(int width, int height);
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void myLine(GLint xa, GLint ya, GLint xb, GLint yb);


//*********************************************************************************
//	Glavni program.
//*********************************************************************************

int main(int argc, char ** argv)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);
	window = glutCreateWindow("Glut OpenGL Linija");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	printf("Lijevom tipkom misa zadaj tocke - algoritam Bresenham-a\n");
	printf("Tipke r, g, b, k mijenjaju boju.\n");
		
	glutMainLoop();
	return 0;
}


//*********************************************************************************
//	Osvjezavanje prikaza. (nakon preklapanja prozora) 
//*********************************************************************************

void myDisplay()
{
	//printf("Pozvan myDisplay()\n");
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //  boja pozadine
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //brisanje nakon svake iscrtane linije
	glFlush();
}

//*********************************************************************************
//	Promjena velicine prozora.
//*********************************************************************************

void myReshape(int w, int h)
{
	//printf("Pozvan myReshape()\n");
	width = w; height = h;               //promjena sirine i visine prozora
	Ix = 0;								//	indeks tocke 0-prva 1-druga tocka
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

//*********************************************************************************
//	Crtaj moju liniju.
//*********************************************************************************

void drawMyLine(GLint xa, GLint ya, GLint xb, GLint yb) {
	glBegin(GL_POINTS);
	GLdouble koeficijent = (double)(yb - ya) / (xb - xa);
	GLint x, y;
	GLdouble d = -0.5;
	if (xa >= xb) {
		x = xa; xa = xb; xb = x;
		y = ya; ya = yb; yb = y;
	}
	if (koeficijent <= 1.0 && koeficijent >= 0.0) {
		x = xa; y = ya;
		for (GLint x = xa; x <= xb; x++) {
			glVertex2d(x, y);
			d += koeficijent;
			if (d > 0) {
				y++;
				d -= 1;
			}
		}
	}
	else if (koeficijent > 1.0) {
		x = xa; xa = ya; ya = x;
		x = xb; xb = yb; yb = x;
		x = xa; y = ya;
		koeficijent = (double)(yb - ya) / (xb - xa);
		for (GLint x = xa; x <= xb; x++) {
			glVertex2d(y, x);
			d += koeficijent;
			if (d > 0) {
				y++;
				d -= 1;
			}
		}
	}
	else if (koeficijent < 0.0 && koeficijent >= -1.0) {
		x = xa; y = ya;
		for (GLint x = xa; x <= xb; x++) {
			glVertex2d(x, y);
			d -= koeficijent;
			if (d > 0) {
				y--;
				d -= 1;
			}
		}
	}
	else if (koeficijent < -1.0) {
		x = xa; xa = ya; ya = x;
		x = xb; xb = yb; yb = x;
		x = xa; y = yb;
		koeficijent = (double)(yb - ya) / (xb - xa);
		for (GLint x = xb; x <= xa; x++) {
			glVertex2d(y, x);
			d -= koeficijent;
			if (d > 0) {
				y --;
				d -= 1;
			}
		}
	}

	glEnd();
}

void myLine(GLint xa, GLint ya, GLint xb, GLint yb)
{
	glBegin(GL_LINES);
	{
		glVertex2i(xa, ya + 10);			//	crtanje gotove linije
		glVertex2i(xb, yb + 10);
	}
	glEnd();
	drawMyLine(xa, ya, xb, yb);
	
}

//*********************************************************************************
//	Mis.
//*********************************************************************************

void myMouse(int button, int state, int x, int y)
{
	//	Lijeva tipka - crta pocetnu tocku ili liniju.
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)

	{
		//	Pamti krajnju tocke linije.
		Lx[Ix] = x;							//	upisi tocku
		Ly[Ix] = height - y;
		Ix = Ix ^ 1;						//	flip - druga tocka

											//	Crta prvu tocku ili liniju do druge tocke.
		if (Ix == 0)	myLine((int)Lx[0], (int)Ly[0], (int)Lx[1], (int)Ly[1]);
		else			glVertex2i(x, height - y);

		printf("Koordinate tocke %d: %d %d \n", Ix ^ 1, x, y);

		glFlush();
	}

	//	Desna tipka - brise canvas. 
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		myReshape(width, height);
	}
}


//*********************************************************************************
//	Tastatura tipke - r, g, b, k - mijenjaju boju.
//*********************************************************************************

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
	switch (theKey)
	{
	case 'r':
		glColor3f(1, 0, 0);
		break;

	case 'g':
		glColor3f(0, 1, 0);
		break;

	case 'b':
		glColor3f(0, 0, 1);
		break;

	case 'k':
		glColor3f(0, 0, 0);

	}
	glRecti(width - 15, height - 15, width, height); // crta mali kvadrat u boji
	glFlush();
}
