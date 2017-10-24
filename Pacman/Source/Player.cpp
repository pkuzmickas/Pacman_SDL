#include "../Headers/Player.h"
#include <iostream>
Player::Player(SDL_Renderer* renderer) {
	currentSprite = IMG_LoadTexture(renderer, "Sprites/pacman.png");
	if (currentSprite == NULL) std::cout << "NULL";
	posX = 0;
	posY = 0;
	spriteRect.x = posX;
	spriteRect.y = posY;
	spriteRect.w = 27;
	spriteRect.h = 27;

	// For the current sprite in the spritesheet
	spriteSheetRect.x = 0;
	spriteSheetRect.y = 0;
	spriteSheetRect.w = 27;
	spriteSheetRect.h = 27;

	speed = GAMESPEED;
}

void Player::animate(std::string direction) {
	Uint32 ticks = SDL_GetTicks();
	Uint32 speed = ticks / ANIMSPEED;
	
	int spriteRow = spriteSheetRect.y/PACMANSIZE;
	int numOfSprites = 2;
	if (direction == "right") {
		spriteRow = 0;
	}
	else if (direction == "down") {
		spriteRow = 1;
	}
	else if (direction == "left") {
		spriteRow = 2;
	}
	else if (direction == "up") {
		spriteRow = 3;
	}
	else if(direction == "none") {
		numOfSprites=1;
		
	}
	Uint32 sprite = speed % numOfSprites;
	spriteSheetRect.x = sprite * PACMANSIZE;
	spriteSheetRect.y = spriteRow*PACMANSIZE;
	spriteSheetRect.w = PACMANSIZE;
	spriteSheetRect.h = PACMANSIZE;
}

void Player::changePos(float x, float y) {
	posX = x;
	posY = y;
	spriteRect.x = posX;
	spriteRect.y = posY;
}

Player::~Player() {
	SDL_DestroyTexture(currentSprite);
}