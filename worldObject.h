#pragma once
#include "gameNode.h"
struct tagTrap
{
	RECT rc;			//����ũ������ ������ �浹�� ��Ʈ ũ��
	bool isTraped;		//������ �ε�����
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

