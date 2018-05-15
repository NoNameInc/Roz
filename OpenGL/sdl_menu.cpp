#include "stdafx.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include<windows.h>

//	/*  variant == 0 Вихід з гри
//		variant == 2x Режим 1
//		variant == 3x Режим 2
//		variant == 4x Режим 3
//		variant == 5x Режим 4
//					х - рівень важкості, х є [1,3]
//		variant == 60 Рекорди
//
//	*/
//}

int level(SDL_Window* window);

int ShowMenu(SDL_Window* window) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	TTF_Font* font1 = TTF_OpenFont("arial.ttf", 20);
	TTF_Font* font2 = TTF_OpenFont("arial.ttf", 15);
	TTF_Font* font = TTF_OpenFont("arial.ttf", 25);
	int i = 0, x, y;
	Uint32 time;
	/*-----------Дані полів----------------*/
	const int NUM = 6; // кількість обєктів у вікні
	const char* pole[NUM] = { "Select mode", "No Name Inc.", "Basic", "Duel", "Moving wall", "Exit" }; // обэкти у вікні
	SDL_Surface* menu[NUM];
	bool sel[NUM] = {0,0,0,0,0,0};
	SDL_Color color[2] = { {0,0,0},{255,0,0} };

	menu[0] = TTF_RenderText_Solid(font1, pole[0], color[0]);
	menu[1] = TTF_RenderText_Solid(font1, pole[1], color[0]);

	for (i = 2; i < NUM; i++)
	{
		menu[i] = TTF_RenderText_Solid(font, pole[i], color[0]);
	}

	 
	SDL_Rect koord[NUM];
	koord[0].x = surface->clip_rect.w / 2 - menu[0]->clip_rect.w / 2;
	koord[0].y = 130;
	koord[1].x = surface->clip_rect.w - menu[1]->clip_rect.w -20;
	koord[1].y = surface->clip_rect.h - menu[1]->clip_rect.h - 20;
	koord[2].x = surface->clip_rect.w / 2 + menu[0]->clip_rect.w / 2 - menu[2]->clip_rect.w;
	koord[2].y = 180;
	int a = 50;
	for (i = 3; i < NUM - 1; i++) {
		koord[i].x = koord[2].x;
		koord[i].y = 180 + a;
		koord[i].w = koord[i].y + 25;
			a += 50;
	}
	for (; i < NUM; i++)
	{
		koord[i].x = surface->clip_rect.w / 2 - menu[0]->clip_rect.w / 2;
		koord[i].y = 180 + a;
		koord[i].w = koord[i].y + 25;
		a += 50;
	}
	
	SDL_Event event;
	SDL_FillRect(surface, &surface->clip_rect, SDL_MapRGB(surface->format, 0xff, 0xff, 0xff));
	SDL_UpdateWindowSurface(window);
	
	/*---------------Визначення дії-------------*/
	while (true) {
		for (i = 0; i < NUM; i++) {
			SDL_BlitSurface(menu[i], NULL, surface, &koord[i]);
			SDL_UpdateWindowSurface(window);
		}
		time = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
				switch(event.type)
				{
					case SDL_QUIT: 
					for (i = 0; i < NUM; i++)
						SDL_FreeSurface(menu[i]);
					return 0;
					

					case SDL_MOUSEMOTION: 
						x = event.motion.x;
						y = event.motion.y;
						for (i = 2; i < NUM; i++)
						{
							sel[i] = 0;
							SDL_FreeSurface(menu[i]);
							menu[i] = TTF_RenderText_Solid(font, pole[i], color[0]);
							if (x >= koord[i].x && x <= koord[i].x + koord[i].w && y >= koord[i].y && y <= koord[i].y + 50)
							{
								if (sel[i] == 0)
								{
									sel[i] = 1;
									SDL_FreeSurface(menu[i]);
									menu[i] = TTF_RenderText_Solid(font, pole[i], color[1]);
									SDL_UpdateWindowSurface(window);
								}
							}
						}
						break;
					
					case SDL_MOUSEBUTTONDOWN: 
						x = event.motion.x;
						y = event.motion.y;
						if (x >= koord[NUM - 1].x && x <= koord[NUM - 1].x + koord[NUM - 1].w && y >= koord[NUM - 1].y && y <= koord[NUM - 1].y + 50)
						{
							return 0;
						}
						for (i = 2; i < NUM - 1; i++) {
							if (x >= koord[i].x && x <= koord[i].x + koord[i].w && y >= koord[i].y && y <= koord[i].y + 50)
							{
								int j = level(window);
								if(j != 0)
									return i*10 + j;
							}
						}
						break;
					
					/*case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE)
						{
							return 0;
						}*/
				}
				SDL_UpdateWindowSurface(window);
		}
	}
}


int level(SDL_Window* window) {
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	TTF_Font* font = TTF_OpenFont("arial.ttf", 25);
	TTF_Font* font1 = TTF_OpenFont("arial.ttf", 20);
	int i = 0, x, y;
	Uint32 time;
	const int NUM = 6;
	const char* pole[NUM] = { "Select the difficulty level", "No Name Inc.", "Easy", "Normal", "Hard", "Back" };
	SDL_Surface* menu[NUM];
	bool sel[NUM] = { 0,0,0,0,0,0 };
	SDL_Color color[2] = { { 0,0,0 },{ 255,0,0 } };

	menu[0] = TTF_RenderText_Solid(font1, pole[0], color[0]);
	menu[1] = TTF_RenderText_Solid(font1, pole[1], color[0]);

	for (i = 2; i < NUM; i++)
	{
		menu[i] = TTF_RenderText_Solid(font, pole[i], color[0]);
	}

	SDL_Rect koord[NUM];
	koord[0].x = surface->clip_rect.w / 2 - menu[0]->clip_rect.w / 2;
	koord[0].y = 130;
	koord[1].x = surface->clip_rect.w - menu[1]->clip_rect.w - 20;
	koord[1].y = surface->clip_rect.h - menu[1]->clip_rect.h - 20;
	koord[2].x = surface->clip_rect.w / 2 + menu[0]->clip_rect.w / 2 - menu[2]->clip_rect.w;
	koord[2].y = 180;
	int a = 50;
	for (i = 3; i < NUM - 1; i++) {
		koord[i].x = koord[2].x;
		koord[i].y = 180 + a;
		koord[i].w = koord[i].y + 25;
		a += 50;
	}
	for (; i < NUM; i++)
	{
		koord[i].x = surface->clip_rect.w / 2 - menu[0]->clip_rect.w / 2;
		koord[i].y = 180 + a;
		koord[i].w = koord[i].y + 25;
		a += 50;
	}
	SDL_Event event;
	SDL_FillRect(surface, &surface->clip_rect, SDL_MapRGB(surface->format, 0xff, 0xff, 0xff));
	SDL_UpdateWindowSurface(window);
	while (true) {
		for (i = 0; i < NUM; i++) {
			SDL_BlitSurface(menu[i], NULL, surface, &koord[i]);
			SDL_UpdateWindowSurface(window);
		}
		time = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEMOTION:
				x = event.motion.x;
				y = event.motion.y;
				for (i = 2; i < NUM; i++)
				{
					sel[i] = 0;
					SDL_FreeSurface(menu[i]);
					menu[i] = TTF_RenderText_Solid(font, pole[i], color[0]);
					if (x >= koord[i].x && x <= koord[i].x + koord[i].w && y >= koord[i].y && y <= koord[i].y + 50)
					{
						if (sel[i] == 0)
						{
							sel[i] = 1;
							SDL_FreeSurface(menu[i]);
							menu[i] = TTF_RenderText_Solid(font, pole[i], color[1]);
						}
					}
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				x = event.motion.x;
				y = event.motion.y;
				if (x >= koord[NUM - 1].x && x <= koord[NUM - 1].x + koord[NUM - 1].w && y >= koord[NUM - 1].y && y <= koord[NUM - 1].y + 50)
				{
					SDL_FillRect(surface, &surface->clip_rect, SDL_MapRGB(surface->format, 0xff, 0xff, 0xff));
					return 0;
				}
				for (i = 2; i < NUM - 1; i++) {
					if (x >= koord[i].x && x <= koord[i].x + koord[i].w && y >= koord[i].y && y <= koord[i].y + 50)
					{
						SDL_FillRect(surface, &surface->clip_rect, SDL_MapRGB(surface->format, 0xff, 0xff, 0xff));
						return i;
					}
				}
				break;

				case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					SDL_FillRect(surface, &surface->clip_rect, SDL_MapRGB(surface->format, 0xff, 0xff, 0xff));
					return 0;
				}
			}
			SDL_UpdateWindowSurface(window);
		}
	}
}

void winner(SDL_Window* window,int win) {
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	TTF_Font* font = TTF_OpenFont("arial.ttf", 50);
	const int NUM = 2;
	const char* pole[NUM] = { "You Win!!!", "Bot Win" };
	SDL_Surface* winn[NUM];
	SDL_Rect koord[NUM];
	switch (win) {
	case 1: {
		winn[0] = TTF_RenderText_Solid(font, pole[0], { 0,0,0 });
		koord[0].x = surface->clip_rect.w / 2 - winn[0]->clip_rect.w / 2;
		koord[0].y = surface->clip_rect.h / 2 - winn[0]->clip_rect.h / 2;
		SDL_UpdateWindowSurface(window);
	}
	case 2: {
		winn[1] = TTF_RenderText_Solid(font, pole[1], { 0,0,0 });
		koord[1].x = surface->clip_rect.w / 2 - winn[1]->clip_rect.w / 2;
		koord[1].y = surface->clip_rect.h / 2 - winn[1]->clip_rect.h / 2;
			SDL_UpdateWindowSurface(window);
	}
	}
}