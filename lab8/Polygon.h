#pragma once
struct Poligon {
	Vector<double>* first;
	Vector<double>* second;
	Vector<double>* third;
	double a;
	double b;
	double c;
	double d;
}typedef Poligon;

struct PoligonIndeksi {
	int first;
	int second;
	int third;
};

void getPolygonCoeficients(std::vector<Poligon>* polygons);

void getMinCoordiantes(double* xmin, double* ymin, double* zmin, std::vector<Vector<double>>* vertices);

void getMaxCoordiantes(double* xmax, double* ymax, double* zmax, std::vector<Vector<double>>* vertices);

void getCenter(double* xmin, double* xmax, double* ymin, double* ymax,
	double* zmin, double* zmax, double* xcenter, double* ycenter, double* zcenter);

double getScalingFactor(double* xmin, double* xmax, double* ymin, double* ymax,
	double* zmin, double* zmax);