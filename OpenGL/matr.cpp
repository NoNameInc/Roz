#include "stdafx.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <conio.h>
#include "matr.h"

//Matryza
double appleVag = 2;
double wallVag = 1;
void GetMatryza(int x, int y, int x2, int y2, int applex, int appley, std::vector<int> &detectedPos, std::vector<double> &matryza, std::vector<double> &path, int xvec, int yvec) {
	for (int i = 0; i < matryza.size(); i++)
		matryza[i] = 0;

	double ax1, ay1, ax2, ay2;
	double bx = applex - x;
	double by = appley - y;
	double znamennuk = sqrt(xvec*xvec + yvec * yvec)*sqrt(bx*bx + by * by);
	if (znamennuk == 0)
		znamennuk = 1;
	double grad = acos((xvec*bx + yvec *by)/znamennuk) * 180 / 3.141;
	if (znamennuk == 0)
		_getch();

	double grad1, grad2;
	if (yvec == 0) {
		if (xvec == 10) {
			ax1 = 0;
			ay1 = -10;
			ax2 = 0;
			ay2 = 10;
		}
		else {
			ax1 = 0;
			ay1 = 10;
			ax2 = 0;
			ay2 = -10;
		}
	}
	else {
		if (yvec == 10) {
			ax1 = 10;
			ay1 = 0;
			ax2 = -10;
			ay2 = 0;
		}
		else {
			ax1 = -10;
			ay1 = 0;
			ax2 = 10;
			ay2 = 0;
		}
	}

	grad1 = acos((ax1*bx + ay1 * by) / (sqrt(ax1*ax1 + ay1 * ay1)*sqrt(bx*bx + by * by))) * 180 / 3.141;
	grad2 = acos((ax2*bx + ay2 * by) / (sqrt(ax2*ax2 + ay2 * ay2)*sqrt(bx*bx + by * by))) * 180 / 3.141;

	if (grad1 > grad2)
		grad *= -1;

	for (int i = 0; i < detectedPos.size(); i += 2) {
		if (x + xvec == detectedPos[i] || y + yvec == detectedPos[i + 1])
			matryza[1] = wallVag;
		else if (x + ax1 == detectedPos[i] && y + ay1 == detectedPos[i + 1]) {
			matryza[0] = wallVag; //Left
		}
		else if (x + ax2 == detectedPos[i] && y + ay2 == detectedPos[i + 1]) {
			matryza[2] = wallVag; //Right
		}
	}

	if (x + xvec == applex && y + yvec == appley)
		matryza[1] = appleVag;
	else if (x + ax1 == applex && y + ay1 == appley) {
		matryza[0] = appleVag; //Left
	}
	else if (x + ax2 == applex && y + ay2 == appley) {
		matryza[2] = appleVag; //Right
	}

	matryza[3] = double(int(grad)) / 100;

	if (matryza[1] != wallVag && ((grad >= -45 && grad <= 0) || (grad <= 45 && grad >= 0)))
		path[0] = 0;
	else if (matryza[0] != wallVag && (grad >= 0 || grad == -180)) //Left
		path[0] = 0.5;
	else if (matryza[2] != wallVag)//Right
		path[0] = 1;
	else if (matryza[1] != wallVag)
		path[0] = 0;
	else if (matryza[0] != wallVag)
		path[0] = 0.5;
	else 
		path[0] = 0;
}