#pragma once

#include "Matrix.h"
#include "Vector.h"

Matrix<double> getPerspectiveProjection(Vector<double>* ociste, Vector<double>* glediste);
void applyProjection(std::vector<Vector<double>>* vertices, Matrix<double>* projectionMatrix);