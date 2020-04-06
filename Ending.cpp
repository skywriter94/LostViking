#include "stdafx.h"
#include "Ending.h"

Ending::Ending()
{
}

Ending::~Ending()
{
}

HRESULT Ending::init()
{
	IMAGEMANAGER->addImage("Clear", "./image/Map/Clear.bmp", WINSIZEX, WINSIZEY, false, RGB(0, 0, 0));
	return S_OK;
}

void Ending::relase()
{
}

void Ending::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		SCENEMANAGER->set_SceneState(SS_INTRO);
	}
}

void Ending::render()
{
	IMAGEMANAGER->findImage("Clear")->render(getMemDC(),
		0, 0);
}

