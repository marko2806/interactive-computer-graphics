#include <string>
#include <fstream>
#include <vector>
#include "Vector.h"
#include "Matrix.h"
#include "Polygon.h"
#include "Transformation.h"

void loadPredefinedObject(std::string name, std::vector<Vector<double>> *tocke, std::vector<Poligon> *poligoni, std::vector<PoligonIndeksi> *indeksiPoligona) {
	std::ifstream ObjectFile(name);
	std::string line;
	if (ObjectFile.is_open()) {
		//iscitavanje podataka o tockama i poligonima
		while (ObjectFile.good()) {
			std::string name;
			ObjectFile >> name;
			if (name == "v") {
				double x, y, z;
				ObjectFile >> x >> y >> z;
				tocke->push_back(Vector<double>({ x,y,z, 1.0 }));
			}
			else if (name == "f") {
				int a, b, c;
				ObjectFile >> a >> b >> c;
				Poligon poligon;
				poligon.first = &(tocke->at(a - 1));
				poligon.second = &(tocke->at(b - 1));
				poligon.third = &(tocke->at(c - 1));
				PoligonIndeksi indeksi;
				indeksi.first = a - 1;
				indeksi.second = b - 1;
				indeksi.third = c - 1;
				poligoni->push_back(poligon);
				indeksiPoligona->push_back(indeksi);
			}
		}
		getPolygonCoeficients(poligoni);
		double xMin, xMax, yMin, yMax, zMin, zMax;
		double xcenter, ycenter, zcenter;

		getMinCoordiantes(&xMin, &yMin, &zMin, tocke);
		getMaxCoordiantes(&xMax, &yMax, &zMax, tocke);

		//postavljanje sredista tijela u ishodiste
		getCenter(&xMin, &xMax, &yMin, &yMax, &zMin, &zMax, &xcenter, &ycenter, &zcenter);
		Matrix<double> translacija({
			{ 1,0,0,0 },
			{ 0,1,0,0 },
			{ 0,0,1,0 },
			{ -xcenter,-ycenter,-zcenter,1 }
			});
		transformVertices(*tocke, &translacija);

		//skaliranje tijela tako da se tijelo nalazi unutar raspona [-1,1]x[-1,1]x[-1,1]
		float faktorSkaliranja = getScalingFactor(&xMin, &xMax, &yMin, &yMax, &zMin, &zMax);
		Matrix<double> skaliranje({
			{ faktorSkaliranja,0,0,0 },
			{ 0, faktorSkaliranja,0,0 },
			{ 0,0, faktorSkaliranja,0 },
			{ 0,0,0,1 }
			});

		transformVertices(*tocke, &skaliranje);
		//getPolygonCoeficients1(poligoni);

	}
}