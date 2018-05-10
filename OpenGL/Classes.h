#pragma once
#ifndef CLASSES_H
#define CLASSES_H

#include "stdafx.h"
#include "SDL.h"
#include <vector>
#include <iostream>

using namespace std;

class Object {
private:
	int xvector = 0, yvector = 0;
	int length = 0;
	vector<int> mas;
	double deaths = 1;
	double lengthFinal = 0;
	int maxScore = 1;
public:
	Object(int start, int end, int WinHight, int length);
	void Snake(SDL_Renderer *renderer, int r, int g, int b);
	void Dis(SDL_Renderer *renderer, int r, int g, int b, int trans);
	void Rendering(SDL_Renderer *renderer, int r, int g, int b, int xp, int yp, int trans);
	void VectorH(int xvector, int yvector); 
	bool VecCollision(vector<int> issuePrime, int start, int end, int WinHight, int Tail, vector<int> &detectedPos);
	bool VecCollision(vector<int> issuePrime, int start, int end, int WinHight, int Tail);
	//Get values
	vector<int> GetVector() { return mas; }
	int GetXvector() { return xvector; }
	int GetYvector() { return yvector; }
	int GetLength() { return length; }
	int GetMaxScore() { return maxScore; }
	//Set values
	void SetVector(vector<int> *vec) { mas = *vec;  }
	void Setx(int x) { mas[0] = x; }
	void Sety(int y) { this->mas[1] = y; }
	void SetLength(int length) { this->length = length; }
	void SetMaxScore(int maxScore) { this->maxScore = maxScore; }
	void DisFinal() { cout << lengthFinal / deaths << endl; }
	double GetDeath() { return deaths; }
};

#endif