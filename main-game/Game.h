#pragma once
#include "Core/Laser.h"
#include "Core/Asteroide.h"
#include "Core/Ship.h"
#include "Core/Keyboard.h"

class Game
{
	bool alive;
	int score;
	float dt;
	float timeSinceLastAsteroide = 0;

	std::vector<Laser> lasers;
	std::vector<Asteroide> asteroides;
	Ship ship;

	Timer timer;
	Keyboard keyboard;
public:
	Game();
	void start();
	void init();
	void moveShip();
	void addAsteroide();
	void addLaser();
	void moveObjects();
	void checkLaserAstroColision();
};