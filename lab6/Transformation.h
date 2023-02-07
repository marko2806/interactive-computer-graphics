#pragma once
#include "Vector.h"
#include "Matrix.h"


//primjena matrice transformaicije nad jednom tockom
void transformVertex(Vector<double>* vertex, Matrix<double>* transformationMatrix);

//transforimiranje svih tocaka odredenom matricom transformacije
void transformVertices(std::vector<Vector<double>>& vertices, Matrix<double>* transformationMatrix);

void transformAndProjectVertices(std::vector<Vector<double>>* tocke, Matrix<double> matricaPogleda, Matrix<double> matricaProjekcije);