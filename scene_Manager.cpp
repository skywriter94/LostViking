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

	// ¾À¸Å´ÏÀú¿¡ ¾ÀÀ» Ãß°¡ÇÑ´Ù.
	SCENEMANAGER->addScene("Intro", new introScene);
	SCENEMANAGER->addScene("Stage", new stageScene);
	SCENEMANAGER->addScene("GameOver", new gameOverScene);
	SCENEMANAGER->addScene("Ending", new Ending);

	// Á¦ÀÏ Ã³À½¿¡´Â ÀÎÆ®·Î¸¦ º¸¿©Áà¾ß ÇÑ´Ù.
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

	SCENEMANAGER->SceneChange();	// ¾À »óÅÂ¿¡ µû¶ó ¾ÀÀ» ¹Ù²ãÁØ´Ù.
	SCENEMANAGER->update();
}

void scene_Manager::render()
{
	SCENEMANAGER->render();
}