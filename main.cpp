#include <iostream>
#include "NeuralNetwork.hpp"
#include <chrono>

int main() {

	NeuralNetwork nn;
	nn.setNumInputs(32);
	nn.add(24);
	nn.add(16);
	nn.add(16);
	nn.add(8);
	nn.add(4);
	nn.add(1);
	nn.compile(0.01f);

	Matrix m(32, 1);

	auto start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < 500; i++) {
		nn.predict(m);
	}
	std::chrono::duration<float> dur = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Time: " << dur.count() << " seconds" << std::endl;

    return 0;
}