#include "Matrix.h"
#include <iostream>

int main() {

	
	double arr[12] = {};
	std::cout << "Unesite koordinate tocke A: ";
	for (int i = 0; i < 3; i++) {
		std::cin >> arr[i];
	}
	std::cout << "Unesite koordinate tocke B: ";
	for (int i = 3; i < 6; i++) {
		std::cin >> arr[i];
	}
	std::cout << "Unesite koordinate tocke C: ";
	for (int i = 6; i < 9; i++) {
		std::cin >> arr[i];
	}
	std::cout << "Unesite koordinate tocke T: ";
	for (int i = 9; i < 12; i++) {
		std::cin >> arr[i];
	}

	Matrix<double> mat({ {arr[0], arr[3],arr[6]},
		{arr[1], arr[4], arr[7]},
		{arr[2], arr[5], arr[8]} });
	Matrix<double> matx({ {(double)arr[9]},
		{(double)arr[10]},
		{(double)arr[11]} });

	Matrix<double> result = Matrix<double>::getInverse(mat) * matx;
	std::cout << "Baricentricne koordinate : ";
	std::cout << "[" << result.transponate() << "]" << std::endl;
	return 0;
}