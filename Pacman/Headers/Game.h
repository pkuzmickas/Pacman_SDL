#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Player.h"
#include "Board.h"
#include "Definitions.h"
#include <vector>

using namespace std;


class Game {
private:
	bool running = false;
	bool exitgame = false;
	bool moving_left = false;
	bool moving_right = false;
	bool moving_up = false;
	bool moving_down = false;
	bool grid = false;
	bool inAction = false;
	bool turning = false;
	bool scaryMode = false;

	void readInput();
	void draw();
	void update();
	void checkMoves();
	void findDirection();
	

	struct Vec2 { float x; float y; };
	Vec2 dest;
	Vec2 dest2;
	Vec2 turn;

	string nextMove; // "none" "left" "right" "up" "down"
	string direction;

	Board board;
	SDL_Renderer* gRenderer;
	Player* player;
	SDL_Texture* background;
	SDL_Texture* scarySprite;
	SDL_Rect scaryRect;
	struct Ghost {
		SDL_Texture* sprite;
		SDL_Texture* originalSprite;
		SDL_Rect posRect;
		SDL_Rect spriteSheetRect;
		bool chaseMode;
		bool moving;
		bool released;
		bool dead;
		Vec2 dest;
		Vec2 pos;
		Vec2 goal;
		std::string direction;
		int secondsInMode;
		int startTime;
	};

	Ghost red, pink, blue, orange;

	void createGhost(Ghost &ghost);
	void findGhostMove(Ghost &ghost);
	void ghostMove(Ghost &ghost);
	void ghostAnimate(Ghost &ghost);
	void checkCollision(Ghost &ghost);
	float vecDistance(Vec2 p, Vec2 q);
	float ghostSpeed;
	int scaryModeStartTime;

public:
	int startGame();
	Game(SDL_Renderer* renderer);
	
};