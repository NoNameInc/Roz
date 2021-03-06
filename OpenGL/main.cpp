// OpenGL.cpp : Defines the entry point for the console application.
#include "stdafx.h"

#include "SDL.h"
#include "SDL_ttf.h"

#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <conio.h>
#include <cstring>
#include <sstream>
#include <windows.h>

#include "Classes.h"
#include "matr.h"
#include "NN.h"
#include "sdl_menu.h"
#include "Modes.h"


//Some comments

using namespace std;

void DisText(int pos, int Hight, const char *text, SDL_Renderer *renderer, TTF_Font *font);
void Events(SDL_Event &event, bool &GoOn, Object &player, double &FPS);
void BotControll(Object &playerBot, double val);
void level(int laval, Net& BotNN);

vector<int> detectedPos;
vector<double> matryza(4);
vector<int> wallVec;
vector<int> borderVec;

//Window size
const int WinWidth = 1030;
const int WinHight = 520;
double FPS = 30;


int main(int argc, char *argv[]) {
	srand(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WinWidth, WinHight + 70, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	TTF_Font *font = TTF_OpenFont("arial.ttf", 25);
	TTF_Font *font1 = TTF_OpenFont("arial.ttf", 150);

	int mode;
	while (true) {
		//---------------------------------------------------
		vector<int> topology = { 4, 20, 1 };
		Net BotNN(topology);

		int variant = ShowMenu(window);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		const int lezim = variant - variant % 10;
		const int lezima = variant % 10;
		switch (lezim) {
			case 0: return 0;

			case 20: {
				/*------режим 1---------*/
				mode = 1;
				level(lezima, BotNN);
				cout << "mode 1" << endl;
				break;
			}
			case 30: {
				/*------режим 2---------*/
				mode = 2;
				level(lezima, BotNN);
				cout << "mode 2" << endl;
				break;
			}
			case 40: {
				/*------режим 3---------*/
				mode = 3;
				level(lezima, BotNN);
				cout << "mode 3" << endl;
				break;
			}
		}
		int a = 0, b = 0, c = 0;

		//Create objects
		Uint32 start;
		SDL_Event event;

		//__MAP__
		wallVec.clear();
		for (int i = 0; i < WinWidth; i += 10) {
			for (int j = 0; j < WinHight; j += 10) {
				if (i == 0 || j == 0 || j == WinHight - 10 || i == WinWidth - 10) {
					wallVec.push_back(i);
					wallVec.push_back(j);
				}
			}
		}
		borderVec.clear();
		for (int j = 0; j < WinHight; j += 10) {
			borderVec.push_back(WinWidth / 2 - 5);
			borderVec.push_back(j);
		}

		//__PLAYER__
		Object stone(0, WinWidth / 2 - 5, WinHight, 20);
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

		for (unsigned i = 0; i < matryza.size(); i++)
			matryza[i] = 0;

		double botVector = 0;
		int collis = 0;


		bool GoOn = true;
		//__Game loop__

		double t = clock();

		while (GoOn) {
			//Counte frame rate
			start = SDL_GetTicks();

			//Clear screen and set background color
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);

			/*____________________CONTROL____________________*/
			Events(event, GoOn, player, FPS);
			BotControll(playerBot, botVector);

			/*________________Modes___________________*/
			switch (mode) {
			case 1: a = Base(renderer, wallVec, borderVec, detectedPos, apple, stone, player, appleBot, stoneBot, playerBot, WinWidth, WinHight);
				break;
			case 2: b = Duel(renderer, wallVec, borderVec, detectedPos, apple, stone, player, appleBot, stoneBot, playerBot, WinWidth, WinHight);
				break;
			case 3: c = movingWall(renderer, wallVec, borderVec, detectedPos, apple, stone, player, appleBot, stoneBot, playerBot, WinWidth, WinHight);
				break;
			}

			/*____________________AI____________________*/
			//Sensors
			GetMatryza(playerBot.GetVector()[0], playerBot.GetVector()[1], playerBot.GetVector()[2], playerBot.GetVector()[3], appleBot.GetVector()[0], appleBot.GetVector()[1], detectedPos, matryza, path, playerBot.GetXvector(), playerBot.GetYvector());

			BotNN.feed_forward(matryza);
			botVector = BotNN.get_output();
			//BotNN.back_propagation(path);
			//BotNN.get_result(matryza, path);

			/*if (playerBot.GetLength() > len) {
				ofstream f1; f1.open(file_name1);
				len = playerBot.GetLength();
				f1 << len;
				cout << len;
				BotNN.save_weight("weightNormal.txt");
				f1.close();
			}*/

			//____________________Drawing text____________________

			std::ostringstream ostr;
			ostr << "Score: " << player.GetMaxScore();

			std::ostringstream ostr2;
			ostr2 << "Score: " << playerBot.GetMaxScore();

			cout << c << endl;
			std::ostringstream ostr4;
			if (60 - (clock() - t) / CLOCKS_PER_SEC < 0 || WinWidth / 2 - 5 + c < 30 || WinWidth / 2 - 5 + c > WinWidth - 30) {
				ResetWall();
				double permanent = (clock() - t) / CLOCKS_PER_SEC;
				if (c < 0 || a < 0 || b < 0) {
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					ostr4 << "Bot   Win";
				}
				else if (c > 0 || a > 0 || b > 0) {
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
					ostr4 << "Human Win";
				}

				SDL_RenderClear(renderer);
				DisText(WinWidth / 2, WinHight / 2, ostr4.str().c_str(), renderer, font1);
				SDL_RenderPresent(renderer);
				SDL_Delay(2000);
				GoOn = false;
			}

			std::ostringstream ostr3;
			ostr3 << 60 - (clock() - t) / CLOCKS_PER_SEC;

			DisText(WinWidth / 2 - 10, WinHight + 10, ostr3.str().c_str(), renderer, font);
			DisText(70, WinHight + 10, ostr.str().c_str(), renderer, font);
			DisText(WinWidth - 100, WinHight + 10, ostr2.str().c_str(), renderer, font);

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
	}
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	SDL_Quit();
	TTF_Quit();
	_getch();
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

void Events(SDL_Event &event, bool &GoOn, Object &player, double &FPS) {
	if (SDL_PollEvent(&event)) {
		/*if (event.type == SDL_QUIT ) {
			GoOn = false;
			break;
		}*/
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				GoOn = false;
				break;
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

void DisText(int pos, int Hight, const char *text, SDL_Renderer *renderer, TTF_Font *font) {
	int texW = 0;
	int texH = 0;

	SDL_Surface *surface = TTF_RenderText_Solid(font, text, {0, 0, 0});
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { pos - texW / 2, Hight - texH / 2, texW, texH };

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void level(int levela, Net& BotNN) {
	switch (levela) {
	case 2: {
		BotNN.load_weight("weightEasy.txt");
		cout << "....Easy...." << endl;
		break;
	}
	case 3: {
		BotNN.load_weight("weightNormal.txt");
		cout << "====Normal====" << endl;
		break;
	}
	case 4: {
		BotNN.load_weight("weightHard.txt");
		cout << "/////HARD/////" << endl;
		break;
	}
	}
}

