#pragma once
#ifndef AI_H
#define AI_H

#include "stdafx.h"
#include <vector>

using namespace std;

class Neuron;

typedef vector<Neuron> Layer;

struct Connection {
	double weight;
	double delta_weight;
};

// class Net
class Net {
private:
	vector<Layer> Layers;
public:
	Net() {}
	Net(const vector<unsigned> &topology);

	void feedForward(const vector<double> &inputs);
	void backProp(const vector<double> &targetVal);

	void saveWeight(string name_file);
	void loadWeight(string name_file);

	void getResult(const vector<double> &inputs, const vector<double> &targetVals);
	double getOutput();
	double getLearningRate();

	void setLearningRate(double learnRate);
};

// class Neuron
class Neuron {
private:
	double sum;
	double value;
	int    index;
	int    amount_of_connection = 0;
	double gradient;
	vector<Connection> connection;
public:
	Neuron(int amount_of_connection, int index);

	void feedForward(Layer &prevLayer);
	void updateWeight();

	void saveWeight(string name_file);
	void loadWeight(vector<double> &weight);

	double activationFunc(double sum);
	double derivativeActivationFunc(double sum);

	void calcOutputGradient(double targetVal);
	void calcHiddenGradient(Layer &nextLayer);
	void calcDeltaWeight(Layer & nextLayer);
	double sumGradient(Layer &layer);

	void setVal(double value) { this->value = value; }
	double getVal() { return value; }
};


#endif 
