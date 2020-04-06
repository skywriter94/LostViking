#pragma once
#include "gameNode.h"
#include "introScene.h"
#include "stageScene.h"
#include "gameOverScene.h"
#include "Ending.h"


class scene_Manager : public gameNode
{	
	


public:
	scene_Manager();
	~scene_Manager();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};

// 캐릭터 살아날때 번개 크기 다시 따야한다.
// 캐릭터의 크기도 다시 따야 한다.
// 망했다