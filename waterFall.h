#pragma once
#include "gameNode.h"
struct tagWaterFall
{
	RECT rc;				//오브젝트(함정/맵 장식 등)
	image* image;
	float x, y;				//오브젝트 좌표
	//프레임 x,y 값
	int frameX;
	int frameY;
};
class waterFall:public gameNode
{
private:
	//■■■■■■■■■■■폭포■■■■■■■■■■■■■■■■■■■■■■
	tagWaterFall _waterFall[3];
	int _frameCount;
public:
	waterFall();
	~waterFall();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();
	virtual void frameWork();
};

