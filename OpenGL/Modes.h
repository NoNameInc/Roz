#pragma once
#ifndef MODES_H
#define MODES_H

#include "Classes.h"
#include "SDL.h"
void ResetWall();

int Base(SDL_Renderer *renderer, vector<int> &wallVec, vector<int> &borderVec, vector<int> &detectedPos,
	Object &apple, Object &stone, Object &player, Object &appleBot, Object &stoneBot, Object &playerBot, int WinWidth, int WinHight);

int Duel(SDL_Renderer *renderer, vector<int> &wallVec, vector<int> &borderVec, vector<int> &detectedPos,
	Object &apple, Object &stone, Object &player, Object &appleBot, Object &stoneBot, Object &playerBot, int WinWidth, int WinHight);

int movingWall(SDL_Renderer *renderer, vector<int> &wallVec, vector<int> &borderVec, vector<int> &detectedPos,
	Object &apple, Object &stone, Object &player, Object &appleBot, Object &stoneBot, Object &playerBot, int WinWidth, int WinHight);

void Rendering(SDL_Renderer *renderer, int r, int g, int b, int xp, int yp);

#endif