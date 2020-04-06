#pragma once
#include "singletonBase.h"

// 플레이어 클래스 전방 선언?

struct _tagCamera
{
	POINTFLOAT world_Size, cameraXY, nextCameraXY;		// 월드 사이즈, 카메라 좌표를 담을곳, 다음 카메라 좌표
	int		   cameraSizeX, cameraSizeY;				// 카메라의 사이즈
	bool	   isMove;
	bool       fastMove;
	bool	   ClearX, ClearY;
};

class cameraManager : public singletonBase<cameraManager>
{
private:
	_tagCamera  _camera;

	HDC			_worldDC;  // 플레이어, 적, 함정, 아이템을 그려줄 DC
	image*		_worImage;

	HDC			_worldObjectDC;	// 배경 오브젝트를 그려줄 DC
	image*		_worObImage;

public:
	cameraManager();
	~cameraManager();

	HRESULT init();
	void release();
	void update();
	void render();

	// 월드 사이즈
	void  set_WorldSize(string strKey);							// 월드 사이즈를 세팅한다.
	float get_WorldSize_X() { return _camera.world_Size.x; }	// 월드 사이즈 X를 가져온다.
	float get_WorldSize_Y() { return _camera.world_Size.y; }	// 월드 사이즈 Y를 가져온다.

	// 카메라 사이즈
	void set_CameraSize(int x, int y) { _camera.cameraSizeX = x; _camera.cameraSizeY = y; };	// 카메라의 사이즈를 세팅한다.
	int  get_CameraSizeX() { return _camera.cameraSizeX; }										// 카메라 사이즈 x를 가져온다.
	int  get_CameraSizeY() { return _camera.cameraSizeY; }										// 카메라 사이즈 y를 가져온다.

	// 카메라의 xy 좌표
	void set_Camera_XY(int x, int y);							// 캐릭터의 좌표를 세팅한다.
	void set_Camera_XY(float x, float y);						// 캐릭터의 좌표를 세팅한다.
	void set_Camera_XY(POINT xy);								// 캐릭터의 좌표를 세팅한다.
	void set_Camera_XY(POINTFLOAT xy);							// 캐릭터의 좌표를 세팅한다.
	void set_Camera_XY(RECT rc);								// 캐릭터의 좌표를 세팅한다.

	float get_Camera_X() { return _camera.cameraXY.x; }			// 카메라 X좌표를 가져온다.
	float get_Camera_Y() { return _camera.cameraXY.y; }			// 카메라 y좌표를 가져온다.
	bool get_Camera_Move() { return _camera.isMove; }			// 카메라가 움직이는 중인지 아닌지

	// 다음 카메라의 좌표를 받는 함수
	void get_Next_CameraXY(float x, float y, bool isMove);
	void move_Camera();

	// 카메라가 맵 밖으로 나가지 못하게
	void Camera_Correction();									// 카메라 예외처리 함수

	// 월드 DC
	HDC  getWorDC() { return _worldDC; }						// _worldDC를 가져온다.
	image* get_WorImage() { return _worImage; }

	// 월드 오브젝트 DC
	HDC getWorOBDC() { return _worldObjectDC; }
	image* get_worObImage() { return _worObImage; }
};

// 처음 시작은 에릭의 좌표를 받아와서 바로 카메라 셋팅을한다
// 두번째 부터는 그 캐릭터의 좌표를 저장하고
// 첫 캐릭터와 두번째 캐릭터 좌표를 비교하여 천천히 카메라를 이동시킨다.
// 카메라의 좌표에 도착하면 더이상 이동하지 않고 멈춘다.
