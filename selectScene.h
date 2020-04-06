#pragma once
#include "gameNode.h"
#include "button.h"

class selectScene : public gameNode
{
private:
	button* _btnSceneChange;

public:
	selectScene();
	~selectScene();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	static void cbSceneChange();
};

