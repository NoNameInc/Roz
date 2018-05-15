#include "stdafx.h"
#include "Classes.h"
#include "SDL.h"
#include "Modes.h"
#include <vector>


void Rendering(SDL_Renderer *renderer, int r, int g, int b, int xp, int yp) {
	SDL_Rect fillRect = { xp, yp, 10, 10 };
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &fillRect);
}

int Base(SDL_Renderer *renderer, vector<int> &wallVec, vector<int> &borderVec, vector<int> &detectedPos, 
				Object &apple, Object &stone, Object &player, Object &appleBot, Object &stoneBot, Object &playerBot, int WinWidth, int WinHight) {

	for (unsigned i = 0; i < wallVec.size(); i += 2) {
		Rendering(renderer, 0, 0, 255, wallVec[i], wallVec[i + 1]);
	}
	for (unsigned i = 0; i < borderVec.size(); i += 2) {
		Rendering(renderer, 0, 0, 255, borderVec[i], borderVec[i + 1]);			
	}
		/*____________________PLAYER____________________*/

		//Display
		apple.Dis(renderer, 0, 255, 0, 255);
		player.Snake(renderer, 255, 0, 0, 255);
		stone.Dis(renderer, 125, 125, 125, 255);

		//Collision
		if (!player.VecCollision(stone.GetVector(), 0, WinWidth / 2 - 5, WinHight, 0))
			player.SetMaxScore(player.GetMaxScore() - 5);
		if (!player.VecCollision(wallVec, 0, WinWidth / 2 - 5, WinHight, 0))
			player.SetMaxScore(player.GetMaxScore() - 5);
		if (!player.VecCollision(borderVec, 0, WinWidth / 2 - 5, WinHight, 0))
			player.SetMaxScore(player.GetMaxScore() - 5);
		if (!player.VecCollision(player.GetVector(), 0, WinWidth / 2 - 5, WinHight, 2))
			player.SetMaxScore(player.GetMaxScore() - 5);

		apple.VecCollision(stone.GetVector(), 0, WinWidth / 2 - 5, WinHight, 0);
		if (!apple.VecCollision(player.GetVector(), 0, WinWidth / 2 - 5, WinHight, 0)) {
			player.SetLength(player.GetLength() + 1);
			player.SetMaxScore(player.GetMaxScore() + 1);
		}


		/*____________________BOT____________________*/

		//Display
		appleBot.Dis(renderer, 0, 255, 0, 255);
		playerBot.Snake(renderer, 255, 0, 0, 255);
		stoneBot.Dis(renderer, 125, 125, 125, 255);

		//Collision
		detectedPos.clear();
		if (!playerBot.VecCollision(stoneBot.GetVector(), WinWidth / 2 - 5, WinWidth - 10, WinHight, 0, detectedPos))
			playerBot.SetMaxScore(playerBot.GetMaxScore() - 5);
		if (!playerBot.VecCollision(wallVec, WinWidth / 2 - 5, WinWidth - 10, WinHight, 0, detectedPos))
			playerBot.SetMaxScore(playerBot.GetMaxScore() - 5);
		if (!playerBot.VecCollision(borderVec, WinWidth / 2 - 5, WinWidth - 10, WinHight, 0, detectedPos))
			playerBot.SetMaxScore(playerBot.GetMaxScore() - 5);
		if (!playerBot.VecCollision(playerBot.GetVector(), WinWidth / 2 - 5, WinWidth - 10, WinHight, 1, detectedPos))
			playerBot.SetMaxScore(playerBot.GetMaxScore() - 5);

		appleBot.VecCollision(stoneBot.GetVector(), WinWidth / 2 - 5, WinWidth - 10, WinHight, 0);
		if (!appleBot.VecCollision(playerBot.GetVector(), WinWidth / 2 - 5, WinWidth - 10, WinHight, 0)) {
			playerBot.SetLength(playerBot.GetLength() + 1);
			playerBot.SetMaxScore(playerBot.GetMaxScore() + 1);
		}

		return player.GetMaxScore() - playerBot.GetMaxScore();
}

static int wallBias = 0;
int movingWall(SDL_Renderer *renderer, vector<int> &wallVec, vector<int> &borderVec, vector<int> &detectedPos,
	Object &apple, Object &stone, Object &player, Object &appleBot, Object &stoneBot, Object &playerBot, int WinWidth, int WinHight) {

	borderVec.clear();
	for (int j = 0; j < WinHight; j += 10) {
		borderVec.push_back(WinWidth / 2 - 5 + wallBias);
		borderVec.push_back(j);
	}

	//Display
	apple.Dis(renderer, 0, 255, 0, 255);
	player.Snake(renderer, 255, 0, 0, 255);
	stone.Dis(renderer, 125, 125, 125, 255);

	//Display
	appleBot.Dis(renderer, 0, 255, 0, 255);
	playerBot.Snake(renderer, 255, 0, 0, 255);
	stoneBot.Dis(renderer, 125, 125, 125, 255);

	for (unsigned i = 0; i < wallVec.size(); i += 2) {
		Rendering(renderer, 0, 0, 255, wallVec[i], wallVec[i + 1]);
	}
	for (unsigned i = 0; i < borderVec.size(); i += 2) {
		Rendering(renderer, 0, 0, 255, borderVec[i], borderVec[i + 1]);
	}

	//Collision
	if (!player.VecCollision(stone.GetVector(), 0, WinWidth / 2 - 5 + wallBias, WinHight, 0))
		wallBias -= 10;
	if (!player.VecCollision(stoneBot.GetVector(), 0, WinWidth / 2 - 5 + wallBias, WinHight, 0, detectedPos)) 
		wallBias -= 10;
	if (!player.VecCollision(wallVec, 0, WinWidth / 2 - 5 + wallBias, WinHight, 0))
		wallBias -= 10;
	if (!player.VecCollision(borderVec, 0, WinWidth / 2 - 5 + wallBias, WinHight, 0)) 
		wallBias -= 10;
	if (!player.VecCollision(player.GetVector(), 0, WinWidth / 2 - 5 + wallBias, WinHight, 2))
		wallBias -= 10;
	apple.VecCollision(stone.GetVector(), 0, WinWidth / 2 - 5 + wallBias, WinHight, 0);
	apple.VecCollision(borderVec, 0, WinWidth / 2 - 5 + wallBias, WinHight, 0);
	if (!apple.VecCollision(player.GetVector(), 0, WinWidth / 2 - 5 + wallBias, WinHight, 0)) {
		player.SetLength(player.GetLength() + 1);
		wallBias += 10;
		player.SetMaxScore(playerBot.GetMaxScore() + 1);
	}


	/*____________________BOT____________________*/

	//Collision
	detectedPos.clear();
	if (!playerBot.VecCollision(stoneBot.GetVector(), WinWidth / 2 - 5 + wallBias, WinWidth - 10, WinHight, 0, detectedPos)) 
		wallBias += 10;
	if (!playerBot.VecCollision(stone.GetVector(), WinWidth / 2 - 5 + wallBias, WinWidth - 10, WinHight, 0, detectedPos)) 
		wallBias += 10;
	if (!playerBot.VecCollision(wallVec, WinWidth / 2 - 5 + wallBias, WinWidth - 10, WinHight, 0, detectedPos))
		wallBias += 10;
	if (!playerBot.VecCollision(borderVec, WinWidth / 2 - 5 + wallBias, WinWidth - 10, WinHight, 0, detectedPos))
		wallBias += 10;
	if (!playerBot.VecCollision(playerBot.GetVector(), WinWidth / 2 - 5 + wallBias, WinWidth - 10, WinHight, 1, detectedPos)) 
		wallBias += 10;
	appleBot.VecCollision(stoneBot.GetVector(), WinWidth / 2 - 5 + wallBias, WinWidth - 10, WinHight, 0);
	appleBot.VecCollision(borderVec, WinWidth / 2 - 5 + wallBias, WinWidth - 10, WinHight, 0);
	if (!appleBot.VecCollision(playerBot.GetVector(), WinWidth / 2 - 5 + wallBias, WinWidth - 10, WinHight, 0)) {
		playerBot.SetLength(playerBot.GetLength() + 1);
		wallBias -= 10;
		playerBot.SetMaxScore(playerBot.GetMaxScore() + 1);
	}
	borderVec.clear();
	return wallBias;
}

void ResetWall() {
	wallBias = 0;
}

int Duel(SDL_Renderer *renderer, vector<int> &wallVec, vector<int> &borderVec, vector<int> &detectedPos,
	Object &apple, Object &stone, Object &player, Object &appleBot, Object &stoneBot, Object &playerBot, int WinWidth, int WinHight) {

	borderVec.clear();
	for (unsigned i = 0; i < wallVec.size(); i += 2) {
		Rendering(renderer, 0, 0, 255, wallVec[i], wallVec[i + 1]);
	}
	/*____________________PLAYER____________________*/

	//Display
	player.Snake(renderer, 255, 0, 0, 255);
	stone.Dis(renderer, 125, 125, 125, 255);

	appleBot.Dis(renderer, 0, 255, 0, 255);
	playerBot.Snake(renderer, 255, 0, 0, 255);
	stoneBot.Dis(renderer, 125, 125, 125, 255);

	//Collision
	if (!player.VecCollision(stone.GetVector(), 0, WinWidth - 10, WinHight, 0))
		player.SetMaxScore(player.GetMaxScore() - 5);
	if(!player.VecCollision(stoneBot.GetVector(), 0, WinWidth - 10, WinHight, 0, detectedPos))
		player.SetMaxScore(player.GetMaxScore() - 5);
	if (!player.VecCollision(wallVec, 0, WinWidth - 10, WinHight, 0))
		player.SetMaxScore(player.GetMaxScore() - 5);
	if (!player.VecCollision(player.GetVector(), 0, WinWidth - 10, WinHight, 2))
		player.SetMaxScore(player.GetMaxScore() - 5);
	if (!player.VecCollision(playerBot.GetVector(), 0, WinWidth - 10, WinHight, 0))
		player.SetMaxScore(player.GetMaxScore() - 5);

	//Collision
	detectedPos.clear();
	if (!playerBot.VecCollision(stoneBot.GetVector(), 0, WinWidth - 10, WinHight, 0, detectedPos))
		playerBot.SetMaxScore(playerBot.GetMaxScore() - 5);
	if (!playerBot.VecCollision(stone.GetVector(), 0, WinWidth - 10, WinHight, 0, detectedPos))
		playerBot.SetMaxScore(playerBot.GetMaxScore() - 5);
	if (!playerBot.VecCollision(wallVec, 0, WinWidth - 10, WinHight, 0, detectedPos))
		playerBot.SetMaxScore(playerBot.GetMaxScore() - 5);
	if (!playerBot.VecCollision(playerBot.GetVector(), 0, WinWidth - 10, WinHight, 1, detectedPos))
		playerBot.SetMaxScore(playerBot.GetMaxScore() - 5);
	if (!playerBot.VecCollision(player.GetVector(), 0, WinWidth - 10, WinHight, 0, detectedPos))
		playerBot.SetMaxScore(playerBot.GetMaxScore() - 5);
	
	appleBot.VecCollision(stoneBot.GetVector(), 0, WinWidth - 10, WinHight, 0);
	appleBot.VecCollision(stone.GetVector(), 0, WinWidth - 10, WinHight, 0);

	if (!appleBot.VecCollision(playerBot.GetVector(), 0, WinWidth - 10, WinHight, 0)) {
		playerBot.SetLength(playerBot.GetLength() + 1);
		playerBot.SetMaxScore(playerBot.GetMaxScore() + 1);
	}
	if (!appleBot.VecCollision(player.GetVector(), 0, WinWidth - 10, WinHight, 0)) {
		player.SetLength(player.GetLength() + 1);
		player.SetMaxScore(player.GetMaxScore() + 1);
	}

	return player.GetMaxScore() - playerBot.GetMaxScore();
}
