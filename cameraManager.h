#pragma once
#include "singletonBase.h"

// �÷��̾� Ŭ���� ���� ����?

struct _tagCamera
{
	POINTFLOAT world_Size, cameraXY, nextCameraXY;		// ���� ������, ī�޶� ��ǥ�� ������, ���� ī�޶� ��ǥ
	int		   cameraSizeX, cameraSizeY;				// ī�޶��� ������
	bool	   isMove;
	bool       fastMove;
	bool	   ClearX, ClearY;
};

class cameraManager : public singletonBase<cameraManager>
{
private:
	_tagCamera  _camera;

	HDC			_worldDC;  // �÷��̾�, ��, ����, �������� �׷��� DC
	image*		_worImage;

	HDC			_worldObjectDC;	// ��� ������Ʈ�� �׷��� DC
	image*		_worObImage;

public:
	cameraManager();
	~cameraManager();

	HRESULT init();
	void release();
	void update();
	void render();

	// ���� ������
	void  set_WorldSize(string strKey);							// ���� ����� �����Ѵ�.
	float get_WorldSize_X() { return _camera.world_Size.x; }	// ���� ������ X�� �����´�.
	float get_WorldSize_Y() { return _camera.world_Size.y; }	// ���� ������ Y�� �����´�.

	// ī�޶� ������
	void set_CameraSize(int x, int y) { _camera.cameraSizeX = x; _camera.cameraSizeY = y; };	// ī�޶��� ����� �����Ѵ�.
	int  get_CameraSizeX() { return _camera.cameraSizeX; }										// ī�޶� ������ x�� �����´�.
	int  get_CameraSizeY() { return _camera.cameraSizeY; }										// ī�޶� ������ y�� �����´�.

	// ī�޶��� xy ��ǥ
	void set_Camera_XY(int x, int y);							// ĳ������ ��ǥ�� �����Ѵ�.
	void set_Camera_XY(float x, float y);						// ĳ������ ��ǥ�� �����Ѵ�.
	void set_Camera_XY(POINT xy);								// ĳ������ ��ǥ�� �����Ѵ�.
	void set_Camera_XY(POINTFLOAT xy);							// ĳ������ ��ǥ�� �����Ѵ�.
	void set_Camera_XY(RECT rc);								// ĳ������ ��ǥ�� �����Ѵ�.

	float get_Camera_X() { return _camera.cameraXY.x; }			// ī�޶� X��ǥ�� �����´�.
	float get_Camera_Y() { return _camera.cameraXY.y; }			// ī�޶� y��ǥ�� �����´�.
	bool get_Camera_Move() { return _camera.isMove; }			// ī�޶� �����̴� ������ �ƴ���

	// ���� ī�޶��� ��ǥ�� �޴� �Լ�
	void get_Next_CameraXY(float x, float y, bool isMove);
	void move_Camera();

	// ī�޶� �� ������ ������ ���ϰ�
	void Camera_Correction();									// ī�޶� ����ó�� �Լ�

	// ���� DC
	HDC  getWorDC() { return _worldDC; }						// _worldDC�� �����´�.
	image* get_WorImage() { return _worImage; }

	// ���� ������Ʈ DC
	HDC getWorOBDC() { return _worldObjectDC; }
	image* get_worObImage() { return _worObImage; }
};

// ó�� ������ ������ ��ǥ�� �޾ƿͼ� �ٷ� ī�޶� �������Ѵ�
// �ι�° ���ʹ� �� ĳ������ ��ǥ�� �����ϰ�
// ù ĳ���Ϳ� �ι�° ĳ���� ��ǥ�� ���Ͽ� õõ�� ī�޶� �̵���Ų��.
// ī�޶��� ��ǥ�� �����ϸ� ���̻� �̵����� �ʰ� �����.
