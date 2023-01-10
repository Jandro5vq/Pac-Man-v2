#include "Enemy.h"

Enemy::Enemy(COORD spawn) 
{	
	position = spawn;
}

bool Enemy::Logic(Map* pacman_map, COORD PlayerPosition)
{
	int dir = rand() % 4;
	COORD newPos = position;
	switch (dir)
	{
	case 0:
		newPos.X++;
		break;
	case 1:
		newPos.X--;
		break;
	case 2:
		newPos.Y++;
		break;
	case 3:
		newPos.Y--;
		break;
	}

	if (newPos.X < 0)
	{
		newPos.X = pacman_map->Width - 1;
	}
	newPos.X %= pacman_map->Width;
	if (newPos.X < 0)
	{
		newPos.Y = pacman_map->Height - 1;
	}
	newPos.Y %= pacman_map->Height;

	if (pacman_map->GetTile(newPos.X, newPos.Y) != Map::MAP_WALL)
	{
		position = newPos;
	}

	return position.X == PlayerPosition.X && position.Y == PlayerPosition.Y;
}

void Enemy::Draw() 
{
	ConsoleUtils::Console_SetPos(position);
	ConsoleUtils::Console_SetColor(foreground, background);
	std::cout << character;
}