#include "Vector.h"
#include "Matrix.h"
#include "Projection.h"

Matrix<double> getPerspectiveProjection(Vector<double>* ociste, Vector<double>* glediste) {
	double dx = pow(ociste->vector[0] - glediste->vector[0], 2);
	double dy = pow(ociste->vector[1] - glediste->vector[1], 2);
	double dz = pow(ociste->vector[2] - glediste->vector[2], 2);
	double h = sqrt(dx + dy + dz);
	Matrix<double> p({
		{1.0, 0.0, 0.0, 0.0},
		{0.0, 1.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 1.0 / h},
		{0.0, 0.0, 0.0, 0.0} });
	return p;
}

void applyProjection(std::vector<Vector<double>>* vertices, Matrix<double>* projectionMatrix) {
	for (int i = 0; i < vertices->size(); i++) {
		Vector<double>* vertex = &(vertices->at(i));
		*vertex = *vertex * (*projectionMatrix);
	}
}