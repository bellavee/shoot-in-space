#include "Game.h"

void Game::start()
{
	init();

	while (true)
	{
		dt = timer.GetSystemTimeEx();
		moveShip();
		moveObjects();
		checkLaserAstroColision();
		timeSinceLastAsteroide += dt;
		if (timeSinceLastAsteroide > 10)
		{
			addAsteroide();
			timeSinceLastAsteroide = 0;
		}
	}
}

void Game::init()
{
	srand(time(NULL));
	timer.InitSystemTime();
	// Spawn Ship
	alive = false;
	score = 0;

}

void Game::moveShip()
{
	Vector3f shipPos = ship.getPosition();
	if (keyboard.isKeyPressed('Z'))
	{
		shipPos = shipPos + Vector3f(0, 1, 0);
	}
	if (keyboard.isKeyPressed('Q'))
	{
		shipPos = shipPos + Vector3f(-1, 0, 0);
	}
	if (keyboard.isKeyPressed('S'))
	{
		shipPos = shipPos + Vector3f(0, -1, 0);
	}
	if (keyboard.isKeyPressed('D'))
	{
		shipPos = shipPos + Vector3f(1, 0, 0);
	}
	ship.setPosition(shipPos);
}

void Game::addAsteroide()
{
	float x = rand() % 100;
	float y = rand() % 100;
	float z = rand() % 100;
	Vector3f position = Vector3f(x, y, z);
	Vector3f velocity = Vector3f(0, 0, -1.0);
	std::string mesh = "aste";

	Asteroide aste = Asteroide(mesh, position, velocity, 3);
	asteroides.push_back(aste);
}

void Game::addLaser()
{
	Vector3f position = ship.getPosition();
	Vector3f velocity = Vector3f(0, 0, 1.0);
	std::string mesh = "laser";

	Laser laser = Laser(mesh, position, velocity);
	lasers.push_back(laser);
}

void Game::moveObjects()
{
	for (int i = 0; i < lasers.size(); i++)
	{
		lasers[i].updatePosition(dt);
	}
	for (int i = 0; i < asteroides.size(); i++)
	{
		asteroides[i].updatePosition(dt);
	}
}


void Game::checkLaserAstroColision()
{
	float distance;
	for (int i = 0; i < lasers.size(); i++)
	{
		Vector3f laserPos = lasers[i].getPosition();
		for (int j = 0; j < asteroides.size(); j++)
		{
			distance = asteroides[j].distanceFromPosition(laserPos);
			if (distance < 100)
			{
				asteroides[j].HP--;
				if (asteroides[j].HP == 0)
				{
					for (int z = j; z < asteroides.size() - 1; z++)
					{
						asteroides[j] = asteroides[j + 1];
					}
				}
				for (int z = j; z < lasers.size() - 1; z++)
				{
					lasers[j] = lasers[j + 1];
				}
			}
		}
	}
}