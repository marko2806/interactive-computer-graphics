#pragma once
#include <vector>
#include <iostream>
#include <math.h>

template <typename T>
class Matrix {
private:
	int m;
	int n;
	static Matrix<T> getSubmatrix(Matrix<T> matrix, int i_sel, int j_sel);
public:
	std::vector<std::vector<T>> matrix;
	Matrix(int m, int n);
	Matrix(std::vector<std::vector<T>> data);
	Matrix<T> transponate();
	static T determinant(Matrix<T> matrix);
	int getM(void) const {
		return m;
	}
	int getN(void) const {
		return n;
	}
	static Matrix<double> getInverse(Matrix<T> matrix);
};

template <typename T>
Matrix<T>::Matrix(int m, int n) {
	this->m = m;
	this->n = n;
	matrix = std::vector<std::vector<T>>(m);
	for (int i = 0; i < matrix.size(); i++) {
		matrix[i] = std::vector<T>(n);
	}
}

template <typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>> data) {
	this->matrix = data;
	this->m = data.size();
	this->n = data[0].size();
}


template <typename T>
Matrix<T> Matrix<T>::transponate() {
	Matrix<T> result(this->n, this->m);
	for (int i = 0; i < this->n; i++) {
		for (int j = 0; j < this->m; j++) {
			result.matrix[i][j] = this->matrix[j][i];
		}
	}
	return result;
}



template <typename T>
Matrix<T> Matrix<T>::getSubmatrix(Matrix<T> matrix, int i_sel, int j_sel) {
	if (matrix.m <= 0 || matrix.n <= 0) {
		return Matrix<T>(0, 0);
	}
	Matrix<T> submatrix(matrix.m - 1, matrix.n - 1);

	int subi = 0;
	for (int i = 0; i < matrix.m; i++) {

		if (i != i_sel) {
			int subj = 0;
			for (int j = 0; j < matrix.n; j++) {
				if (j != j_sel) {
					submatrix.matrix[subi][subj] = matrix.matrix[i][j];
					subj++;
				}

			}
			subi++;
		}
	}
	return submatrix;
}

template <typename T>
Matrix<double> Matrix<T>::getInverse(Matrix<T> matrix) {
	int matrixDet = determinant(matrix);
	if (matrixDet == 0) {
		return Matrix<double>(0,0);
	}
	Matrix<double> result(matrix.getM(), matrix.getN());

	for (int i = 0; i < matrix.getM(); i++) {
		for (int j = 0; j < matrix.getN(); j++) {
			result.matrix[i][j] = (1.0 / matrixDet) * determinant(getSubmatrix(matrix, i, j));
			if ((i + j) % 2 != 0) {
				result.matrix[i][j] = -result.matrix[i][j];
			}
		}
	}
	result = result.transponate();
	return result;
}
template <typename T>
T Matrix<T>::determinant(Matrix<T> matrix) {
	T det = 0;
	Matrix<T> submatrix(matrix.getM() - 1, matrix.getN() - 1);
	if (matrix.getM() == 2)
		return ((matrix.matrix[0][0] * matrix.matrix[1][1]) - (matrix.matrix[1][0] * matrix.matrix[0][1]));
	else {
		for (int x = 0; x < matrix.getN(); x++) {
			int subi = 0;
			for (int i = 1; i < matrix.getN(); i++) {
				int subj = 0;
				for (int j = 0; j < matrix.getN(); j++) {
					if (j == x)
						continue;
					submatrix.matrix[subi][subj] = matrix.matrix[i][j];
					subj++;
				}
				subi++;
			}
			det = det + (pow(-1, x) * matrix.matrix[0][x] * determinant(submatrix));
		}
	}
	return det;
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& m1, const Matrix<T>& m2) {
	if (m1.getN() != m2.getM()) {
		return Matrix<T>(0,0);
	}
	Matrix<T> result(m1.getM(), m2.getN());

	for (int i = 0; i < m1.getM(); i++) {
		for (int j = 0; j < m2.getN(); j++) {
			result.matrix[i][j] = 0;
			for (int k = 0; k < m1.getN(); k++) {
				result.matrix[i][j] += m1.matrix[i][k] * m2.matrix[k][j];
			}
		}
	}
	return result;
}

template <typename T>
Matrix<T> operator+(const Matrix<T>& m1, const Matrix<T>& m2) {
	if (m1.getM() != m2.getM() || m1.getN() != m2.getN()) {
		return Matrix<T>(0,0);
	}
	Matrix<T> result(m1.getM(), m1.getN());
	if (m1.getM() == m2.getM() && m1.getN() == m2.getN()) {
		for (int i = 0; i < m1.getM(); i++) {
			for (int j = 0; j < m1.getN(); j++) {
				result.matrix[i][j] = m1.matrix[i][j] + m2.matrix[i][j];
			}
		}
	}
	return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T> m)
{
	for (int i = 0; i < m.getM(); i++) {
		for (int j = 0; j < m.getN(); j++) {
			os << m.matrix[i][j] << " ";
		}
		if (i != m.getM() - 1) {
			os << std::endl;
		}
	}
	return os;
}

