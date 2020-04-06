#pragma once
#include"enemy.h"
class Enemy_Scorpion :public Enemy
{
private:
	RECT _bullet;

public:
	void EnemyAction();
	void Frame();

};

