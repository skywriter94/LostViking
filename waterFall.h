#pragma once
#include "gameNode.h"
struct tagWaterFall
{
	RECT rc;				//������Ʈ(����/�� ��� ��)
	image* image;
	float x, y;				//������Ʈ ��ǥ
	//������ x,y ��
	int frameX;
	int frameY;
};
class waterFall:public gameNode
{
private:
	//���������������������������������������
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

