#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}

//초기화는 여기다 해라!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
HRESULT playGround::init()
{
	gameNode::init(true);

	//_wd = new worldMap;
	//_wd->init();
	//
	//_sm = new scene_Manager;
	//_sm->init();
	//
	//
	//_enemyManager = new EnemyManager;
	//_enemyManager->init();

	_sm = new scene_Manager;
	_sm->init();

	return S_OK;
}

//메모리 해제는 여기다 해라!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void playGround::release()
{
	gameNode::release();	
}

//연산은 여기다 해라!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void playGround::update()
{
	gameNode::update();
	//_wd->update();
	//
	//_sm->update();
	//
	//_enemyManager->update();
	//
	//if(SCENEMANAGER->get_SceneState() == SS_STAGE)	_pm->update();

	_sm->update();

}

//여기다 그려라!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void playGround::render()
{
	PatBlt(CAMERAMANAGER->getWorDC(), 0, 0, CAMERAMANAGER->get_WorldSize_X(), CAMERAMANAGER->get_WorldSize_Y(), WHITENESS);
	//==================================================

	//TIMEMANAGER->render(getMemDC());

	//_wd->render();
	//
	//_pm->render();
	//
	//
	//// 191229 PM 19:11 -락훈 - WorldDc 케릭터-> PG로 렌더링부분 옮김 
	//
	//_enemyManager->render();
	//
	//_sm->render();
	
	_sm->render();
	//===================================================
	//딱 말했다
	_backBuffer->render(getHDC(), 0, 0);
}
