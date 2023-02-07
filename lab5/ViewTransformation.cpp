#include "Vector.h"
#include "Matrix.h"

Matrix<double> getT1(Vector<double>* vertex) {
	Matrix<double> matricaTranslacije({
	{ 1.0,0,0.0,0.0 },
	{ 0.0,1.0,0.0,0.0 },
	{ 0.0,0.0,1.0,0.0 },
	{-(vertex->getElement(0)),-(vertex->getElement(1)),-(vertex->getElement(2)), 1} });
	return matricaTranslacije;
}

Matrix<double> getT2(Vector<double>* vertex) {
	double x1 = vertex->vector[0];
	double y1 = vertex->vector[1];
	double denominator = sqrt(pow(x1, 2) + pow(y1, 2));
	double sinl = y1 / denominator;
	double cosl = x1 / denominator;
	if (denominator == 0) {
		Matrix<double> t({
			{1.0, 0, 0.0, 0.0},
			{0, 1.0, 0.0, 0.0},
			{0.0, 0.0, 1.0, 0.0},
			{0.0, 0.0, 0.0, 1.0}
			});
		return t;
	}
	Matrix<double> t2({
		{cosl, -sinl, 0.0, 0.0},
		{sinl, cosl, 0.0, 0.0},
		{0.0, 0.0, 1.0, 0.0},
		{0.0, 0.0, 0.0, 1.0} });
	return t2;
}

Matrix<double> getT3(Vector<double>* vertex) {
	double x2 = vertex->vector[0];
	double z2 = vertex->vector[2];
	double denominator = sqrt(pow(x2, 2) + pow(z2, 2));
	double sinb = x2 / denominator;
	double cosb = z2 / denominator;
	if (denominator == 0) {
		Matrix<double> t({
			{1.0, 0, 0.0, 0.0},
			{0, 1.0, 0.0, 0.0},
			{0.0, 0.0, 1.0, 0.0},
			{0.0, 0.0, 0.0, 1.0}
			});
		return t;
	}
	Matrix<double> t3({
		{cosb, 0.0, sinb, 0.0},
		{0.0, 1.0, 0.0, 0.0},
		{-sinb, 0.0, cosb, 0.0},
		{0.0, 0.0, 0.0, 1.0} });
	return t3;
}

Matrix<double> getT4() {
	Matrix<double> t4({
		{1.0, 0.0, 0.0, 0.0},
		{0.0, 1.0, 0.0, 0.0},
		{0.0, 0.0, 1.0, 0.0},
		{0.0, 0.0, 0.0, 1.0} });
	return t4;
}

Matrix<double> getT5() {
	Matrix<double> t5({
		{-1.0, 0.0, 0.0, 0.0},
		{0.0, 1.0, 0, 0.0},
		{0.0, 0.0, 1.0, 0.0},
		{0.0, 0.0, 0.0, 1.0} });
	return t5;
}

