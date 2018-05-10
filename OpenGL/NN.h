#pragma once
#ifndef NN_H
#define NN_H

#include "stdafx.h"
#include <vector>

using namespace std;

const double e = 2.71828182846;

class Net {
private:
	class Neuron {
	public:
		double gradient;
		double value;
		double sum;
		int    index;

		vector<double> weight;
		vector<double> delta_weight;

		Neuron(int num_of_connection, int index);

		static double activation_function(double sum) { return 1 / (1 + pow(e, -sum)); }
		static double derivative_of_act_func(double sum) { return pow(e, -sum) / pow(1 + pow(e, -sum), 2); }
	};

	vector<vector<Neuron>> structure;

public:
	Net(const vector<int> &str);

	void feed_forward(const vector<double> &input_values);
	void back_propagation(const vector<double> &target_values);

	void get_result(const vector<double> &input_values, const vector<double> &target_values);
	double get_output() { return structure.back().front().value; }

	void save_weight(string path);
	void load_weight(string path);
};

#endif