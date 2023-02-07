#include "Vector.h"
#include "Matrix.h"
#include <iostream>
#include <vector>

int main() {
	Vector<int> vl({ 2, 3, 4 });
	Vector<int> vr({ -1, 4,-1 });
	Vector<int> v1 = vl + vr;
	std::cout << " v1 = (2i + 3j + 4k) + (-i + 4j - k)" << std::endl;
	std::cout << " v1: ";
	std::cout << v1 << std::endl;
	std::cout << std::endl;
	
	std::cout << " s = v1 * (-i + 4j - k)" << std::endl;
	std::cout << " s : ";
	std::cout << v1 * vr << std::endl;
	std::cout << std::endl;

	Vector<int> v2 = Vector<int>::crossProduct3D(&v1, new Vector<int>({ 2,2,4 }));
	std::cout << " v2 = v1 x (2i + 2j + 4k)" << std::endl;
	std::cout << " v2: ";
	std::cout << v2 << std::endl;
	std::cout << std::endl;

	Vector<double> v3 = v2.normalized();
	std::cout << " v3 = |v2|" << std::endl;
	std::cout << " v3: ";
	std::cout << v3 << std::endl;
	std::cout << std::endl;

	Vector<int> v4 = -v2;
	std::cout << " v4 = -v2" << std::endl;
	std::cout << " v4: ";
	std::cout << v4 << std::endl;
	std::cout << std::endl;
	Matrix<int> m1({ {1,2,3},
					{2,1,3},
					{4,5,1} });

	Matrix<int> m2({ {-1,2,-3},
					{5,-2,7},
					{-4,-1,3 } });
	Matrix<int> m3 = m1 + m2;
	std::cout << "M1:" << std::endl;
	std::cout << m3 << std::endl;

	Matrix<int> m4 = m1 * m2.transponate();
	std::cout << "M2:" << std::endl;
	std::cout << m4 << std::endl;
	Matrix<double> m6({ {1,2,3 }, 
						{ 2,1,3 }, 
						{ 4,5,1 }});
	Matrix<double> m5 = Matrix<int>::getInverse(m2);
	std::cout << "M3:" << std::endl;
	std::cout << m6 * m5 << std::endl;

	return 0;
}
