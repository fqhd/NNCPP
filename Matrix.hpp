#pragma once
#include <cstdint>
#include <random>
#include <iostream>
#include "Random.hpp"

class Matrix {
public:

	Matrix() {}

	Matrix(int r, int c) {
		m_numRows = r;
		m_numCols = c;
		m_data = new float[r * c];
		for(int i = 0; i < r * c; i++) {
			m_data[i] = Random::uniformFloat(-1.0f, 1.0f);
		}
	}

	Matrix(int r, int c, float v) {
		m_numRows = r;
		m_numCols = c;
		m_data = new float[r * c];
		for(int i = 0; i < r * c; i++) {
			m_data[i] = v;
		}
	}

	Matrix(const Matrix& other) {
		if(m_data) delete[] m_data;
		m_numRows = other.m_numRows;
		m_numCols = other.m_numCols;
		m_data = new float[m_numRows * m_numCols];
		memcpy(m_data, other.m_data, sizeof(float) * m_numRows * m_numCols);
	}

	void operator=(const Matrix& other) {
		if(m_data) delete[] m_data;
		m_numRows = other.m_numRows;
		m_numCols = other.m_numCols;
		m_data = new float[m_numRows * m_numCols];
		memcpy(m_data, other.m_data, sizeof(float) * m_numRows * m_numCols);
	}

	Matrix operator*(const Matrix& other) {
		Matrix result;
		result.m_numRows = m_numRows;
		result.m_numCols = other.m_numCols;
		result.m_data = new float[m_numRows * other.m_numCols];
		if(m_numCols != other.m_numRows) {
			std::cout << "Matrix: cannot matrix multiply matrices" << std::endl;
			for(int i = 0; i < result.m_numRows * result.m_numCols; i++) result.m_data[i] = 0.0f;
			return result;
		}
		for(int i = 0; i < m_numRows; i++) {
			for(int j = 0; j < other.m_numCols; j++) {
				float sum = 0.0f;
				for(int k = 0; k < m_numCols; k++) {
					sum += m_data[i * m_numCols + k] * other.m_data[k * other.m_numCols + j];
				}
				result.m_data[i * other.m_numCols + j] = sum;
			}
		}
		return result;
	}

	void mul(const Matrix& other) {
		if(m_numRows != other.m_numRows || m_numCols != other.m_numCols) {
			std::cout << "Matrix: Cannot multiply matrices" << std::endl;
			return;
		}
		for(int i = 0; i < m_numRows * m_numCols; i++) {
			m_data[i] *= other.m_data[i];
		}
	}

	void scale(float s) {
		for(int i = 0; i < m_numRows * m_numCols; i++) {
			m_data[i] *= s;
		}
	}

	void add(const Matrix& other) {
		if(m_numRows != other.m_numRows || m_numCols != other.m_numCols) {
			std::cout << "Matrix: Cannot add matrices" << std::endl;
			return;
		}
		for(int i = 0; i < m_numRows * m_numCols; i++) {
			m_data[i] += other.m_data[i];
		}
	}

	void map(float (*func)(float)){
		for(int i = 0; i < m_numRows * m_numCols; i++) {
			m_data[i] = func(m_data[i]);
		}
	}

	Matrix operator+(const Matrix& other) const {
		if(m_numRows != other.m_numRows || m_numCols != other.m_numCols) {
			std::cout << "Matrix: Cannot add matrices" << std::endl;
			return other;
		}
		Matrix result;
		result.m_numRows = m_numRows;
		result.m_numCols = m_numCols;
		result.m_data = new float[m_numRows * m_numCols];
		for(int i = 0; i < m_numRows * m_numCols; i++) {
			result.m_data[i] = m_data[i] + other.m_data[i];
		}
		return result;
	}

	Matrix operator-(const Matrix& other) const {
		if(m_numRows != other.m_numRows || m_numCols != other.m_numCols) {
			std::cout << "Matrix: Cannot add matrices" << std::endl;
			return other;
		}
		Matrix result;
		result.m_numRows = m_numRows;
		result.m_numCols = m_numCols;
		result.m_data = new float[m_numRows * m_numCols];
		for(int i = 0; i < m_numRows * m_numCols; i++) {
			result.m_data[i] = m_data[i] - other.m_data[i];
		}
		return result;
	}

	Matrix transpose() const {
		Matrix result;
		result.m_numRows = m_numCols;
		result.m_numCols = m_numRows;
		result.m_data = new float[m_numRows * m_numCols];
		for(int i = 0; i < m_numCols; i++) {
			for(int j = 0; j < m_numRows; j++) {
				result.m_data[i * m_numRows + j] = m_data[j * m_numCols + i];
			}
		}
		return result;
	}

	static Matrix fromData(float* data, int numValues) {
		Matrix result;
		result.m_numRows = numValues;
		result.m_numCols = 1;
		result.m_data = new float[numValues];
		memcpy(result.m_data, data, sizeof(float) * numValues);
		return result;
	}

	int numRows() const {
		return m_numRows;
	}

	int numCols() const {
		return m_numCols;
	}

	float* data() const {
		return m_data;
	}

	~Matrix() {
		m_numRows = 0;
		m_numCols = 0;
		if(m_data) delete[] m_data;
	}

private:

	float* m_data = nullptr;
	int m_numRows = 0;
	int m_numCols = 0;

};

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
	for(int i = 0; i < matrix.numRows(); i++) {
		os << "[";
		for(int j = 0; j < matrix.numCols() - 1; j++) {
			os << matrix.data()[i * matrix.numCols() + j] << ", ";
		}
		os << matrix.data()[i * matrix.numCols() + matrix.numCols() - 1] << "]" << std::endl;
	}
	return os;
}