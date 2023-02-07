#include "Vector.h"
#include "Matrix.h"
#include "Polygon.h"

void getPolygonCoeficients(std::vector<Poligon>* polygons) {
	for (int i = 0; i < polygons->size(); i++) {
		double x1 = (*polygons)[i].first->vector[0] / (*polygons)[i].first->vector[3];
		double x2 = (*polygons)[i].second->vector[0] / (*polygons)[i].second->vector[3];
		double x3 = (*polygons)[i].third->vector[0] / (*polygons)[i].third->vector[3];

		double y1 = (*polygons)[i].first->vector[1] / (*polygons)[i].first->vector[3];
		double y2 = (*polygons)[i].second->vector[1] / (*polygons)[i].second->vector[3];
		double y3 = (*polygons)[i].third->vector[1] / (*polygons)[i].third->vector[3];

		double z1 = (*polygons)[i].first->vector[2] / (*polygons)[i].first->vector[3];
		double z2 = (*polygons)[i].second->vector[2] / (*polygons)[i].second->vector[3];
		double z3 = (*polygons)[i].third->vector[2] / (*polygons)[i].third->vector[3];
		(*polygons)[i].a = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
		(*polygons)[i].b = -(x2 - y1) * (z3 - z1) + (z2 - z1) * (x3 - x1);
		(*polygons)[i].c = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
		(*polygons)[i].d = -x1 * (*polygons)[i].a - y1 *
			(*polygons)[i].b - z1 * (*polygons)[i].c;
	}
}

void getMinCoordiantes(double* xmin, double* ymin, double* zmin, std::vector<Vector<double>>* vertices) {
	for (int i = 0; i < vertices->size(); i++) {
		Vector<double> current = vertices->at(i);
		if (i == 0) {
			*xmin = current.vector[0];
			*ymin = current.vector[1];
			*zmin = current.vector[2];
		}

		if (current.vector[0] < *xmin) {
			*xmin = current.vector[0];
		}
		if (current.vector[1] < *ymin) {
			*ymin = current.vector[1];
		}
		if (current.vector[2] < *zmin) {
			*zmin = current.vector[2];
		}
	}
}

void getMaxCoordiantes(double* xmax, double* ymax, double* zmax, std::vector<Vector<double>>* vertices) {
	for (int i = 0; i < vertices->size(); i++) {
		Vector<double> current = vertices->at(i);
		if (i == 0) {
			*xmax = current.vector[0];
			*ymax = current.vector[1];
			*zmax = current.vector[2];
		}

		if (current.vector[0] > *xmax) {
			*xmax = current.vector[0];
		}
		if (current.vector[1] > *ymax) {
			*ymax = current.vector[1];
		}
		if (current.vector[2] > *zmax) {
			*zmax = current.vector[2];
		}
	}
}

void getCenter(double* xmin, double* xmax, double* ymin, double* ymax,
	double* zmin, double* zmax, double* xcenter, double* ycenter, double* zcenter) {
	*xcenter = (*xmax + *xmin) / 2;
	*ycenter = (*ymax + *ymin) / 2;
	*zcenter = (*zmax + *zmin) / 2;
}

double getScalingFactor(double* xmin, double* xmax, double* ymin, double* ymax,
	double* zmin, double* zmax) {
	double x = (*xmax - *xmin) / 2;
	double y = (*ymax - *ymin) / 2;
	double z = (*zmax - *zmin) / 2;
	double max = std::max(x, y);
	max = std::max(max, z);
	return 1.0 / max;
}