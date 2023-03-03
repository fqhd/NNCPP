#pragma once
#include "Matrix.hpp"

#define EULER_NUMBER_F 2.71828182846f

float sigmoid(float x) {
    return (1 / (1 + powf(EULER_NUMBER_F, -x)));
}

float dsigmoid(float y) {
	return y * (1.0f - y);
}

class Dense {
public:

    Dense(int numInputs, int numNodes) {
		weights = Matrix(numNodes, numInputs);
		biases = Matrix(numNodes, 1);
		activations = Matrix(numNodes, 1);
    }

    const Matrix& run(const Matrix& inputs) {
		activations = weights * inputs + biases;
		activations.map(sigmoid);
        return activations;
    }

    Matrix activations;
    Matrix weights;
    Matrix biases;

};