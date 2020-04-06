#pragma once
#include "gameNode.h"
struct tagTrap
{
	RECT rc;			//함정크기이자 함정에 충돌할 렉트 크기
	bool isTraped;		//함정에 부딛혔나
};

class worldObject:public gameNode
{
private:

	tagTrap _niddle;
	tagTrap _poision;

public:
	worldObject();
	~worldObject();

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();


};

