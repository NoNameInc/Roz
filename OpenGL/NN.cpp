#include "stdafx.h"
#include "NN.h"

#include <iostream>
#include <windows.h>
#include <cmath>
#include <ctime>
#include <fstream>
#include <vector>

using namespace std;

double learning_rate = 2;

Net::Net(const vector<int>& str) {
	int num_of_layers = str.size();

	for (int i = 0; i < num_of_layers; i++) {
		structure.push_back(vector<Neuron>());
		int num_of_connection = 0;
		if (i != str.size() - 1) num_of_connection = str[i + 1];

		for (int j = 0; j <= str[i]; j++) {
			structure.back().push_back(Neuron(num_of_connection, j));
		}
		structure.back().back().value = 1.0;
	}
}

void Net::feed_forward(const vector<double> &input_values) {
	for (int i = 0; i < input_values.size(); i++) {
		structure[0][i].value = input_values[i];
	}

	for (int i = 1; i < structure.size(); i++) {
		vector<Neuron> &current_layer = structure[i];
		vector<Neuron> &prev_layer = structure[i - 1];

		for (int j = 0; j < current_layer.size() - 1; j++) {
			double sum = 0.0;
			for (int k = 0; k < prev_layer.size(); k++) {
				sum += prev_layer[k].value * prev_layer[k].weight[current_layer[j].index];
			}
			current_layer[j].value = Neuron::activation_function(sum);
			current_layer[j].sum = sum;
		}
	}
}

void Net::back_propagation(const vector<double>& target_values) {
	for (int i = 0; i < target_values.size(); i++) {
		structure.back()[i].gradient = (target_values[i] - structure.back()[i].value) *
											Neuron::derivative_of_act_func(structure.back()[i].sum);
	}

	for (int i = structure.size() - 1; i > 0; i--) {
		vector<Neuron> &current_layer = structure[i];
		vector<Neuron> &prev_layer = structure[i - 1];

		for (int j = 0; j < prev_layer.size(); j++) {
			double sum_of_gradient = 0.0;

			for (int k = 0; k < current_layer.size() - 1; k++) {
				sum_of_gradient = prev_layer[j].weight[k] * current_layer[k].gradient;
			}

			prev_layer[j].gradient = sum_of_gradient * Neuron::derivative_of_act_func(prev_layer[j].sum);
		}
	}

	for (int i = 0; i < structure.size() - 1; i++) {
		vector<Neuron> &current_layer = structure[i];
		vector<Neuron> &next_layer = structure[i + 1];

		for (int j = 0; j < current_layer.size(); j++) {
			for (int k = 0; k < next_layer.size() - 1; k++) {
				current_layer[j].delta_weight[k] = current_layer[j].value * next_layer[k].gradient * learning_rate;
				current_layer[j].weight[k] += current_layer[j].delta_weight[k];
			}
		}
	}
}

void Net::get_result(const vector<double>& input_values, const vector<double>& target_values) {
	cout << "Inputs: ";
	for (int i = 0; i < input_values.size(); i++) { cout << input_values[i] << " "; } cout << endl;
	cout << "Target: ";
	for (int i = 0; i < target_values.size(); i++) { cout << target_values[i] << " "; } cout << endl;
	cout << "Net result: ";
	for (int i = 0; i < structure.back().size() - 1; i++) {
		cout << structure.back()[i].value << " ";
	}
	cout << endl << endl;
}

void Net::save_weight(string path) {
	ofstream f; f.open(path);

	for (int i = 0; i < structure.size(); i++) {
		for (int j = 0; j < structure[i].size(); j++) {
			for (int k = 0; k < structure[i][j].weight.size(); k++) {
				f << structure[i][j].weight[k] << endl;
			}
		}
	}

	f.close();
}

void Net::load_weight(string path) {
	ifstream f; f.open(path);
	if (!f) { cout << "Error. File doesn't exist!!!"; Sleep(2000); exit(-1); };

	for (int i = 0; i < structure.size(); i++) {
		for (int j = 0; j < structure[i].size(); j++) {
			for (int k = 0; k < structure[i][j].weight.size(); k++) {
				f >> structure[i][j].weight[k];
			}
		}
	}
}

Net::Neuron::Neuron(int num_of_connection, int index) {
	for (int i = 0; i < num_of_connection; i++) {
		this->weight.push_back(static_cast<double>((rand() % 10000) / 10000.0) + 0.0001);
		this->delta_weight.push_back(0);
		cout << this->weight[i] << endl;
	}

	this->index = index;
}
