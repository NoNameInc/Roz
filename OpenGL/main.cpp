// OpenGL.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <iostream>
#include <ctime>
#include <vector>
#include "Classes.h"
#include "matr.h"
#include <fstream>
#include <conio.h>
#include <cstring>
#include <sstream>
#include <windows.h>
#include "NN.h"

//Some comments

using namespace std;

void Rendering(SDL_Renderer *renderer, int r, int g, int b, int xp, int yp);
void DisText(int pos, const char *text, SDL_Renderer *renderer, TTF_Font *font);
void Events(SDL_Event &event, bool &GoOn, Object &player, int &FPS);
void BotControll(Object &playerBot, double val);

vector<int> detectedPos;
vector<double> matryza(4);
vector<int> wallVec;
vector<int> borderVec;

//Window size
const int WinWidth = 1030;
const int WinHight = 520;
int main(int argc, char *argv[]) {
	srand(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WinWidth, WinHight + 70, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	TTF_Font *font = TTF_OpenFont("arial.ttf", 25);

	//Create objects
	Uint32 start;
	SDL_Event event;

	//__MAP__
	for (int i = 0; i < WinWidth; i += 10) {
		for (int j = 0; j < WinHight; j += 10) {
			if (i == 0 || j == 0 || j == WinHight - 10 || i == WinWidth - 10) {
				wallVec.push_back(i);
				wallVec.push_back(j);
			}
		}
	}
	for (int j = 0; j < WinHight; j += 10) {
		borderVec.push_back(WinWidth / 2 - 5);
		borderVec.push_back(j);
	}

	//__PLAYER__
	Object stone(0, WinWidth / 2 - 5, WinHight, 10);
	Object apple(0, WinWidth / 2 - 5, WinHight, 1);
	Object player(0, WinWidth / 2 - 5, WinHight, 1);
	//player.VectorH(10, 0);

	//__BOT__
	Object stoneBot(WinWidth / 2 - 5, WinWidth - 10, WinHight, 20);
	Object appleBot(WinWidth / 2 - 5, WinWidth - 10, WinHight, 1);
	Object playerBot(WinWidth / 2 - 5, WinWidth - 10, WinHight, 1);
	playerBot.VectorH(10, 0);

	//__AI__
	vector<double> path(1);
	double oldPath = 0;

	vector<int> topology = {4, 20, 1};
	Net Cheater(topology);
	for (unsigned i = 0; i < matryza.size(); i++)
		matryza[i] = 0;

	double botVector = 0;
	int collis = 0;
	
	Cheater.load_weight("weight5.txt");

	string file_name1 = "recor5.txt";
	ifstream f1; f1.open(file_name1);

	int len = 0;
	f1 >> len;
	f1.close();
	int counter = 0;
	//__*/

	bool GoOn = true;
	int FPS = 20;
	//__Game loop__

	while (GoOn) {
		//Counte frame rate
		start = SDL_GetTicks();

		//Clear screen and set background color
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		
		/*____________________CONTROL____________________*/
		Events(event, GoOn, player, FPS);
		BotControll(playerBot, botVector);		

		/*____________________WALL____________________*/
		for (unsigned i = 0; i < wallVec.size(); i += 2) {
			Rendering(renderer, 0, 0, 255, wallVec[i], wallVec[i + 1]);
		}
		for (unsigned i = 0; i < borderVec.size(); i += 2) {
			Rendering(renderer, 0, 0, 255, borderVec[i], borderVec[i + 1]);
		}

		/*____________________PLAYER____________________*/

		//Display
		apple.Dis(renderer, 0, 255, 0, 255);
		player.Snake(renderer, 255, 0, 0);
		stone.Dis(renderer, 125, 125, 125, 255);

		//Collision
		player.VecCollision(stone.GetVector(), 0, WinWidth / 2 - 5, WinHight, 0);
		player.VecCollision(wallVec, 0, WinWidth / 2 - 5, WinHight, 0);
		player.VecCollision(borderVec, 0, WinWidth / 2 - 5, WinHight, 0);
		player.VecCollision(player.GetVector(), 0, WinWidth / 2 - 5, WinHight, 2);
		apple.VecCollision(stone.GetVector(), 0, WinWidth / 2 - 5, WinHight, 0);
		if (!apple.VecCollision(player.GetVector(), 0, WinWidth / 2 - 5, WinHight, 0)) {
			player.SetLength(player.GetLength() + 1);
			if (player.GetMaxScore() < player.GetLength())
				player.SetMaxScore(player.GetLength());
		}


		/*____________________BOT____________________*/

		//Display
		appleBot.Dis(renderer, 0, 255, 0, 255);
		playerBot.Snake(renderer, 255, 0, 0);
		stoneBot.Dis(renderer, 125, 125, 125, 255);

		//Collision
		detectedPos.clear();
		playerBot.VecCollision(stoneBot.GetVector(), WinWidth / 2 - 5, WinWidth - 10, WinHight, 0, detectedPos);
		playerBot.VecCollision(wallVec, WinWidth / 2 - 5, WinWidth - 10, WinHight, 0, detectedPos);
		playerBot.VecCollision(borderVec, WinWidth / 2 - 5, WinWidth - 10, WinHight, 0, detectedPos);
		playerBot.VecCollision(playerBot.GetVector(), WinWidth / 2 - 5, WinWidth - 10, WinHight, 1, detectedPos);
		appleBot.VecCollision(stoneBot.GetVector(), WinWidth / 2 - 5, WinWidth - 10, WinHight, 0);
		if (!appleBot.VecCollision(playerBot.GetVector(), WinWidth / 2 - 5, WinWidth - 10, WinHight, 0)) {
			playerBot.SetLength(playerBot.GetLength() + 1);			
			if (playerBot.GetMaxScore() < playerBot.GetLength())
				playerBot.SetMaxScore(playerBot.GetLength());
		}

		/*____________________AI____________________*/
		//Sensors
		GetMatryza(playerBot.GetVector()[0], playerBot.GetVector()[1], playerBot.GetVector()[2], playerBot.GetVector()[3], appleBot.GetVector()[0], appleBot.GetVector()[1], detectedPos, matryza, path, playerBot.GetXvector(), playerBot.GetYvector());

		Cheater.feed_forward(matryza);
		botVector = Cheater.get_output();
		//Cheater.back_propagation(path);
		Cheater.get_result(matryza, path);

		if (playerBot.GetLength() > len) {
			ofstream f1; f1.open(file_name1);
			len = playerBot.GetLength();
			f1 << len;
			cout << len;
			Cheater.save_weight("weight5.txt");
			f1.close();
		}

		cout << "Hello" << endl;

		//____________________Drawing text____________________
		std::ostringstream ostr;
		ostr << "MaxScore: " << player.GetMaxScore();

		std::ostringstream ostr2;
		ostr2 << "MaxScore: " << playerBot.GetMaxScore();

		DisText(0, ostr.str().c_str(), renderer, font);
		DisText(WinWidth / 2 - 5, ostr2.str().c_str(), renderer, font);

		//Render and FPS
		SDL_RenderPresent(renderer);
		if (1000 / FPS > SDL_GetTicks() - start) {
			SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
		}
	}
	cout << "Avarage length: " << endl;
	playerBot.DisFinal();
	cout << "Eaten apples: " << endl;
	cout << appleBot.GetDeath() << endl;
	_getch();

	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	SDL_Quit();
	TTF_Quit();
	return 0;
}
void BotControll(Object &playerBot, double val) {
	if (val >= 0 && val <= 0.33) {
		;
	}
	else if (val > 0.33 && val <= 0.66) { //Left
		playerBot.GetYvector() == 0 ? (playerBot.GetXvector() == 10 ? playerBot.VectorH(0, -10) : playerBot.VectorH(0, 10)) : playerBot.GetYvector() == 10 ? playerBot.VectorH(10, 0) : playerBot.VectorH(-10, 0);
	}
	else if (val >= 0.66 && val <= 1) { //Right
		playerBot.GetYvector() == 0 ? (playerBot.GetXvector() == 10 ? playerBot.VectorH(0, 10) : playerBot.VectorH(0, -10)) : playerBot.GetYvector() == 10 ? playerBot.VectorH(-10, 0) : playerBot.VectorH(10, 0);
	}
}

void Events(SDL_Event &event, bool &GoOn, Object &player, int &FPS) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			GoOn = false;
			break;
		}
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				if (player.GetXvector() != 10)
					player.VectorH(-10, 0);
				break;
			case SDLK_RIGHT:
				if (player.GetXvector() != -10)
					player.VectorH(10, 0);
				break;
			case SDLK_UP:
				if (player.GetYvector() != 10)
					player.VectorH(0, -10);
				break;
			case SDLK_DOWN:
				if (player.GetYvector() != -10)
					player.VectorH(0, 10);
				break;
			case SDLK_w:
				FPS += 10;
				break;
			case SDLK_s:
				if (FPS > 10)
					FPS -= 10;
				else
					FPS = 1;
				break;
			case SDLK_TAB:
				SDL_Delay(20000);
				break;
			default:
				break;
			}
		}
	}
}

void DisText(int pos, const char *text, SDL_Renderer *renderer, TTF_Font *font) {
	int texW = 0;
	int texH = 0;

	SDL_Surface *surface = TTF_RenderText_Solid(font, text, {0, 0, 0});
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { pos, WinHight, texW, texH };

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void Rendering(SDL_Renderer *renderer, int r, int g, int b, int xp, int yp) {
	SDL_Rect fillRect = { xp, yp, 10, 10 };
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &fillRect);
}

