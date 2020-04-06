#pragma once
#include"enemy.h"
class Enemy_Snake :public Enemy
{
private:
	RECT _bullet;
public:
	void EnemyAction();
	void Frame();

};

