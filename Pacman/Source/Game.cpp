#include "../Headers/Game.h"
#include <iostream>

using namespace std;



int Game::startGame() {

	running = true;
	while (running) {
		readInput();
		update();
		draw();
	}
	if (exitgame) return -1;
	else return 0;
}

Game::Game(SDL_Renderer* renderer) {
	gRenderer = renderer;
	player = new Player(gRenderer);
	player->changePos(STARTCOL*BOXSIZE, STARTROW*BOXSIZE);
	background = IMG_LoadTexture(gRenderer, "Sprites/map.png");
	nextMove = "none";
	scarySprite = IMG_LoadTexture(gRenderer, "Sprites/misc.png");
	createGhost(red);
	red.sprite = IMG_LoadTexture(gRenderer, "Sprites/red.png");
	red.originalSprite = IMG_LoadTexture(gRenderer, "Sprites/red.png");
	red.posRect.x = 13 * BOXSIZE;
	red.posRect.y = 11 * BOXSIZE;
	red.pos.x = 13 * BOXSIZE;
	red.pos.y = 11 * BOXSIZE;
	red.startTime = SDL_GetTicks() / 1000;
	red.goal.x = RED_CORNER_COL;
	red.goal.y = RED_CORNER_ROW;
	red.released = true;
	createGhost(pink);
	pink.sprite = IMG_LoadTexture(gRenderer, "Sprites/pink.png");
	pink.originalSprite = IMG_LoadTexture(gRenderer, "Sprites/pink.png");
	createGhost(orange);
	orange.sprite = IMG_LoadTexture(gRenderer, "Sprites/orange.png");
	orange.originalSprite = IMG_LoadTexture(gRenderer, "Sprites/orange.png");
	orange.posRect.x = 14 * BOXSIZE;
	orange.posRect.y = 13 * BOXSIZE;
	orange.pos.x = 14 * BOXSIZE;
	orange.pos.y = 13 * BOXSIZE;
	createGhost(blue);
	blue.sprite = IMG_LoadTexture(gRenderer, "Sprites/blue.png");
	blue.originalSprite = IMG_LoadTexture(gRenderer, "Sprites/blue.png");
	blue.posRect.x = 13 * BOXSIZE;
	blue.posRect.y = 13 * BOXSIZE;
	blue.pos.x = 13 * BOXSIZE;
	blue.pos.y = 13 * BOXSIZE;
	
	ghostSpeed = player->getSpeed();
}

void Game::draw() {

	SDL_RenderClear(gRenderer);
	
	SDL_RenderCopy(gRenderer, background, NULL, NULL);
	SDL_RenderCopy(gRenderer, player->getSprite() , &player->getSSRect() , &player->getRect());
	if (!scaryMode) {
		SDL_RenderCopy(gRenderer, red.sprite, &red.spriteSheetRect, &red.posRect);
		SDL_RenderCopy(gRenderer, pink.sprite, &pink.spriteSheetRect, &pink.posRect);
		SDL_RenderCopy(gRenderer, orange.sprite, &orange.spriteSheetRect, &orange.posRect);
		SDL_RenderCopy(gRenderer, blue.sprite, &blue.spriteSheetRect, &blue.posRect);
	}
	else {
		SDL_RenderCopy(gRenderer, red.sprite, &scaryRect, &red.posRect);
		SDL_RenderCopy(gRenderer, pink.sprite, &scaryRect, &pink.posRect);
		SDL_RenderCopy(gRenderer, orange.sprite, &scaryRect, &orange.posRect);
		SDL_RenderCopy(gRenderer, blue.sprite, &scaryRect, &blue.posRect);
	}
	if (grid) {
		for (int i = 0, k = 0; i < COLUMNS; i++, k += 29) {
			SDL_RenderDrawLine(gRenderer, k, 0, k, SCREEN_HEIGHT);
		}
		for (int i = 0, k = 0; i < ROWS; i++, k += 29) {
			SDL_RenderDrawLine(gRenderer, 0, k, SCREEN_WIDTH, k);
		}
		
	}
	for (int i = 0; i < COLUMNS; i++)
	{
		for (int j = 0; j < ROWS; j++) {

			if (board.isCandy(i, j)) {
				SDL_Rect rect;
				rect.w = 6;
				rect.h = 6;
				rect.x = i*BOXSIZE + BOXSIZE / 2 - rect.w / 2;
				rect.y = j*BOXSIZE + BOXSIZE / 2 - rect.h / 2;
				SDL_RenderFillRect(gRenderer, &rect);
			}
			if (board.getValue(i, j) == 2) {
				SDL_Rect rect;
				rect.w = 13;
				rect.h = 13;
				rect.x = i*BOXSIZE + BOXSIZE / 2 - rect.w / 2;
				rect.y = j*BOXSIZE + BOXSIZE / 2 - rect.h / 2;
				SDL_RenderFillRect(gRenderer, &rect);
			}
			
			

		}
	}

	SDL_RenderPresent(gRenderer);
	//cout << SDL_GetError << endl;
	
}

void Game::checkMoves() {
	moving_down = false;
	moving_left = false;
	moving_up = false;
	moving_right = false;
	inAction = false;
	if (nextMove == "up") {
		moving_up = true;
		dest = dest2;
		inAction = true;
	}
	else if (nextMove == "down") {
		moving_down = true;
		dest = dest2;
		inAction = true;
	}
	else if (nextMove == "left") {
		moving_left = true;
		dest = dest2;
		inAction = true;
	}
	else if (nextMove == "right") {
		moving_right = true;
		dest = dest2;
		inAction = true;
	}
	nextMove = "none";
	turning = false;
}

void Game::ghostMove(Ghost &ghost) {
	int destRow = ghost.posRect.y / BOXSIZE;
	int destCol = ghost.posRect.x / BOXSIZE;
	board.findAPossibleTurn(ghost.direction, "any", destCol, destRow);
	ghost.dest.x = destCol * BOXSIZE;
	ghost.dest.y = destRow * BOXSIZE;
	ghost.moving = true;
}

void Game::ghostAnimate(Ghost & ghost)
{
	Uint32 ticks = SDL_GetTicks();
	Uint32 speed = ticks / ANIMSPEED;
	int numOfSprites = 2;
	int spriteRow = 0;
	if (!scaryMode) {
		spriteRow = ghost.spriteSheetRect.y / GHOSTSIZEY;
		
		if (ghost.direction == "right") {
			spriteRow = 0;
		}
		else if (ghost.direction == "down") {
			spriteRow = 1;
		}
		else if (ghost.direction == "left") {
			spriteRow = 2;
		}
		else if (ghost.direction == "up") {
			spriteRow = 3;
		}
		else if (ghost.direction == "none") {
			numOfSprites = 1;
		}
		Uint32 sprite = speed % numOfSprites;
		ghost.spriteSheetRect.x = sprite * GHOSTSIZEX;
		ghost.spriteSheetRect.y = spriteRow*GHOSTSIZEY;
		ghost.spriteSheetRect.w = GHOSTSIZEX;
		ghost.spriteSheetRect.h = GHOSTSIZEY;
	}
	else {
		Uint32 sprite = speed % numOfSprites;
		if (SDL_GetTicks() / 1000 - scaryModeStartTime >= 5) {
			spriteRow = speed % numOfSprites;
		}
		if(ghost.dead) spriteRow = 2;
		scaryRect.x = sprite * GHOSTSIZEX;
		scaryRect.y = spriteRow*GHOSTSIZEY;
		scaryRect.w = GHOSTSIZEX;
		scaryRect.h = GHOSTSIZEY;
	}
	
}

void Game::checkCollision(Ghost & ghost)
{
	if (player->getPosX() >= ghost.pos.x + COLLISION_ERROR && player->getPosX() <= ghost.pos.x - COLLISION_ERROR + GHOSTSIZEX && player->getPosY() >= ghost.pos.y + COLLISION_ERROR && player->getPosY() <= ghost.pos.y - COLLISION_ERROR + GHOSTSIZEY) {
		if (scaryMode) {
			ghost.goal.x = 14;
			ghost.goal.y = 14;
			ghost.dead = true;
			ghost.released = false;
		
		}
		else {
			running = false;
			
		}
	}
	if (ghost.dead && scaryMode && ghost.pos.y / BOXSIZE == 11) {
		board.setBoard(14, 12, 0);
	}
	if (ghost.dead && ghost.pos.y / BOXSIZE == 12 && ghost.pos.x/BOXSIZE == 14) {
		board.setBoard(14, 12, 1);
		ghost.startTime = SDL_GetTicks() / 1000;
	}
	if (scaryMode) {
		ghost.sprite = scarySprite;
	}
	else {
		ghost.sprite = ghost.originalSprite;
	}
}

float Game::vecDistance(Vec2 p, Vec2 q)
{
	return std::sqrt((p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y));
}
void Game::createGhost(Ghost & ghost)
{
	ghost.chaseMode = false;
	ghost.moving = false;
	ghost.goal.x = 14;
	ghost.goal.y = 14;
	ghost.posRect.h = GHOSTSIZEY; // texture height
	ghost.posRect.w = 26; // width
	ghost.posRect.x = GHOST_START_COL * BOXSIZE;
	ghost.posRect.y = GHOST_START_ROW * BOXSIZE;
	ghost.pos.x = GHOST_START_COL * BOXSIZE;
	ghost.pos.y = GHOST_START_ROW * BOXSIZE;
	//red.sprite = IMG_LoadTexture(gRenderer, "Sprites/red.png");
	ghost.spriteSheetRect.h = GHOSTSIZEY;
	ghost.spriteSheetRect.w = 26;
	ghost.spriteSheetRect.x = 0;
	ghost.spriteSheetRect.y = 0;
	ghost.direction = "none";
	ghost.released = false;
	ghost.startTime = SDL_GetTicks() / 1000;
}
// GHOST AI
void Game::findGhostMove(Ghost &ghost) {
	if (!ghost.moving) {
		if (ghost.direction == "right") {
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) != 1 && board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, (ghost.posRect.y / BOXSIZE)) == 1) { // if down direction is free but up is a wall
				ghost.direction = "down";
				ghost.spriteSheetRect.y =GHOSTSIZEY;
				ghostMove(ghost);
			}
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) == 1 && board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, (ghost.posRect.y / BOXSIZE)) == 1) { // if up direction is free but down is a wall
				ghost.direction = "up";
				
				ghost.spriteSheetRect.y =GHOSTSIZEY*3;
				ghostMove(ghost);
			}
			//if top and right are open
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) == 1 && board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, (ghost.posRect.y / BOXSIZE)) != 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3;
					v1.y = ((ghost.posRect.y / BOXSIZE) - 1)*BOXSIZE;
					v1.x = ghost.posRect.x;
					v2.x = ((ghost.posRect.x / BOXSIZE) + 1)*BOXSIZE;
					v2.y = ghost.posRect.y;
					v3.x = ghost.goal.x*BOXSIZE;
					v3.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v1, v3) < vecDistance(v2, v3)) {
						ghost.direction = "up";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 3;
					}
					else {
						ghost.direction = "right";
					}
					
				}
				else {
					int t = rand() % 2;
					if (t == 0) {
						ghost.direction = "up";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 3;
					}
					else {
						ghost.direction = "right";
					}
				}
				ghostMove(ghost);
			}
			// down and right
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) != 1 && board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, (ghost.posRect.y / BOXSIZE)) != 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3;

					v1.y = ((ghost.posRect.y / BOXSIZE) + 1)*BOXSIZE;
					v1.x = ghost.posRect.x;
					v2.x = ((ghost.posRect.x / BOXSIZE) + 1)*BOXSIZE;
					v2.y = ghost.posRect.y;
					v3.x = ghost.goal.x*BOXSIZE;
					v3.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v1, v3) < vecDistance(v2, v3)) {
						ghost.direction = "down";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 1;
					}
					else {
						ghost.direction = "right";
					}
				}
				else {
					int t = rand() % 2;
					if (t == 0) {
						ghost.direction = "down";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 1;
					}
					else {
						ghost.direction = "right";
					}
				}
				ghostMove(ghost);
			}
			//down and up
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) != 1 && board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, (ghost.posRect.y / BOXSIZE)) == 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3;
					v1.y = ((ghost.posRect.y / BOXSIZE) - 1)*BOXSIZE;
					v1.x = ghost.posRect.x;
					v2.y = ((ghost.posRect.y / BOXSIZE) + 1)*BOXSIZE;
					v2.x = ghost.posRect.x;
					v3.x = ghost.goal.x*BOXSIZE;
					v3.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v1, v3) < vecDistance(v2, v3)) {
						ghost.direction = "up";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 3;
					}
					else {
						ghost.direction = "down";
						ghost.spriteSheetRect.y = GHOSTSIZEY;

					}
				}
				else {
					int t = rand() % 2;
					if (t == 0) {
						ghost.direction = "up";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 3;
					}
					else {
						ghost.direction = "down";
						ghost.spriteSheetRect.y = GHOSTSIZEY;
					}
				}
				ghostMove(ghost);
			}
			// up down right
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) != 1 && board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, (ghost.posRect.y / BOXSIZE)) != 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3, v4;
					v1.y = ((ghost.posRect.y / BOXSIZE) - 1)*BOXSIZE; //up
					v1.x = ghost.posRect.x;
					v2.y = ((ghost.posRect.y / BOXSIZE) + 1)*BOXSIZE; //down
					v2.x = ghost.posRect.x;
					v3.x = ((ghost.posRect.x / BOXSIZE) + 1)*BOXSIZE; //right
					v3.y = ghost.posRect.y;
					v4.x = ghost.goal.x*BOXSIZE;
					v4.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v1, v4) < vecDistance(v2, v4) && vecDistance(v1, v4) < vecDistance(v3, v4)) {
						ghost.direction = "up";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 3;
					}
					else if (vecDistance(v2, v4) < vecDistance(v1, v4) && vecDistance(v2, v4) < vecDistance(v3, v4)) {
						ghost.direction = "down";
						ghost.spriteSheetRect.y = GHOSTSIZEY;
					}
					else {
						ghost.direction = "right";
					}
				}
				else {
					int t = rand() % 3;
					if (t == 0) {
						ghost.direction = "up";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 3;
					}
					else if(t==1) {
						ghost.direction = "down";
						ghost.spriteSheetRect.y = GHOSTSIZEY;
					}
					else {
						ghost.direction = "right";
					}
				}
				ghostMove(ghost);
			}
		}
		//LEFT
		else if (ghost.direction == "left") {
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) != 1 && board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) == 1) { 
				ghost.direction = "down";
				ghost.spriteSheetRect.y =GHOSTSIZEY;
				ghostMove(ghost);
			}
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) == 1 && board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) == 1) {
				ghost.direction = "up";

				ghost.spriteSheetRect.y =GHOSTSIZEY * 3;
				ghostMove(ghost);
			}
			//if top and left are open
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) == 1 && board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) != 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3;
					v1.y = ((ghost.posRect.y / BOXSIZE) - 1)*BOXSIZE;
					v1.x = ghost.posRect.x;
					v2.x = ((ghost.posRect.x / BOXSIZE) - 1)*BOXSIZE;
					v2.y = ghost.posRect.y;
					v3.x = ghost.goal.x*BOXSIZE;
					v3.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v1, v3) < vecDistance(v2, v3)) {
						ghost.direction = "up";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 3;
					}
					else {
						ghost.direction = "left";
					}
				}
				else {
					int t = rand() % 2;
					if (t == 0) {
						ghost.direction = "up";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 3;
					}
					else {
						ghost.direction = "left";
					}
				}
				ghostMove(ghost);
			}
			// down and left
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) != 1 && board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) != 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3;
					v1.y = ((ghost.posRect.y / BOXSIZE) + 1)*BOXSIZE;
					v1.x = ghost.posRect.x;
					v2.x = ((ghost.posRect.x / BOXSIZE) - 1)*BOXSIZE;
					v2.y = ghost.posRect.y;
					v3.x = ghost.goal.x*BOXSIZE;
					v3.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v1, v3) < vecDistance(v2, v3)) {
						ghost.direction = "down";
						ghost.spriteSheetRect.y = GHOSTSIZEY;
					}
					else {
						ghost.direction = "left";
					}
				}
				else {
					int t = rand() % 2;
					if (t == 0) {
						ghost.direction = "down";
						ghost.spriteSheetRect.y = GHOSTSIZEY;
					}
					else {
						ghost.direction = "left";
					}
				}
				ghostMove(ghost);
			}
			//down and up
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) != 1 && board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) == 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3;
					v1.y = ((ghost.posRect.y / BOXSIZE) - 1)*BOXSIZE;
					v1.x = ghost.posRect.x;
					v2.y = ((ghost.posRect.y / BOXSIZE) + 1)*BOXSIZE;
					v2.x = ghost.posRect.x;
					v3.x = ghost.goal.x*BOXSIZE;
					v3.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v1, v3) < vecDistance(v2, v3)) {
						ghost.direction = "up";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 3;
					}
					else {
						ghost.direction = "down";
						ghost.spriteSheetRect.y = GHOSTSIZEY;
					}
				}
				else {
					int t = rand() % 2;
					if (t == 0) {
						ghost.direction = "up";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 3;
					}
					else {
						ghost.direction = "down";
						ghost.spriteSheetRect.y = GHOSTSIZEY;
					}
				}
				ghostMove(ghost);
			}
			// up down left
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) != 1 && board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) != 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3, v4;
					v1.y = ((ghost.posRect.y / BOXSIZE) - 1)*BOXSIZE; //up
					v1.x = ghost.posRect.x;
					v2.y = ((ghost.posRect.y / BOXSIZE) + 1)*BOXSIZE; //down
					v2.x = ghost.posRect.x;
					v3.x = ((ghost.posRect.x / BOXSIZE) - 1)*BOXSIZE; //left
					v3.y = ghost.posRect.y;
					v4.x = ghost.goal.x*BOXSIZE;
					v4.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v1, v4) < vecDistance(v2, v4) && vecDistance(v1, v4) < vecDistance(v3, v4)) {
						ghost.direction = "up";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 3;
					}
					else if (vecDistance(v2, v4) < vecDistance(v1, v4) && vecDistance(v2, v4) < vecDistance(v3, v4)) {
						ghost.direction = "down";
						ghost.spriteSheetRect.y = GHOSTSIZEY;
					}
					else {
						ghost.direction = "left";
					}
				}
				else {
					int t = rand() % 3;
					if (t == 0) {
						ghost.direction = "up";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 3;
					}
					else if (t == 1) {
						ghost.direction = "down";
						ghost.spriteSheetRect.y = GHOSTSIZEY;
					}
					else {
						ghost.direction = "left";
					}
				}
				ghostMove(ghost);
			}
		}
		//DOWN
		else if (ghost.direction == "down") {
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, ghost.posRect.y / BOXSIZE) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) == 1) {
				ghost.direction = "right";
				ghost.spriteSheetRect.y = 0;
				ghostMove(ghost);
			}
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, ghost.posRect.y / BOXSIZE) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) != 1) {
				ghost.direction = "left";

				ghost.spriteSheetRect.y =GHOSTSIZEY * 2;
				ghostMove(ghost);
			}
			//if down and left are open 
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, ghost.posRect.y / BOXSIZE) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) != 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3;
					v1.y = ((ghost.posRect.y / BOXSIZE) + 1)*BOXSIZE;
					v1.x = ghost.posRect.x;
					v2.x = ((ghost.posRect.x / BOXSIZE) - 1)*BOXSIZE; //left
					v2.y = ghost.posRect.y;
					v3.x = ghost.goal.x*BOXSIZE;
					v3.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v2, v3) < vecDistance(v1, v3)) {
						ghost.direction = "left";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 2;
					}
					else {
						ghost.direction = "down";
					}
				}
				else {
					int t = rand() % 2;
					if (t == 0) {
						ghost.direction = "left";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 2;
					}
					else {
						ghost.direction = "down";
					}
				}
				ghostMove(ghost);
			}
			// down and right
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, ghost.posRect.y / BOXSIZE) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) == 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3;
					v1.y = ((ghost.posRect.y / BOXSIZE) + 1)*BOXSIZE;
					v1.x = ghost.posRect.x;
					v2.x = ((ghost.posRect.x / BOXSIZE) + 1)*BOXSIZE; //right
					v2.y = ghost.posRect.y;
					v3.x = ghost.goal.x*BOXSIZE;
					v3.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v2, v3) < vecDistance(v1, v3)) {
						ghost.direction = "right";
						ghost.spriteSheetRect.y = 0;
					}
					else {
						ghost.direction = "down";
					}
				}
				else {
					int t = rand() % 2;
					if (t == 0) {
						ghost.direction = "right";
						ghost.spriteSheetRect.y = 0;
					}
					else {
						ghost.direction = "down";
					}
				}
				ghostMove(ghost);
			}
			//right and left
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, ghost.posRect.y / BOXSIZE) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) != 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3;
					v1.x = ((ghost.posRect.x / BOXSIZE) - 1)*BOXSIZE; //left
					v1.y = ghost.posRect.y;
					v2.x = ((ghost.posRect.x / BOXSIZE) + 1)*BOXSIZE; //right
					v2.y = ghost.posRect.y;
					v3.x = ghost.goal.x*BOXSIZE;
					v3.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v1, v3) < vecDistance(v2, v3)) {
						ghost.direction = "left";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 2;
					}
					else {
						ghost.direction = "right";
						ghost.spriteSheetRect.y = 0;
					}
				}
				else {
					int t = rand() % 2;
					if (t == 0) {
						ghost.direction = "left";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 2;
					}
					else {
						ghost.direction = "right";
						ghost.spriteSheetRect.y = 0;
					}
				}
				ghostMove(ghost);
			}
			// right down left
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) + 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, ghost.posRect.y / BOXSIZE) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) != 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3, v4;
					v1.x = ((ghost.posRect.x / BOXSIZE) + 1)*BOXSIZE; //right
					v1.y = ghost.posRect.y;
					v2.y = ((ghost.posRect.y / BOXSIZE) + 1)*BOXSIZE; //down
					v2.x = ghost.posRect.x;
					v3.x = ((ghost.posRect.x / BOXSIZE) - 1)*BOXSIZE; //left
					v3.y = ghost.posRect.y;
					v4.x = ghost.goal.x*BOXSIZE;
					v4.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v1, v4) < vecDistance(v2, v4) && vecDistance(v1, v4) < vecDistance(v3, v4)) {
						ghost.direction = "right";
						ghost.spriteSheetRect.y = 0;
					}
					else if (vecDistance(v2, v4) < vecDistance(v1, v4) && vecDistance(v2, v4) < vecDistance(v3, v4)) {
						ghost.direction = "down";
					}
					else {
						ghost.direction = "left";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 2;

					}
				}
				else {
					int t = rand() % 3;
					if (t == 0) {
						ghost.direction = "right";
						ghost.spriteSheetRect.y = 0;
					}
					else if (t == 1) {
						ghost.direction = "down";
					}
					else {
						ghost.direction = "left";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 2;
					}
				}
				ghostMove(ghost);
			}
		}
		//UP
		else if (ghost.direction == "up") {
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, ghost.posRect.y / BOXSIZE) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) == 1) {
				ghost.direction = "right";
				ghost.spriteSheetRect.y = 0;
				ghostMove(ghost);
			}
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, ghost.posRect.y / BOXSIZE) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) != 1) {
				ghost.direction = "left";

				ghost.spriteSheetRect.y =GHOSTSIZEY * 2;
				ghostMove(ghost);
			}
			//if up and left are open 
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, ghost.posRect.y / BOXSIZE) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) != 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3;
					v1.y = ((ghost.posRect.y / BOXSIZE) - 1)*BOXSIZE;
					v1.x = ghost.posRect.x;
					v2.x = ((ghost.posRect.x / BOXSIZE) - 1)*BOXSIZE; //left
					v2.y = ghost.posRect.y;
					v3.x = ghost.goal.x*BOXSIZE;
					v3.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v2, v3) < vecDistance(v1, v3)) {
						ghost.direction = "left";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 2;
					}
					else {
						ghost.direction = "up";
					}
				}
				else {
					int t = rand() % 2;
					if (t == 0) {
						ghost.direction = "left";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 2;
					}
					else {
						ghost.direction = "up";
					}
				}
				ghostMove(ghost);
			}
			// up and right
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, ghost.posRect.y / BOXSIZE) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) == 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3;
					v1.y = ((ghost.posRect.y / BOXSIZE) - 1)*BOXSIZE;
					v1.x = ghost.posRect.x;
					v2.x = ((ghost.posRect.x / BOXSIZE) + 1)*BOXSIZE; //right
					v2.y = ghost.posRect.y;
					v3.x = ghost.goal.x*BOXSIZE;
					v3.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v2, v3) < vecDistance(v1, v3)) {
						ghost.direction = "right";
						ghost.spriteSheetRect.y = 0;
					}
					else {
						ghost.direction = "up";
					}
				}
				else {
					int t = rand() % 2;
					if (t == 0) {
						ghost.direction = "right";
						ghost.spriteSheetRect.y = 0;
					}
					else {
						ghost.direction = "up";
					}
				}
				ghostMove(ghost);
			}
			//right and left
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) == 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, ghost.posRect.y / BOXSIZE) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) != 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3;
					v1.x = ((ghost.posRect.x / BOXSIZE) - 1)*BOXSIZE; //left
					v1.y = ghost.posRect.y;
					v2.x = ((ghost.posRect.x / BOXSIZE) + 1)*BOXSIZE; //right
					v2.y = ghost.posRect.y;
					v3.x = ghost.goal.x*BOXSIZE;
					v3.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v1, v3) < vecDistance(v2, v3)) {
						ghost.direction = "left";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 2;
					}
					else {
						ghost.direction = "right";
						ghost.spriteSheetRect.y = 0;
					}
				}
				else {
					int t = rand() % 2;
					if (t == 0) {
						ghost.direction = "left";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 2;
					}
					else {
						ghost.direction = "right";
						ghost.spriteSheetRect.y = 0;
					}
				}
				ghostMove(ghost);
			}
			// up right left
			if (board.getValue(ghost.posRect.x / BOXSIZE, (ghost.posRect.y / BOXSIZE) - 1) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) + 1, ghost.posRect.y / BOXSIZE) != 1 && board.getValue((ghost.posRect.x / BOXSIZE) - 1, (ghost.posRect.y / BOXSIZE)) != 1)
			{
				if (!scaryMode) {
					Vec2 v1, v2, v3, v4;
					v1.x = ((ghost.posRect.x / BOXSIZE) + 1)*BOXSIZE; //right
					v1.y = ghost.posRect.y;
					v2.y = ((ghost.posRect.y / BOXSIZE) - 1)*BOXSIZE; //up
					v2.x = ghost.posRect.x;
					v3.x = ((ghost.posRect.x / BOXSIZE) - 1)*BOXSIZE; //left
					v3.y = ghost.posRect.y;
					v4.x = ghost.goal.x*BOXSIZE;
					v4.y = ghost.goal.y*BOXSIZE;

					if (vecDistance(v1, v4) < vecDistance(v2, v4) && vecDistance(v1, v4) < vecDistance(v3, v4)) {
						ghost.direction = "right";
						ghost.spriteSheetRect.y = 0;
					}
					else if (vecDistance(v2, v4) < vecDistance(v1, v4) && vecDistance(v2, v4) < vecDistance(v3, v4)) {
						ghost.direction = "up";
					}
					else {
						ghost.direction = "left";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 2;

					}
				}
				else {
					int t = rand() % 3;
					if (t == 0) {
						ghost.direction = "right";
						ghost.spriteSheetRect.y = 0;
					}
					else if (t == 1) {
						ghost.direction = "up";
					}
					else {
						ghost.direction = "left";
						ghost.spriteSheetRect.y = GHOSTSIZEY * 2;
					}
				}
				ghostMove(ghost);
			}
		}
		if (ghost.direction == "none") {
			ghost.direction = "right";
			ghostMove(ghost);
		}
	}
	if (ghost.moving) {
		if (ghost.direction == "right") {
			ghost.pos.x += ghostSpeed;
			if (ghost.pos.x / 0.5 != ghost.pos.x * 2) ghost.pos.x = (int)ghost.pos.x;
			ghost.posRect.x = ghost.pos.x;
			if (ghost.pos.x >= ghost.dest.x) {
				if (ghost.dest.x > (COLUMNS-1)*BOXSIZE) {
					ghost.pos.x = 0;
					ghost.posRect.x = ghost.pos.x;
					ghostMove(ghost);
				}
				else ghost.moving = false;
			}
		}
		if (ghost.direction == "left") {
			ghost.pos.x -= ghostSpeed;
			if (ghost.pos.x / 0.5 != ghost.pos.x * 2) ghost.pos.x = (int)ghost.pos.x;
			ghost.posRect.x = ghost.pos.x;
			if (ghost.pos.x <= ghost.dest.x) {
				if (ghost.dest.x < 0) {
					ghost.pos.x = (COLUMNS - 1) * BOXSIZE;
					ghost.posRect.x = ghost.pos.x;
					ghostMove(ghost);
				}
				else ghost.moving = false;
			}
		}
		if (ghost.direction == "down") {
			ghost.pos.y += ghostSpeed;
			if (ghost.pos.y / 0.5 != ghost.pos.y * 2) ghost.pos.y = (int)ghost.pos.y;
			ghost.posRect.y = ghost.pos.y;
			if (ghost.pos.y >= ghost.dest.y) {
				ghost.moving = false;
			}
		}
		if (ghost.direction == "up") {
			ghost.pos.y -= ghostSpeed;
			if (ghost.pos.y / 0.5 != ghost.pos.y * 2) ghost.pos.y = (int)ghost.pos.y;
			ghost.posRect.y = ghost.pos.y;
			if (ghost.pos.y <= ghost.dest.y) {
				ghost.moving = false;
			}
		}
	}
}

void Game::update() {
	
	findDirection();


	

	findGhostMove(red);
	ghostAnimate(red);
	checkCollision(red);
	red.secondsInMode = SDL_GetTicks() / 1000 - red.startTime;
	if (!red.released) {
		if (red.secondsInMode == 1 && red.goal.x != RED_CORNER_COL) {
			board.setBoard(14, 12, 0);

			red.startTime = SDL_GetTicks() / 1000;
			red.goal.x = 14;
			red.goal.y = 11;
		}
		if (red.pos.y / BOXSIZE == 11) {
			board.setBoard(14, 12, 1);
			red.goal.x = RED_CORNER_COL;
			red.goal.y = RED_CORNER_ROW;
			red.released = true;
		}
	}

	if (red.released) {
		if (!red.chaseMode && red.secondsInMode == SCATTER_MODE_LENGTH) {
			red.chaseMode = true;
			red.startTime = SDL_GetTicks() / 1000;
		}
		else if (red.chaseMode && red.secondsInMode == CHASE_MODE_LENGTH) {
			red.chaseMode = false;
			red.startTime = SDL_GetTicks() / 1000;
			red.goal.x = RED_CORNER_COL;
			red.goal.y = RED_CORNER_ROW;
		}
		if (red.chaseMode) {
			red.goal.x = player->getPosX() / BOXSIZE;
			red.goal.y = player->getPosY() / BOXSIZE;
		}
	}

	findGhostMove(pink);
	ghostAnimate(pink);
	checkCollision(pink);
	pink.secondsInMode = SDL_GetTicks() / 1000 - pink.startTime;
	if (pink.released) {
		if (!pink.chaseMode && pink.secondsInMode == SCATTER_MODE_LENGTH) {
			pink.chaseMode = true;
			pink.startTime = SDL_GetTicks() / 1000;
		}
		else if (pink.chaseMode && pink.secondsInMode == CHASE_MODE_LENGTH) {
			pink.chaseMode = false;
			pink.startTime = SDL_GetTicks() / 1000;
			pink.goal.x = PINK_CORNER_COL;
			pink.goal.y = PINK_CORNER_ROW;
		}
		if (pink.chaseMode) {
			if (direction == "left") {
				pink.goal.x = player->getPosX() / BOXSIZE - 3;
				pink.goal.y = player->getPosY() / BOXSIZE;
			}
			else if (direction == "right") {
				pink.goal.x = player->getPosX() / BOXSIZE + 3;
				pink.goal.y = player->getPosY() / BOXSIZE;
			}
			else if (direction == "up") {
				pink.goal.x = player->getPosX() / BOXSIZE;
				pink.goal.y = player->getPosY() / BOXSIZE - 3;
			}
			else if (direction == "down") {
				pink.goal.x = player->getPosX() / BOXSIZE;
				pink.goal.y = player->getPosY() / BOXSIZE + 3;
			}
			
			
		}
	}
	else if (!pink.released) {
		if (pink.secondsInMode == 1 && pink.goal.x != PINK_CORNER_COL) {
			board.setBoard(14, 12, 0);
			
			pink.startTime = SDL_GetTicks() / 1000;
			pink.goal.x = 14;
			pink.goal.y = 11;
		}
		if (pink.pos.y / BOXSIZE == 11) {
			board.setBoard(14, 12, 1);
			pink.goal.x = PINK_CORNER_COL;
			pink.goal.y = PINK_CORNER_ROW;
			pink.released = true;
			cout << "OUT" << endl;
		}
	}
	

	findGhostMove(orange);
	ghostAnimate(orange);
	checkCollision(orange);
	orange.secondsInMode = SDL_GetTicks() / 1000 - orange.startTime;
	if (orange.released) {
		if (!orange.chaseMode && orange.secondsInMode == SCATTER_MODE_LENGTH) {
			orange.chaseMode = true;
			orange.startTime = SDL_GetTicks() / 1000;
		}
		else if (orange.chaseMode && orange.secondsInMode == CHASE_MODE_LENGTH) {
			orange.chaseMode = false;
			orange.startTime = SDL_GetTicks() / 1000;
			orange.goal.x = ORANGE_CORNER_COL;
			orange.goal.y = ORANGE_CORNER_ROW;
		}
		if (orange.chaseMode) {
			Vec2 dist;
			dist.x = orange.pos.x / BOXSIZE - player->getPosX() / BOXSIZE;
			if (dist.x < 0) dist.x *= -1;
			dist.y = orange.pos.y / BOXSIZE - player->getPosY() / BOXSIZE;
			if (dist.x < 0) dist.x *= -1;
			if (dist.x + dist.y <= 8) {
				orange.goal.x = ORANGE_CORNER_COL;
				orange.goal.y = ORANGE_CORNER_ROW;
			}
			else {
				orange.goal.x = player->getPosX() / BOXSIZE;
				orange.goal.y = player->getPosY() / BOXSIZE;
			}


		}
	}
	else if (!orange.released) {
		if (board.candyEaten() > board.getTotalCandy()/3) {
			board.setBoard(14, 12, 0);
			//cout << "BAMN" << endl;
			orange.startTime = SDL_GetTicks() / 1000;
			orange.goal.x = 14;
			orange.goal.y = 11;
		}
		if (orange.pos.y / BOXSIZE == 11) {
			board.setBoard(14, 12, 1);
			orange.goal.x = ORANGE_CORNER_COL;
			orange.goal.y = ORANGE_CORNER_ROW;
			orange.released = true;
			cout << "OUTA" << endl;

		}
	}


	findGhostMove(blue);
	ghostAnimate(blue);
	checkCollision(blue);
	blue.secondsInMode = SDL_GetTicks() / 1000 - blue.startTime;
	if (blue.released) {
		if (!blue.chaseMode && blue.secondsInMode == SCATTER_MODE_LENGTH) {
			blue.chaseMode = true;
			blue.startTime = SDL_GetTicks() / 1000;
		}
		else if (blue.chaseMode && blue.secondsInMode == CHASE_MODE_LENGTH) {
			blue.chaseMode = false;
			blue.startTime = SDL_GetTicks() / 1000;
			blue.goal.x = BLUE_CORNER_COL;
			blue.goal.y = BLUE_CORNER_ROW;
		}
		if (blue.chaseMode) {
			if (direction == "left" || direction == "right") {
				float goalPosX = 0;
				if(direction == "left") goalPosX = (((player->getPosX() / BOXSIZE) - 2) * BOXSIZE);
				else goalPosX = (((player->getPosX() / BOXSIZE) + 2) * BOXSIZE);
				if (red.pos.x / BOXSIZE > goalPosX / BOXSIZE && red.pos.y / BOXSIZE > player->getPosY() / BOXSIZE) { //4th square
					Vec2 dist;
					dist.x = red.pos.x - goalPosX;
					dist.y = red.pos.y - player->getPosY();
					blue.goal.x = (goalPosX - dist.x)/BOXSIZE;
					blue.goal.y = (player->getPosY() - dist.y) / BOXSIZE;
					//cout << "4th square" << endl;
				}
				else if (red.pos.x / BOXSIZE < goalPosX / BOXSIZE && red.pos.y / BOXSIZE > player->getPosY() / BOXSIZE) { // 3rd square
					Vec2 dist;
					dist.x = goalPosX - red.pos.x;
					dist.y = red.pos.y - player->getPosY();
					blue.goal.x = (goalPosX + dist.x) / BOXSIZE;
					blue.goal.y = (player->getPosY() - dist.y) / BOXSIZE;
					//cout << "3th square" << endl;
				}
				else if (red.pos.x / BOXSIZE < goalPosX / BOXSIZE && red.pos.y / BOXSIZE < player->getPosY() / BOXSIZE) { // 2nd square
					Vec2 dist;
					dist.x = goalPosX - red.pos.x;
					dist.y = player->getPosY() - red.pos.y;
					blue.goal.x = (goalPosX + dist.x) / BOXSIZE;
					blue.goal.y = (player->getPosY() + dist.y) / BOXSIZE;
					//cout << "2nd square" << endl;
				}
				else if (red.pos.x / BOXSIZE > goalPosX / BOXSIZE && red.pos.y / BOXSIZE < player->getPosY() / BOXSIZE) { // 1st square
					Vec2 dist;
					dist.x = red.pos.x - goalPosX;
					dist.y = player->getPosY() - red.pos.y;
					blue.goal.x = (goalPosX - dist.x) / BOXSIZE;
					blue.goal.y = (player->getPosY() + dist.y) / BOXSIZE;
					//cout << "1st square" << endl;
				}
				else if (red.pos.x / BOXSIZE == goalPosX / BOXSIZE && red.pos.y / BOXSIZE < player->getPosY() / BOXSIZE) { // from top
					Vec2 dist;
					dist.x = 0;
					dist.y = player->getPosY() - red.pos.y;
					blue.goal.x = goalPosX/BOXSIZE;
					blue.goal.y = (player->getPosY() + dist.y) / BOXSIZE;
					//cout << "from Top" << endl;
				}
				else if (red.pos.x / BOXSIZE == goalPosX / BOXSIZE && red.pos.y / BOXSIZE > player->getPosY() / BOXSIZE) { // from bottom
					Vec2 dist;
					dist.x = 0;
					dist.y = red.pos.y - player->getPosY();
					blue.goal.x = goalPosX / BOXSIZE;
					blue.goal.y = (player->getPosY() - dist.y) / BOXSIZE;
					//cout << "from Bottom" << endl;
				}
				else if (red.pos.x / BOXSIZE < goalPosX / BOXSIZE && red.pos.y / BOXSIZE == player->getPosY() / BOXSIZE) { // from left
					Vec2 dist;
					dist.x = goalPosX - red.pos.x;
					dist.y = 0;
					blue.goal.x = (goalPosX + dist.x)/BOXSIZE;
					blue.goal.y = player->getPosY() / BOXSIZE;
					//cout << "from Left" << endl;
				}
				else if (red.pos.x / BOXSIZE > goalPosX / BOXSIZE && red.pos.y / BOXSIZE == player->getPosY() / BOXSIZE) { // from right
					Vec2 dist;
					dist.x = red.pos.x - goalPosX;
					dist.y = 0;
					blue.goal.x = (goalPosX - dist.x) / BOXSIZE;
					blue.goal.y = player->getPosY() / BOXSIZE;
					//cout << "from Right" << endl;
				}
			}
			if (direction == "up" || direction == "down") {
				float goalPosY = 0;
				if (direction == "up") goalPosY = (((player->getPosY() / BOXSIZE) - 2) * BOXSIZE);
				else goalPosY = (((player->getPosY() / BOXSIZE) + 2) * BOXSIZE);
				if (red.pos.x / BOXSIZE > player->getPosX() / BOXSIZE && red.pos.y / BOXSIZE > goalPosY / BOXSIZE) { //4th square
					Vec2 dist;
					dist.x = red.pos.x - player->getPosX();
					dist.y = red.pos.y - goalPosY;
					blue.goal.x = (player->getPosX() - dist.x) / BOXSIZE;
					blue.goal.y = (goalPosY - dist.y) / BOXSIZE;
					//cout << "4th square" << endl;
				}
				else if (red.pos.x / BOXSIZE < player->getPosX() / BOXSIZE && red.pos.y / BOXSIZE > goalPosY / BOXSIZE) { // 3rd square
					Vec2 dist;
					dist.x = player->getPosX() - red.pos.x;
					dist.y = red.pos.y - goalPosY;
					blue.goal.x = (player->getPosX() + dist.x) / BOXSIZE;
					blue.goal.y = (goalPosY - dist.y) / BOXSIZE;
					//cout << "3th square" << endl;
				}
				else if (red.pos.x / BOXSIZE < player->getPosX() / BOXSIZE && red.pos.y / BOXSIZE < goalPosY / BOXSIZE) { // 2nd square
					Vec2 dist;
					dist.x = player->getPosX() - red.pos.x;
					dist.y = goalPosY - red.pos.y;
					blue.goal.x = (player->getPosX() + dist.x) / BOXSIZE;
					blue.goal.y = (goalPosY + dist.y) / BOXSIZE;
					//cout << "2nd square" << endl;
				}
				else if (red.pos.x / BOXSIZE > player->getPosX() / BOXSIZE && red.pos.y / BOXSIZE < goalPosY / BOXSIZE) { // 1st square
					Vec2 dist;
					dist.x = red.pos.x - player->getPosX();
					dist.y = goalPosY - red.pos.y;
					blue.goal.x = (player->getPosX() - dist.x) / BOXSIZE;
					blue.goal.y = (goalPosY + dist.y) / BOXSIZE;
					//cout << "1st square" << endl;
				}
				else if (red.pos.x / BOXSIZE == player->getPosX() / BOXSIZE && red.pos.y / BOXSIZE < goalPosY / BOXSIZE) { // from top
					Vec2 dist;
					dist.x = 0;
					dist.y = goalPosY - red.pos.y;
					blue.goal.x = player->getPosX() / BOXSIZE;
					blue.goal.y = (goalPosY + dist.y) / BOXSIZE;
					//cout << "from Top" << endl;
				}
				else if (red.pos.x / BOXSIZE == player->getPosX() / BOXSIZE && red.pos.y / BOXSIZE > goalPosY / BOXSIZE) { // from bottom
					Vec2 dist;
					dist.x = 0;
					dist.y = red.pos.y - goalPosY;
					blue.goal.x = player->getPosX() / BOXSIZE;
					blue.goal.y = (goalPosY - dist.y) / BOXSIZE;
					//cout << "from Bottom" << endl;
				}
				else if (red.pos.x / BOXSIZE < player->getPosX() / BOXSIZE && red.pos.y / BOXSIZE == goalPosY / BOXSIZE) { // from left
					Vec2 dist;
					dist.x = player->getPosX() - red.pos.x;
					dist.y = 0;
					blue.goal.x = (player->getPosX() + dist.x) / BOXSIZE;
					blue.goal.y = goalPosY / BOXSIZE;
					//cout << "from Left" << endl;
				}
				else if (red.pos.x / BOXSIZE > player->getPosX() / BOXSIZE && red.pos.y / BOXSIZE == goalPosY / BOXSIZE) { // from right
					Vec2 dist;
					dist.x = red.pos.x - player->getPosX();
					dist.y = 0;
					blue.goal.x = (player->getPosX() - dist.x) / BOXSIZE;
					blue.goal.y = goalPosY / BOXSIZE;
					//cout << "from Right" << endl;
				}
			}


		}
	}
	else if (!blue.released) {
		if (board.candyEaten()==30) {
			board.setBoard(14, 12, 0);
			blue.startTime = SDL_GetTicks() / 1000;
			blue.goal.x = 14;
			blue.goal.y = 11;
		}
		if (blue.pos.y / BOXSIZE == 11) {
			board.setBoard(14, 12, 1);
			blue.goal.x = BLUE_CORNER_COL;
			blue.goal.y = BLUE_CORNER_ROW;
			blue.released = true;
		}
	}
	
/*	if (board.getValue(player->getPosX() / BOXSIZE, player->getPosY() / BOXSIZE)==2) { // SUPER CANDY
		board.setBoard(player->getPosX() / BOXSIZE, player->getPosY() / BOXSIZE, 0);
		ghostSpeed = GAMESPEED/2;
		scaryMode = true;
		scaryModeStartTime = SDL_GetTicks() / 1000;
	}*/
	if (SDL_GetTicks() / 1000 - scaryModeStartTime >= 7 && scaryMode) {
		scaryMode = false;
		ghostSpeed = GAMESPEED;
	}

	if (player->getPosY() / BOXSIZE == 14) {
		board.setBoard(COLUMNS - 1, player->getPosY() / BOXSIZE, 0);
		board.setBoard(0, player->getPosY() / BOXSIZE, 0);
	}

	if (moving_right && player->getPosX()<dest.x && (!turning || player->getPosX()<turn.x)) {
		board.setBoard(player->getPosX() / BOXSIZE, player->getPosY() / BOXSIZE, 0);
		float newPos = player->getPosX() + player->getSpeed();
		if (newPos / 0.5 != newPos * 2) newPos = (int)newPos;
		player->changePos(newPos, player->getPosY());
		if (newPos / BOXSIZE < COLUMNS) {
			//board.setBoard(newPos / BOXSIZE, player->getPosY() / BOXSIZE, 4);
			board.eatCandy(newPos / BOXSIZE, player->getPosY() / BOXSIZE);
		}
		player->animate("right");
		
	}
	else if (moving_left && player->getPosX() > dest.x && (!turning || player->getPosX()>turn.x)) {
		board.setBoard(player->getPosX() / BOXSIZE, player->getPosY() / BOXSIZE, 0);
		
		float newPos = player->getPosX() - player->getSpeed();
		if (newPos / 0.5 != newPos * 2) newPos = (int)newPos;
		player->changePos(newPos, player->getPosY());
		if (newPos / BOXSIZE >= 0) {
			//board.setBoard(newPos / BOXSIZE, player->getPosY() / BOXSIZE, 4);
			board.eatCandy(newPos / BOXSIZE, player->getPosY() / BOXSIZE);
		}
		player->animate("left");
	}
	else if (moving_up && player->getPosY() > dest.y && (!turning || player->getPosY()>turn.y)) {

		board.setBoard(player->getPosX() / BOXSIZE, player->getPosY() / BOXSIZE, 0);
		float newPos = player->getPosY() - player->getSpeed();
		if (newPos / 0.5 != newPos * 2) newPos = (int)newPos;
		player->changePos(player->getPosX(), newPos);
		//board.setBoard(player->getPosX() / BOXSIZE, newPos / BOXSIZE, 4);
		board.eatCandy(player->getPosX() / BOXSIZE, newPos / BOXSIZE);
		player->animate("up");
	}
	else if (moving_down && player->getPosY() < dest.y && (!turning || player->getPosY()<turn.y)) {
		board.setBoard(player->getPosX() / BOXSIZE, player->getPosY() / BOXSIZE, 0);
		float newPos = player->getPosY() + player->getSpeed();
		if (newPos / 0.5 != newPos * 2) newPos = (int)newPos;
		player->changePos(player->getPosX(), newPos);
		//board.setBoard(player->getPosX() / BOXSIZE, newPos / BOXSIZE, 4);
		board.eatCandy(player->getPosX() / BOXSIZE, newPos / BOXSIZE);
		player->animate("down");
	}
	else {
		if (moving_right && player->getPosX() / BOXSIZE == COLUMNS) {
			player->changePos(0, player->getPosY());
			int destCol = player->getPosX() / BOXSIZE;
			int destRow = player->getPosY() / BOXSIZE;
			board.findAPossibleMove("right", destCol, destRow);
			dest.x = destCol * BOXSIZE;
			
		}
		else if (moving_left && player->getPosX() < 0) {
			player->changePos((COLUMNS-1)*BOXSIZE, player->getPosY());
			int destCol = player->getPosX() / BOXSIZE;
			int destRow = player->getPosY() / BOXSIZE;
			board.findAPossibleMove("left", destCol, destRow);
			dest.x = destCol * BOXSIZE;
			board.eatCandy(COLUMNS - 1, destRow);
			
		}
		else {
			player->animate("none");
			checkMoves();
		}
	}
}

void Game::findDirection() {
	if (moving_down) direction = "down";
	else if (moving_up) direction = "up";
	else if (moving_left) direction = "left";
	else if (moving_right) direction = "right";
} 

void Game::readInput() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
			case SDL_QUIT:
				exitgame = true;
				running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				/*red.goal.x = e.button.x / BOXSIZE;
				red.goal.y = e.button.y / BOXSIZE;
				*/
				cout << blue.goal.x << " " << blue.goal.y<< endl;
				break;
			case SDL_KEYDOWN: {
				int destCol;
				int destRow;
				if (e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT) {

					if (!inAction || moving_left) {
						checkMoves();
						dest.x = player->getPosX();
						dest.y = player->getPosY();
						
						destCol = dest.x / BOXSIZE;
						destRow = dest.y / BOXSIZE;

						board.findAPossibleMove("right", destCol, destRow);
						if (destCol != dest.x / BOXSIZE || destRow != dest.y / BOXSIZE) {
							moving_right = true;
							inAction = true;
							dest.x = destCol * BOXSIZE;
							if (dest.x / BOXSIZE == COLUMNS - 1) {
								dest.x += BOXSIZE;
							}
							dest2.x = dest.x;
							dest2.y = dest.y;
						}
					}
					else {
			
						findDirection();
						if (direction != "right") {
							destCol = player->getPosX() / BOXSIZE;
							destRow = player->getPosY() / BOXSIZE;
							board.findAPossibleTurn(direction, "right", destCol, destRow);
							if (destRow == (int)(player->getPosY() / BOXSIZE) && player->getPosY() != (int)(player->getPosY() / BOXSIZE)*BOXSIZE) {
								if(direction=="down") destRow++;
								//else if (direction == "up") destRow--;
								int tempRow2 = destRow;
								int tempCol2 = destCol;
								board.findAPossibleTurn(direction, "right", destCol, destRow);
								if (destRow == tempRow2 && destCol == tempCol2) {
									if(direction=="down") break;
								}
							}
							if (destCol != (int)player->getPosX() / BOXSIZE || destRow != (int)player->getPosY() / BOXSIZE || board.getValue(destCol + 1, destRow) == 0 ) {
								cout << "comes" << endl;
								dest2.x = dest.x;
								dest2.y = dest.y;
								turn.x = destCol * BOXSIZE;
								turn.y = destRow * BOXSIZE;
								board.findAPossibleMove("right", destCol, destRow);
								if (destCol != dest2.x / BOXSIZE || destRow != dest2.y / BOXSIZE) {
									turning = true;
									nextMove = "right";
									dest2.y = destRow * BOXSIZE;
									dest2.x = destCol * BOXSIZE;
									if (dest2.x / BOXSIZE == COLUMNS - 1) {
										dest2.x += BOXSIZE;
									}
								}
							}
						}
					}
				}
				if (e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) {
					if (!inAction || moving_up) {
						checkMoves();
						dest.x = player->getPosX();
						dest.y = player->getPosY();
						
						destCol = dest.x / BOXSIZE;
						destRow = dest.y / BOXSIZE;

						board.findAPossibleMove("down", destCol, destRow);
						if (destCol != dest.x / BOXSIZE || destRow != dest.y / BOXSIZE) {
							moving_down = true;
							inAction = true;
							dest.y = destRow * BOXSIZE;
							dest2.x = dest.x;
							dest2.y = dest.y;
						}
					}
					else {
						findDirection();
						if (direction != "down") {
							destCol = player->getPosX() / BOXSIZE;
							destRow = player->getPosY() / BOXSIZE;
							board.findAPossibleTurn(direction, "down", destCol, destRow);
							if (destCol == (int)(player->getPosX() / BOXSIZE) && player->getPosX() != (int)(player->getPosX() / BOXSIZE)*BOXSIZE) {
								destCol++;
								int tempCol = destCol;
								int tempRow = destRow;
								board.findAPossibleTurn(direction, "down", destCol, destRow);
								if (destCol == tempCol && destRow == tempRow) {
									break;
								}
							}

							if (destCol != (int)player->getPosX() / BOXSIZE || destRow != (int)player->getPosY() / BOXSIZE || board.getValue(destCol, destRow + 1) == 0) {
								dest2.x = dest.x;
								dest2.y = dest.y;
								turn.x = destCol * BOXSIZE;
								turn.y = destRow * BOXSIZE;
								board.findAPossibleMove("down", destCol, destRow);
								if (destCol != dest2.x / BOXSIZE || destRow != dest2.y / BOXSIZE) {
									turning = true;
									nextMove = "down";
									dest2.y = destRow * BOXSIZE;
									dest2.x = destCol * BOXSIZE;
								}
							}
						}
					}
				}
				
				if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) {
					if (!inAction || moving_right) {
						checkMoves();
						dest.x = player->getPosX();
						dest.y = player->getPosY();
						destCol = dest.x / BOXSIZE;
						destRow = dest.y / BOXSIZE;
						
						board.findAPossibleMove("left", destCol, destRow);
						if (destCol != dest.x / BOXSIZE || destRow != dest.y / BOXSIZE) {
							moving_left = true;
							inAction = true;
							dest.x = destCol * BOXSIZE;
							if (dest.x / BOXSIZE == 0) {
								dest.x -= BOXSIZE;
							}
							dest2.x = dest.x;
							dest2.y = dest.y;
						}

					} else {
						findDirection();
						if (direction != "left") {
							destCol = player->getPosX() / BOXSIZE;
							destRow = player->getPosY() / BOXSIZE;
							board.findAPossibleTurn(direction, "left", destCol, destRow);
							if (destRow == (int)(player->getPosY() / BOXSIZE) && player->getPosY() != (int)(player->getPosY() / BOXSIZE)*BOXSIZE) {
								if (direction == "down") destRow++;
								//else if (direction == "up") destRow--;
								int tempRow2 = destRow;
								int tempCol2 = destCol;
								board.findAPossibleTurn(direction, "left", destCol, destRow);
								if (destRow == tempRow2 && destCol == tempCol2) {
									if (direction == "down") break;
								}
							}
							if (destCol != (int)player->getPosX() / BOXSIZE || destRow != (int)player->getPosY() / BOXSIZE || board.getValue(destCol - 1, destRow) == 0) {
								dest2.x = dest.x;
								dest2.y = dest.y;
								turn.x = destCol * BOXSIZE;
								turn.y = destRow * BOXSIZE;
								board.findAPossibleMove("left", destCol, destRow);
								if (destCol != dest2.x / BOXSIZE || destRow != dest2.y / BOXSIZE) {
									turning = true;
									nextMove = "left";
									dest2.y = destRow * BOXSIZE;
									dest2.x = destCol * BOXSIZE;
									if (dest2.x / BOXSIZE == 0) {
										dest2.x -= BOXSIZE;
									}
								}

							}
						}
					}
				}
				if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) {
					if (!inAction || moving_down) {
						checkMoves();
						dest.x = player->getPosX();
						dest.y = player->getPosY();
						destCol = dest.x / BOXSIZE;
						destRow = dest.y / BOXSIZE;
						
						board.findAPossibleMove("up", destCol, destRow);
						if (destCol != dest.x / BOXSIZE || destRow != dest.y / BOXSIZE) {
							moving_up = true;
							inAction = true;
							dest.y = destRow * BOXSIZE;
							dest2.x = dest.x;
							dest2.y = dest.y;
						}
					} else {
						
						findDirection();
						if (direction != "up") {
							destCol = player->getPosX() / BOXSIZE;
							destRow = player->getPosY() / BOXSIZE;
							board.findAPossibleTurn(direction, "up", destCol, destRow);
							if (destCol == (int)(player->getPosX() / BOXSIZE) && player->getPosX() != (int)(player->getPosX() / BOXSIZE)*BOXSIZE) {
								destCol++;
								int tempCol = destCol;
								int tempRow = destRow;
								board.findAPossibleTurn(direction, "up", destCol, destRow);
								if (destCol == tempCol && destRow == tempRow) {
									break;
								}
							}
							if (destCol != (int)player->getPosX() / BOXSIZE || destRow != (int)player->getPosY() / BOXSIZE || board.getValue(destCol, destRow - 1) == 0) {
								dest2.x = dest.x;
								dest2.y = dest.y;
								turn.x = destCol * BOXSIZE;
								turn.y = destRow * BOXSIZE;
								board.findAPossibleMove("up", destCol, destRow);
								if (destCol != (int)dest2.x / BOXSIZE || destRow != (int)dest2.y / BOXSIZE) {
									turning = true;
									nextMove = "up";
									dest2.y = destRow * BOXSIZE;
									dest2.x = destCol * BOXSIZE;
								}
							}
						}
					}
					

				}
				if (e.key.keysym.sym == SDLK_m) { // map
					board.printBoard();
				}
				if (e.key.keysym.sym == SDLK_g) { // grid
					grid = !grid;
				}
				break;
			}
		


		}




	}


}