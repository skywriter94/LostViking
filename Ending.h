#pragma once
#include "gameNode.h"

class Ending : public gameNode
{
public:
	Ending();
	~Ending();

	virtual HRESULT init();
	virtual void relase();
	virtual void update();
	virtual void render();
};

