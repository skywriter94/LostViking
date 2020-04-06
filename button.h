#pragma once
#include "gameNode.h"

//콜백함수 == 함수포인터
typedef void(*CALLBACK_FUNCTION)(void);

//<Functional> 라이브러리를 사용하면 글로벌 영역에 선언한 콜백함수에 매개변수를 사용할 수 있음.
//std::bind, std::move, std::Function 

//callback & delegate <--- 차이점을 찾아보세염

//Lambda식 

enum BUTTONDIRECTION
{
	BUTTONDIRECTION_NULL,
	BUTTONDIRECTION_UP,
	BUTTONDIRECTION_DOWN
};

class button : public gameNode
{
private:
	BUTTONDIRECTION _direction;

	const char* _imageName;
	image* _image;
	RECT _rc;

	float _x, _y;
	POINT _btnDownFramePoint;
	POINT _btnUpFramePoint;

	CALLBACK_FUNCTION _callbackFunction;

public:
	button();
	~button();

	HRESULT init(const char* imageName, int x, int y,
		POINT btnDownFramePoint, POINT btnUpFramePoint,
		CALLBACK_FUNCTION cbFunction);

	void release();
	void update();
	void render();
};

