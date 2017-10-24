#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Definitions.h"
#include <string>
class Player {
private:
	SDL_Texture* currentSprite;
	SDL_Rect spriteRect;
	SDL_Rect spriteSheetRect;
	float posX;
	float posY;
	float speed;
public:
	Player(SDL_Renderer* renderer);
	~Player();
	SDL_Texture* getSprite() { return currentSprite; }
	SDL_Rect getRect() { return spriteRect; }
	SDL_Rect getSSRect() { return spriteSheetRect; }
	void changePos(float x, float y);
	void animate(std::string direction);
	float getPosX() { return posX; }
	float getPosY() { return posY; }
	float getSpeed() { return speed; }
};