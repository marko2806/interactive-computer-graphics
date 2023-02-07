#include "Matrix.h"
#include <iostream>



int main() {
	std::cout << "Unesite parametre jednadzbi: " << std::endl;
	int arr[12] = {};
	for (int i = 0; i < 12; i++) {
		std::cin >> arr[i];
	}

	int arri[9] = { arr[0], arr[1], arr[2],
		arr[4], arr[5], arr[6],
		arr[8], arr[9], arr[10] };

	Matrix<int> mat({ {arr[0], arr[1], arr[2]},
		{arr[4], arr[5], arr[6] },
		{arr[8], arr[9], arr[10]} });
	
	Matrix<double> matx({ {(double)arr[3]},
		{(double)arr[7]},
		{(double)arr[11]}});
	

	Matrix<double> result = Matrix<int>::getInverse(mat) * matx;
	std::cout << "[x y z] = [" << result.transponate() << "]" << std::endl;
	return 0;
}