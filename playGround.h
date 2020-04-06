#pragma once

#include "gameNode.h"
#include "worldMap.h"
#include "scene_Manager.h"
#include "playerManager.h" // 플레이어
#include"enemyManager.h"
#include "scene_Manager.h"
#include "effect.h"
class playGround : public gameNode
{
private:
	worldMap* _wd;
	//scene_Manager* _sm;
	playerManager* _pm;   // 플레이어 매니져 
	EnemyManager* _enemyManager;

	

	scene_Manager* _sm;

public:
	playGround();
	~playGround();

	virtual HRESULT init();			//초기화 전용 함수
	virtual void release();			//메모리 해제 함수
	virtual void update();			//연산 전용 함수
	virtual void render();			//그리기 전용 함수

};

