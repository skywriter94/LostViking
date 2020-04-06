#pragma once
#include "gameNode.h"
#include "worldObjects.h"
struct tagTorch
{
	image* image;
	int frameX;
	int frameY;
};
class worldMap : public gameNode
{

private:
	worldObjects* _wo;
	tagTorch _torch;
	int _frameCount;
public:
	worldMap();
	~worldMap();
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void frameWork();
	virtual void torchDraw();

	worldObjects* getWorldObject() { return _wo; }

};

