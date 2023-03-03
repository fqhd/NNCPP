#pragma once
#include "Dense.hpp"
#include <vector>

class NeuralNetwork {
public:

	NeuralNetwork() {}

	~NeuralNetwork() {
		for(int i = 0; i < layers.size(); i++) {
			delete layers[i];
		}
	}

	void save(const std::string& path) {
		if(!m_isCompiled) {
			std::cout << "NeuralNetwork: Cannot save a model that isn't compiled" << std::endl;
			return;
		}
		FILE* out = fopen(path.c_str(), "wb");
		if(!out) {
			std::cout << "NeuralNetwork: Failed to open a file for saving the NN" << std::endl;
			return;
		}
		fwrite((void*)(&m_learningRate), sizeof(float), 1, out);
		fwrite((void*)(&m_numInputs), sizeof(int), 1, out);
		int numLayers = layers.size();
		fwrite((void*)(&numLayers), sizeof(int), 1, out);
		fwrite((void*)layerTemplates.data(), sizeof(int), layerTemplates.size(), out);
		for(int i = 0; i < numLayers; i++) {
			fwrite((void*)layers[i]->weights.data(), sizeof(float), layers[i]->weights.numRows() * layers[i]->weights.numCols(), out);
			fwrite((void*)layers[i]->biases.data(), sizeof(float), layers[i]->biases.numRows() * layers[i]->biases.numCols(), out);
		}
		fclose(out);
	}

	void load(const std::string& path) {
		FILE* in = fopen(path.c_str(), "rb");
		if(!in) {
			std::cout << "NeuralNetwork: Failed to open NN file" << std::endl;
			return;
		}
		if(m_isCompiled) {
			std::cout << "NeuralNetwork: Cannot load into a compiled model" << std::endl;
			return;
		}
		m_isCompiled = true;
		
		fread((void*)&m_learningRate, sizeof(float), 1, in);
		fread((void*)&m_numInputs, sizeof(int), 1, in);
		int numLayers;
		layerTemplates.clear();
		fread((void*)&numLayers, sizeof(int), 1, in);
		for(int i = 0; i < numLayers; i++) {
			int numNodes;
			fread((void*)&numNodes, sizeof(int), 1, in);
			layerTemplates.push_back(numNodes);
		}
		int lastNumInputs = m_numInputs;
		for(int i = 0; i < layerTemplates.size(); i++) {
			layers.push_back(new Dense(lastNumInputs, layerTemplates[i]));
			lastNumInputs = layerTemplates[i];
		}
		for(int i = 0; i < numLayers; i++) {
			fread((void*)layers[i]->weights.data(), sizeof(float), layers[i]->weights.numRows() * layers[i]->weights.numCols(), in);
			fread((void*)layers[i]->biases.data(), sizeof(float), layers[i]->biases.numRows() * layers[i]->biases.numCols(), in);
		}
		fclose(in);
	}

	void setNumInputs(int numInputs) {
		m_numInputs = numInputs;
	}

	void add(int numNodes) {
		layerTemplates.push_back(numNodes);
	}

	void compile(float learningRate) {
		m_isCompiled = true;
		if(layerTemplates.size() < 0) {
			std::cout << "NeuralNetwork: compiling a model with 0 layers" << std::endl;
			m_isCompiled = false;
			return;
		}

		if(m_numInputs <= 0) {
			std::cout << "NeuralNetwork: cannot compile a model with no inputs" << std::endl;
			m_isCompiled = false;
			return;
		}

		m_learningRate = 0.01f;
		int lastNumInputs = m_numInputs;
		for(int i = 0; i < layerTemplates.size(); i++) {
			layers.push_back(new Dense(lastNumInputs, layerTemplates[i]));
			lastNumInputs = layerTemplates[i];
		}
	}

	Matrix predict(Matrix inputs) {
		if(inputs.numRows() != m_numInputs) {
			std::cout << "NeuralNetwork: invalid number of inputs" << std::endl;
			return inputs;
		}
		if(!m_isCompiled) {
			std::cout << "NeuralNetwork: you must call .compile() on the model before using it" << std::endl;
			return inputs;
		}
		for(int i = 0; i < layers.size(); i++) {
			inputs = layers[i]->run(inputs);
		}
		return inputs;
	}

	void train(Matrix inputs, Matrix targets) {
		Matrix outputs = predict(inputs);

		Matrix errors = targets - outputs;
		for(int i = layers.size() - 1; i >= 1; i--) {
			Matrix gradients = layers[i]->activations;
			gradients.map(dsigmoid);
			gradients.mul(errors);
			gradients.scale(m_learningRate);

			Matrix prevT = layers[i-1]->activations.transpose();
			Matrix deltas = gradients * prevT;

			layers[i]->weights.add(deltas);
			layers[i]->biases.add(gradients);

			errors = layers[i]->weights.transpose() * errors;
		}

		Matrix gradients = layers[0]->activations;
		gradients.map(dsigmoid);
		gradients.mul(errors);
		gradients.scale(m_learningRate);
		Matrix inpT = inputs.transpose();
		Matrix deltas = gradients * inpT;
		layers[0]->weights.add(deltas);
		layers[0]->biases.add(gradients);
	}

private:
	std::vector<Dense*> layers;
	std::vector<int> layerTemplates;
	int m_numInputs = 0;
	bool m_isCompiled = false;
	float m_learningRate = 0.01f;

};
