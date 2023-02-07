#include "Transformation.h"
#include "Vector.h"
#include "Matrix.h"

//primjena matrice transformaicije nad jednom tockom
void transformVertex(Vector<double>* vertex, Matrix<double>* transformationMatrix) {
	*vertex = (*vertex) * (*transformationMatrix);
}

//transforimiranje svih tocaka odredenom matricom transformacije
void transformVertices(std::vector<Vector<double>>& vertices, Matrix<double>* transformationMatrix) {
	for (int i = 0; i < vertices.size(); i++) {
		transformVertex(&vertices[i], transformationMatrix);
	}
}


void transformAndProjectVertices(std::vector<Vector<double>>* tocke, Matrix<double> matricaPogleda, Matrix<double> matricaProjekcije) {
	for (int i = 0; i < tocke->size(); i++) {
		Vector<double>* vertex = &(tocke->at(i));
		*vertex = (*vertex) * matricaPogleda;
		*vertex = (*vertex) * matricaProjekcije;
	}
}