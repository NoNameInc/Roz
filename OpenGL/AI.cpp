// ConsoleApplication3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <fstream>
#include <windows.h>
#include "AI.h"

using namespace std;

// Коефіцієнт навчання 
double lr = 1;

int maint() {
	vector<unsigned> topology = { 9, 18, 1 };
	Net net(topology);

	//net.loadWeight("weight.txt");

	string path = "trainingSet2.txt";
	ifstream f; f.open(path);

	vector<double> inputs(9);
	vector<double> targetVals(1);

	int amount_of_epoch = 300;
	int amount_of_situations = 512;

	int counter = 0; int i = 0;
	do {
		for (int i = 0; i < amount_of_situations; i++) {
			//f >> inputs[0] >> inputs[1] >> inputs[2] >> inputs[3] >> inputs[4] >> inputs[5] >> inputs[6] >> inputs[7] >> inputs[8] >> targetVals[0];
			for (int i = 0; i < inputs.size(); i++) {
				f >> inputs[i];
			}
			for (int i = 0; i < targetVals.size(); i++) {
				f >> targetVals[i];
			}

			net.feedForward(inputs);
			net.backProp(targetVals);

			net.getResult(inputs, targetVals);
		}
		f.seekg(0, ios::beg);

		i++;
	} while (i < amount_of_epoch);

	f.close();
	system("pause");
	return 0;
}

// Class Net
Net::Net(const vector<unsigned> &topology) {
	unsigned numLayers = topology.size();

	for (int i = 0; i < numLayers; i++) {
		Layers.push_back(Layer());
		unsigned amount_of_connection = i == topology.size() - 1 ? 0 : topology[i + 1];

		for (int j = 0; j <= topology[i]; j++) {
			Layers.back().push_back(Neuron(amount_of_connection, j));
		}
		Layers.back().back().setVal(1.0);
	}
};

// Виведення результатів
void Net::getResult(const vector<double> &inputs, const vector<double> &targetVals) {
	cout << "Inputs: ";
	for (int i = 0; i < inputs.size(); i++) { cout << inputs[i] << " "; } cout << endl;
	cout << "Target: ";
	for (int i = 0; i < targetVals.size(); i++) { cout << targetVals[i] << " "; } cout << endl;
	cout << "Net result: ";
	for (int i = 0; i < Layers.back().size() - 1; i++) {
		cout << Layers.back()[i].getVal() << " ";
		if (Layers.back()[i].getVal() > 0.04 && Layers.back()[i].getVal() < 0.20)      cout << "right";
		else if (Layers.back()[i].getVal() > 0.20 && Layers.back()[i].getVal() < 0.55) cout << "left";
		else if (Layers.back()[i].getVal() > 0.55 && Layers.back()[i].getVal() < 0.75) cout << "up";
		else if (Layers.back()[i].getVal() > 0.75 && Layers.back()[i].getVal() < 1)    cout << "down";
		else cout << "do whatever you want!!";
	}
	cout << endl << endl;
}

double Net::getOutput()
{
	double num = Layers.back().front().getVal();
	return num;
}

double Net::getLearningRate() {
	return lr;
}

void Net::setLearningRate(double learnRate) {
	lr = learnRate;
}

// Поширення даних
void Net::feedForward(const vector<double> &inputs) {
	for (int i = 0; i < inputs.size(); i++) {
		Layers[0][i].setVal(inputs[i]);
	}
	for (int i = 1; i < Layers.size(); i++) {
		Layer &prevLayer = Layers[i - 1];

		for (int j = 0; j < Layers[i].size() - 1; j++) {
			Layers[i][j].feedForward(prevLayer);
		}
	}
}

// Навчання найронної сітки за допомогою методу оберненого пиширення похибки
void Net::backProp(const vector<double> &targetVal) {
	// Градієнт для вихідного шару
	Layer &output = Layers.back();
	for (int i = 0; i < output.size() - 1; i++) {
		output[i].calcOutputGradient(targetVal[i]);
	}

	// Градієнт для прихованого шару
	Layer &hiddenLayer1 = Layers[Layers.size() - 2];
	for (int i = 0; i < hiddenLayer1.size() - 1; i++) {
		hiddenLayer1[i].calcHiddenGradient(output);
	}

	// Коригування ваг для вихідного шару
	for (int i = 0; i < hiddenLayer1.size(); i++) {
		hiddenLayer1[i].calcDeltaWeight(output);
		hiddenLayer1[i].updateWeight();
	}

	// Коригування ваг для прихованого шару
	Layer &inputLayer = Layers[Layers.size() - 3];
	for (int i = 0; i < inputLayer.size(); i++) {
		inputLayer[i].calcDeltaWeight(hiddenLayer1);
		inputLayer[i].updateWeight();
	}
}

// Зберігання ваг в файл
void Net::saveWeight(string name_file) {
	ofstream f; f.open(name_file);

	for (int i = 0; i < Layers.size(); i++) {
		for (int j = 0; j < Layers[i].size(); j++) {
			Layers[i][j].saveWeight(name_file);
		}
	}

	f.close();
}

// Завантаження ваг в нейронну сітку
void Net::loadWeight(string name_file) {
	ifstream f; f.open(name_file);
	if (!f) { cout << "Error. File doesn't exist!!!"; Sleep(2000); exit(-1); };

	vector<double> weight;
	double temp;

	while (!f.eof()) {
		f >> temp;
		weight.push_back(temp);
	}
	for (int i = 0; i < Layers.size(); i++) {
		for (int j = 0; j < Layers[i].size(); j++) {
			Layers[i][j].loadWeight(weight);
		}
	}
}

//class Neuron
Neuron::Neuron(int amount_of_connection, int index) {
	this->amount_of_connection = amount_of_connection;

	for (int i = 0; i < this->amount_of_connection; i++) {
		connection.push_back(Connection());
		connection.back().weight = (double)(rand()) / RAND_MAX;
	}

	this->index = index;
}

// Обчислення значення для нейрону
void Neuron::feedForward(Layer &prevLayer) {
	double sum = 0;
	for (int i = 0; i < prevLayer.size(); i++) {
		sum += prevLayer[i].getVal() * prevLayer[i].connection[index].weight;
	}
	this->sum = sum;
	this->value = activationFunc(sum);
}

// Коригування всіх ваг з'єднань які виходять з нейрону
void Neuron::updateWeight() {
	for (int i = 0; i < connection.size(); i++) {
		connection[i].weight += connection[i].delta_weight;
	}
}

// Обчислення градієнту для вихідного шару
void Neuron::calcOutputGradient(double targetVal) {
	this->gradient = (targetVal - value) * derivativeActivationFunc(sum);
}

// Обчислення похидки для ваги
void Neuron::calcDeltaWeight(Layer & nextLayer) {
	for (int i = 0; i < nextLayer.size() - 1; i++) {
		this->connection[i].delta_weight = lr * this->value * nextLayer[i].gradient;
	}
}

// Обчислення градієнту для прихованого шару
void Neuron::calcHiddenGradient(Layer &nextLayer) {
	double sum = sumGradient(nextLayer);

	this->gradient = sum * Neuron::derivativeActivationFunc(this->sum);
}

// Обчислення суми градієнту наступного шару для нейрону
double Neuron::sumGradient(Layer &layer) {
	double sum = 0.0;

	for (int i = 0; i < layer.size() - 1; i++) {
		sum += this->connection[i].weight * layer[i].gradient;
	}

	return sum;
}

// Функція активації діапазон (0, 1)
double Neuron::activationFunc(double sum) {
	return 1 / (1 + pow(2.71828182846, -sum));
}

// Похідна ф-ї активації
double Neuron::derivativeActivationFunc(double sum) {
	return pow(2.71828182846, -sum) / pow(1 + pow(2.71828182846, -sum), 2);
}

// Збереження всіх ваг з'єднань які виходять з нейрону
void Neuron::saveWeight(string name_file) {
	ofstream f; f.open(name_file, ostream::app);

	for (int j = 0; j < this->connection.size(); j++) {
		f << this->connection[j].weight << endl;
	}

	f.close();
}

int counterW;

// Завантаження ваг для нейрону
void Neuron::loadWeight(vector<double> &weight) {
	for (int i = 0; i < this->connection.size(); i++) {
		this->connection[i].weight = weight[counterW];
		counterW++;
	}
}
