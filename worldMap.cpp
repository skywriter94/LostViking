#include "stdafx.h"
#include "worldMap.h"


worldMap::worldMap()
{
}


worldMap::~worldMap()
{
}

HRESULT worldMap::init()
{
	_torch.image = IMAGEMANAGER->addFrameImage("StandTorch", "./image/ImageCollection/StandTorch.bmp", 434, 159, 7, 1, true, RGB(255, 0, 255));
	_torch.frameX = 0;
	_torch.frameY = 0;
	IMAGEMANAGER->addImage("BG", "./image/Map/secondTest.bmp", 3840, 2292, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Background", "./image/Map/blackground.bmp", 3840, 2292, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("OBJECT", "./image/Map/first.bmp", 3840, 2292, true, RGB(255, 0, 255));

	// 191229 PM 02:56 월드 좌표와 카메라의 크기를 셋팅해준다. + 월드 DC
	CAMERAMANAGER->set_WorldSize("BG");
	CAMERAMANAGER->set_CameraSize(960, 573);

	_wo = new worldObjects;
	_wo->init();

	return S_OK;
}

void worldMap::release()
{
}

void worldMap::update()
{
	_frameCount++;
	frameWork();
	_wo->update();
}

void worldMap::render()
{
	// 191229 PM 02:09 보여지는 부분 카메라 좌표를 수정했음 -형길-
	// 191229 PM 19:11 -락훈 - 렉 때문에 blackground를 지우고 patBlt로 카메라 배경만큼 그리도록 함 
    PatBlt(CAMERAMANAGER->getWorDC(), CAMERAMANAGER->get_Camera_X(), CAMERAMANAGER->get_Camera_Y(), WINSIZEX, WINSIZEY, BLACKNESS);
	torchDraw();
	IMAGEMANAGER->findImage("Background")->render(CAMERAMANAGER->getWorDC(), 0, 0);
	//IMAGEMANAGER->findImage("OBJECT")->render(CAMERAMANAGER->getWorDC(), 0, 0);
	_wo->render();
	if (KEYMANAGER->isStayKeyDown('Q'))
	{
		IMAGEMANAGER->findImage("BG")->render(CAMERAMANAGER->getWorDC(), 0, 0);
	}
}

void worldMap::frameWork()
{
	if (_frameCount % 6 == 0)
	{
		_torch.image->setFrameX(_torch.frameX);
		_torch.frameX++;
		if (_torch.frameX > _torch.image->getMaxFrameX()) { _torch.frameX = 0; }
	}
}

void worldMap::torchDraw()
{
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		173, 120, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		365, 120, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		749, 120, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		1037, 312, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		1421, 312, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		1901, 120, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		2717, 120, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		2981, 120, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		3149, 120, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		3461, 120, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		3197, 1176, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		3509, 1176, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		3725, 1176, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		3725, 1512, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		3485, 2040, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		2789, 552, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		2429, 552, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		2045, 936, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		1925, 936, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		125, 1656, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		245, 1656, _torch.frameX, _torch.frameY);
	IMAGEMANAGER->findImage("StandTorch")->frameRender(CAMERAMANAGER->getWorDC(),
		173, 648, _torch.frameX, _torch.frameY);
}