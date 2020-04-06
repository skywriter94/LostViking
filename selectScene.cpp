#include "stdafx.h"
#include "selectScene.h"


selectScene::selectScene()
{
}


selectScene::~selectScene()
{
}

HRESULT selectScene::init()
{
	_btnSceneChange = new button;
	_btnSceneChange->init("¹öÆ°", WINSIZEX / 2, WINSIZEY / 2,
		PointMake(0, 1), PointMake(0, 0), cbSceneChange);

	return S_OK;
}

void selectScene::release()
{
}

void selectScene::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_F1)) SCENEMANAGER->changeScene("½ºÅ¸¾À");

	_btnSceneChange->update();
}

void selectScene::render()
{
	char str[128];
	sprintf_s(str, "¼¿·ºÆ®¾À");
	TextOut(getMemDC(), WINSIZEX / 2 - 200, WINSIZEY / 2, str, strlen(str));

	_btnSceneChange->render();
}

void selectScene::cbSceneChange()
{
	SCENEMANAGER->changeScene("½ºÅ¸¾À");
}
