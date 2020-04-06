#pragma once

#include "gameNode.h"
#include "worldMap.h"
#include "scene_Manager.h"
#include "playerManager.h" // �÷��̾�
#include"enemyManager.h"
#include "scene_Manager.h"
#include "effect.h"
class playGround : public gameNode
{
private:
	worldMap* _wd;
	//scene_Manager* _sm;
	playerManager* _pm;   // �÷��̾� �Ŵ��� 
	EnemyManager* _enemyManager;

	

	scene_Manager* _sm;

public:
	playGround();
	~playGround();

	virtual HRESULT init();			//�ʱ�ȭ ���� �Լ�
	virtual void release();			//�޸� ���� �Լ�
	virtual void update();			//���� ���� �Լ�
	virtual void render();			//�׸��� ���� �Լ�

};

