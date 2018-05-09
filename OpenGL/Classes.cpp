#include "stdafx.h"
#include "Classes.h"
#include <vector>
#include <iostream>
#include <iterator>

using namespace std;

void Object::Dis(SDL_Renderer *renderer, int r, int g, int b, int trans) {
	for (int i = (length - 1) * 2; i >= 0; i -= 2) {
		Rendering(renderer, r, g, b, mas[i], mas[i + 1], trans);
	}
}

bool Object::VecCollision(vector<int> issuePrime, int start, int end, int WinHight, int Tail) {
	vector<int> issue(issuePrime.begin() + Tail*2, issuePrime.end());
	for (unsigned i = 0; i < issue.size(); i += 2) {
		if (issue[i] == mas[0] && issue[i + 1] == mas[1]) {
			deaths++;
			lengthFinal += length;
			length = 1;
			mas[0] = ((rand() % ((end - start - 1) / 10)) + 1 + start / 10) * 10;
			mas[1] = ((rand() % (WinHight / 10 - 2)) + 1) * 10;
			return false;
		}
	}
	return true;
}

bool Object::VecCollision(vector<int> issuePrime, int start, int end, int WinHight, int Tail, vector<int> &detectedPos) {
	vector<int> issue(issuePrime.begin() + Tail*2, issuePrime.end());
	for (unsigned i = 0; i < issue.size(); i += 2) {
		if (length != 1 && (((issue[i] == mas[0] + 10 || issue[i] == mas[0] - 10) && issue[i + 1] == mas[1]) || ((issue[i + 1] == mas[1] + 10 || issue[i + 1] == mas[1] - 10) && issue[i] == mas[0]))) {
			if (Tail == 1 && i == 0) 
				;
			else {
				detectedPos.push_back(issue[i]);
				detectedPos.push_back(issue[i + 1]);
			}
		}
		if (issue[i] == mas[0] && issue[i + 1] == mas[1]) {
			deaths++;
			lengthFinal += length;
			length = 1;
			mas[0] = ((rand() % ((end - start - 1) / 10)) + 1 + start / 10) * 10;
			mas[1] = ((rand() % (WinHight / 10 - 2)) + 1) * 10;
			detectedPos.clear();
			return false;
		}
	}
	return true;
}

void Object::Rendering(SDL_Renderer *renderer, int r, int g, int b, int xp, int yp, int trans) {
	SDL_Rect fillRect = { xp, yp, 10, 10 };
	SDL_SetRenderDrawColor(renderer, r, g, b, trans);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(renderer, &fillRect);
}

void Object::VectorH(int xvector, int yvector) {
	this->xvector = xvector;
	this->yvector = yvector;
}

void Object::Snake(SDL_Renderer *renderer, int r, int g, int b) {
	mas.resize((length+1)*2);
	for (int i = (length-1)*2; i >= 0; i -= 2) {
		/*if (i > 3)
		Collision(mas[i][0], mas[i][1], GoOn);*/
		mas[i + 2] = mas[i];
		mas[i + 3] = mas[i + 1];
		if (i != 0) {
			mas[i] = mas[i - 1];
			mas[i + 1] = mas[i - 1];
		}
		else {
			mas[i] += xvector;
			mas[i + 1] += yvector;
		}
	}
	//Sensors
	Dis(renderer, r, g, b, 255);

	/*Rendering(renderer, 0, 255, 0, mas[0] + 10, mas[1], 128);
	Rendering(renderer, 0, 255, 0, mas[0] - 10, mas[1], 128);
	Rendering(renderer, 0, 255, 0, mas[0], mas[1] - 10, 128);
	Rendering(renderer, 0, 255, 0, mas[0], mas[1] + 10, 128);*/

}

Object::Object(int start, int end, int WinHight, int length) {
	this->length = length;
	mas.resize(length*2);
	for (int i = 0; i < length*2; i += 2) {
		mas[i] = ((rand() % ((end - start - 1) / 10)) + 1 + start/10) * 10;
		mas[i + 1] = ((rand() % (WinHight / 10 - 2)) + 1) * 10;
	}
}
