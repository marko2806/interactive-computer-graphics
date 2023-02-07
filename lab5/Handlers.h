#pragma once
#include <GL/freeglut.h>

void initOpenGl(int argc, char** argv, GLuint height, GLuint width, GLuint *window);
void myDisplay();
void myReshape(int w, int h);