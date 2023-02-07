#pragma once 
#include <iostream>
#include <vector>
#include <math.h>
template <typename T>
class Vector {
public:
	std::vector<T> vector;
	Vector(int n);
	Vector(std::vector<T> vector);
	Vector();
	Vector<double> normalized();
	T getElement(int m);
	static Vector<T> crossProduct3D(Vector<T>* v1, Vector<T>* v2);
};

template <typename T>
Vector<T>::Vector() {
	vector = std::vector<T>(0);
}

template <typename T>
Vector<T>::Vector(int n) {
	vector = std::vector<T>(n);
}

template <typename T>
Vector<T>::Vector(std::vector<T> vector) {
	this->vector = vector;
}

template<typename T>
T Vector<T>::getElement(int m) {
	return this->vector[m];
}

template <typename T>
Vector<double> Vector<T>::normalized() {
	Vector<double> result(this->vector.size());
	double norm = 0.0;
	for (int i = 0; i < this->vector.size(); i++) {
		norm += (double)(this->vector[i]) * (this->vector[i]);
	}
	norm = sqrt(norm);
	for (int i = 0; i < this->vector.size(); i++) {
		result.vector[i] = this->vector[i] / norm;
	}
	return result;
}

template <typename T>
Vector<T> Vector<T>::crossProduct3D(Vector<T>* v1, Vector<T>* v2) {
	if (v1->vector.size() != 3 || v2->vector.size() != 3) {
		return Vector<T>(0);
	}
	Vector<T> result(3);
	if (v1->vector.size() == 3 && v2->vector.size() == 3) {
		result.vector[0] = v1->vector[1] * v2->vector[2] - v1->vector[2] * v2->vector[1];
		result.vector[1] = v1->vector[2] * v2->vector[0] - v1->vector[0] * v2->vector[2];
		result.vector[2] = v1->vector[0] * v2->vector[1] - v1->vector[1] * v2->vector[0];
	}
	return result;
}

template <typename T>
double operator*(const Vector<T>& v1, const Vector<T>& v2) {
	double result = 0.0;
	if (v1.vector.size() == v2.vector.size()) {
		for (int i = 0; i < v1.vector.size(); i++) {
			result = result + ((double)v1.vector[i] * v2.vector[i]);
		}
	}
	return result;
}
template <typename T>
Vector<T> operator*(const double scalar, const Vector<T>& v) {
	Vector<T> result(v.vector.size());
	for (int i = 0; i < v.vector.size(); i++) {
		result.vector[i] = v.vector[i] * scalar;
	}
	return result;
}
template <typename T>
Vector<T> operator*(const Vector<T>& v, const double scalar) {
	Vector<T> result(v.vector.size());
	for (int i = 0; i < v.vector.size(); i++) {
		result.vector[i] = v.vector[i] * scalar;
	}
	return result;
}
template <typename T>
Vector<T> operator-(const Vector<T>& v) {
	Vector<T> result(v.vector.size());
	for (int i = 0; i < v.vector.size(); i++) {
		result.vector[i] = -v.vector[i];
	}
	return result;
}
template <typename T>
Vector<T> operator-(const Vector<T>& v1, const Vector<T>& v2) {
	Vector<T> result(v1.vector.size());
	for (int i = 0; i < v1.vector.size(); i++) {
		result.vector[i] = v1.vector[i] -v2.vector[i];
	}
	return result;
}

template <typename T>
Vector<T> operator+(const Vector<T>& v1, const Vector<T>& v2) {
	Vector<T> result(v1.vector.size());
	if (v1.vector.size() == v2.vector.size()) {
		for (int i = 0; i < v1.vector.size(); i++) {
			result.vector[i] = v1.vector[i] + v2.vector[i];
		}
	}
	return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T> v)
{
	os << "[";
	for (int i = 0; i < v.vector.size(); i++) {
		os << v.vector[i] << " ";
	}
	os << "]";
	return os;
}
