#include "stdafx.h"
#include "scene_Manager.h"

scene_Manager::scene_Manager()
{
}

scene_Manager::~scene_Manager()
{
}

HRESULT scene_Manager::init()
{

	// 씬매니저에 씬을 추가한다.
	SCENEMANAGER->addScene("Intro", new introScene);
	SCENEMANAGER->addScene("Stage", new stageScene);
	SCENEMANAGER->addScene("GameOver", new gameOverScene);
	SCENEMANAGER->addScene("Ending", new Ending);

	// 제일 처음에는 인트로를 보여줘야 한다.
	SCENEMANAGER->set_SceneState(SS_INTRO);

	return S_OK;
}

void scene_Manager::release()
{
}

void scene_Manager::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_F5))
	{
		SCENEMANAGER->set_SceneState(SS_INTRO);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_F6))
	{
		SCENEMANAGER->set_SceneState(SS_STAGE);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_F7))
	{
		SCENEMANAGER->set_SceneState(SS_GAMEOVER);
	}

	SCENEMANAGER->SceneChange();	// 씬 상태에 따라 씬을 바꿔준다.
	SCENEMANAGER->update();
}

void scene_Manager::render()
{
	SCENEMANAGER->render();
}