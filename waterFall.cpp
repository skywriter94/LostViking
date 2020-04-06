#include "stdafx.h"
#include "waterFall.h"

waterFall::waterFall()
{
}

waterFall::~waterFall()
{
}

HRESULT waterFall::init()
{	//■■■■■■■■■■■■■■■■■폭포 위치 렉트■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	IMAGEMANAGER->addFrameImage("Water_Fall", "./image/ImageCollection/water_Fall.bmp", 564, 602, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("long_Water_Fall", "./image/ImageCollection/long_Water_Fall.bmp", 1152, 1467, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("short_Water_Fall", "./image/ImageCollection/short_Water_Fall.bmp", 564, 622, 4, 1, true, RGB(255, 0, 255));
	_waterFall[0].image = IMAGEMANAGER->findImage("Water_Fall");
	_waterFall[1].image = IMAGEMANAGER->findImage("long_Water_Fall");
	_waterFall[2].image = IMAGEMANAGER->findImage("short_Water_Fall");
	for (int i = 0; i < 3; i++)
	{
		_waterFall[i].frameX = 0;
		_waterFall[i].frameY = 0;
	}
	_frameCount = 0;
	return S_OK;
}

void waterFall::update()
{
	_frameCount++;
	frameWork();
}

void waterFall::release()
{
}

void waterFall::render()
{
	//■■■■■■■■■■■■■■■■■폭포 이미지 출력■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	IMAGEMANAGER->findImage("Water_Fall")->frameRender(CAMERAMANAGER->getWorDC(),
		1150, 148, _waterFall[0].frameX, _waterFall[0].frameY);
	IMAGEMANAGER->findImage("long_Water_Fall")->frameRender(CAMERAMANAGER->getWorDC(),
		815, 832, _waterFall[1].frameX, _waterFall[1].frameY);
	IMAGEMANAGER->findImage("short_Water_Fall")->frameRender(CAMERAMANAGER->getWorDC(),
		3263, -60, _waterFall[2].frameX, _waterFall[2].frameY);
}

void waterFall::frameWork()
{
	//■■■■■■■■■■■■■■■■■폭포 프레임■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	for (int i = 0; i < 3; i++)
	{
		if (_frameCount % 4 == 0)
		{
			_waterFall[i].image->setFrameX(_waterFall[i].frameX);
			_waterFall[i].frameX++;
			if (_waterFall[i].frameX > _waterFall[i].image->getMaxFrameX()) { _waterFall[i].frameX = 0; }
		}
	}
}
