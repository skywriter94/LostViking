#pragma once
#include"gameNode.h"
#include"Enemy_Mummy.h"
#include"Enemy_Scorpion.h"
#include"Enemy_Snake.h"
#include"Enemy_PlayerMummy.h"
#include"Enemy_Bullet.h"
#include"worldObjects.h"
#include<vector>
#include "effect.h"

using namespace std;

class playerManager;


class EnemyManager : public gameNode
{
private:
	typedef vector<Enemy*>					vEnemy;
	typedef vector<Enemy*>::iterator		viEnemy; 

private:
	//Enemy클래스 벡터로 선언
	vEnemy _vEnemy;
	viEnemy _viEnemy;
	Enemy_Bullet* _enemyBullet;
	worldObjects* _worldObjects;

	playerManager* _playerManager;
	RECT _ericRect;
	RECT _olafRect;
	RECT _baleogRect;
	int _playerMirraCount;
	bool _playerMirra;

	effect* _enemyEffect;
	vector<effect*>						_vEffect;
	vector<effect*>::iterator		   _viEffect;
public:
	EnemyManager();
	~EnemyManager();

	HRESULT init();
	void release();
	void update();
	void render();

	void EnemyCreate();
	void EnemyCreate(float x, float y);
	void EnemyRemove();
	void EnemyRemove(int Num) 
	{
		createEffect(_vEnemy[Num]->getX(), _vEnemy[Num]->getY() - 15
		
		);
		_vEnemy.erase(_vEnemy.begin() + Num);
	}
	void Collision();
	void AddressLink(playerManager* pm) { _playerManager = pm; }
	
	void bulletFire();
	void createEffect(float x,float y);
	vector<Enemy*> getVEnemy() { return _vEnemy; }
	vector<Enemy*>::iterator getViEnemy() { return _viEnemy; }
};