#include "stdafx.h"
#include "cameraManager.h"

cameraManager::cameraManager()
{
	_camera.cameraSizeX =   NULL;
	_camera.cameraSizeY =   NULL;
	_camera.cameraXY.x =    NULL;
	_camera.cameraXY.y =    NULL;
	_camera.nextCameraXY.x = NULL;
	_camera.nextCameraXY.y = NULL;
	_camera.world_Size.x =  NULL;
	_camera.world_Size.y =  NULL;
	_camera.isMove = false;
	_camera.ClearX = false;
	_camera.ClearY = false;

	_worImage = new image;
	_worImage = IMAGEMANAGER->addImage("worldMap", 3840, 2292);
	_worldDC = _worImage->getMemDC();

	_worObImage = new image;
	_worObImage = IMAGEMANAGER->addImage("worObject", 3840, 2292);
	_worldObjectDC = _worObImage->getMemDC();
}

cameraManager::~cameraManager()
{
}

HRESULT cameraManager::init()
{
	return S_OK;
}

void cameraManager::release()
{
}

void cameraManager::update()
{
}

void cameraManager::render()
{
}

void cameraManager::set_WorldSize(string strKey)
{
	// ������� Ű���� �޾ƿͼ� ���� ����, ������ ũ�⸦ �����Ѵ�.
	_camera.world_Size.x = IMAGEMANAGER->findImage(strKey)->getWidth();
	_camera.world_Size.y = IMAGEMANAGER->findImage(strKey)->getHeight();
}


void cameraManager::set_Camera_XY(int x, int y)
{
	_camera.cameraXY.x = x - _camera.cameraSizeX / 2.f;
	_camera.cameraXY.y = y - _camera.cameraSizeY / 2.f;

	Camera_Correction();	// ī�޶� �� ������ �������� ����ó��
}

void cameraManager::set_Camera_XY(float x, float y)
{
	_camera.cameraXY.x = x - _camera.cameraSizeX / 2.f;
	_camera.cameraXY.y = y - _camera.cameraSizeY / 2.f;

	Camera_Correction();	// ī�޶� �� ������ �������� ����ó��
}

void cameraManager::set_Camera_XY(POINT xy)
{
	_camera.cameraXY.x = xy.x - _camera.cameraSizeX / 2.f;
	_camera.cameraXY.y = xy.y - _camera.cameraSizeY / 2.f;

	Camera_Correction();	// ī�޶� �� ������ �������� ����ó��
}

void cameraManager::set_Camera_XY(POINTFLOAT xy)
{
	_camera.cameraXY.x = xy.x - _camera.cameraSizeX / 2.f;
	_camera.cameraXY.y = xy.y - _camera.cameraSizeY / 2.f;

	Camera_Correction();	// ī�޶� �� ������ �������� ����ó��
}

void cameraManager::set_Camera_XY(RECT rc)
{
	// �޾ƿ� rc�� x, y �߽���ǥ�� ���ؼ�, ī�޶� ������ ���� ��ŭ�� ���ָ�
	// ī�޶��� left, top ��ǥ�� ��������.
	_camera.cameraXY.x = ((rc.right + rc.left) / 2) - _camera.cameraSizeX / 2;
	_camera.cameraXY.y = ((rc.bottom + rc.top) / 2) - _camera.cameraSizeY / 2;

	Camera_Correction();	// ī�޶� �� ������ �������� ����ó��
}

void cameraManager::get_Next_CameraXY(float x, float y, bool isMove)
{
	// ���� ī�޶� ��ǥ�� �޾ƿͼ� �����ϰ�
	// ī�޶� �̵��� �ϴ� bool ���� true�� ���� �ִ´�.
	// ��� �̵��� ���� �Ŀ��� false�� �ٲ�� 
	// �� ���� �������������� �޾Ƽ� ĳ���� ��ǥ�� ��� �����ش�.
	
	_camera.nextCameraXY.x = x - _camera.cameraSizeX / 2;
	_camera.nextCameraXY.y = y - _camera.cameraSizeY / 2;
	_camera.isMove = isMove;
}

void cameraManager::move_Camera()
{
	// true�� ������ ���� �Ǿ����� ���� ī�޶���� �̵��� �����Ѵ�.
	if (_camera.isMove)
	{
		// ���� 0���϶�� ���� ĳ���Ͱ� �����ʿ� �ִ�.
		if (_camera.cameraXY.x - _camera.nextCameraXY.x <= 0)
		{
			if (-(_camera.cameraXY.x - _camera.nextCameraXY.x) > _camera.world_Size.x / 3) _camera.fastMove = true;
			
			if (!_camera.fastMove)	_camera.cameraXY.x += 10;
			if (_camera.fastMove)	_camera.cameraXY.x += 30;
			
			Camera_Correction();	// ī�޶� �� ������ �������� ����ó��

			if (_camera.cameraXY.x - _camera.nextCameraXY.x >= 0)
				_camera.ClearX = true;
		}

		// ���� 0�̻��̶�� ���� ĳ���Ͱ� ���ʿ� �ִ�.
		if (_camera.cameraXY.x - _camera.nextCameraXY.x > 0)
		{
			if ((_camera.cameraXY.x - _camera.nextCameraXY.x) > _camera.world_Size.x / 3) _camera.fastMove = true;

			if (!_camera.fastMove)	_camera.cameraXY.x -= 10;
			if (_camera.fastMove)	_camera.cameraXY.x -= 30;
			
			Camera_Correction();	// ī�޶� �� ������ �������� ����ó��

			if (_camera.cameraXY.x - _camera.nextCameraXY.x <= 0)
				_camera.ClearX = true;
		}

		// ���� 0���϶�� ���� ĳ���Ͱ� �Ʒ��� �ִ�.
		if (_camera.cameraXY.y - _camera.nextCameraXY.y <= 0)
		{
			if (-(_camera.cameraXY.y - _camera.nextCameraXY.y) > _camera.world_Size.y / 3) _camera.fastMove = true;
			
			if (!_camera.fastMove)	_camera.cameraXY.y += 10;
			if (_camera.fastMove)	_camera.cameraXY.y += 30;
			
			Camera_Correction();	// ī�޶� �� ������ �������� ����ó��

			if (_camera.cameraXY.y - _camera.nextCameraXY.y >= 0)
				_camera.ClearY = true;
		}

		// ���� 0���϶�� ���� ĳ���Ͱ� ���� �ִ�.
		if (_camera.cameraXY.y - _camera.nextCameraXY.y > 0)
		{
			if ((_camera.cameraXY.y - _camera.nextCameraXY.y) > _camera.world_Size.y / 3) _camera.fastMove = true;
			
			if (!_camera.fastMove)	_camera.cameraXY.y -= 10;
			if (_camera.fastMove)	_camera.cameraXY.y -= 30;
			
			Camera_Correction();	// ī�޶� �� ������ �������� ����ó��

			if (_camera.cameraXY.y - _camera.nextCameraXY.y <= 0)
				_camera.ClearY = true;
		}

		// ī�޶��� �̵��� ��� ���������� �ٷιٷ� ĳ���͸� ����ٴѴ�.
		if (_camera.ClearX && _camera.ClearY)
		{
			_camera.isMove = false;
			_camera.fastMove = false;
			_camera.ClearX = false;
			_camera.ClearY = false;
		}
		
	}
}

void cameraManager::Camera_Correction()
{
	// ���� ī�޶� ���� �Ѿ���� �Ҷ� ������ ���ش�.
	if (_camera.cameraXY.x < 0) _camera.cameraXY.x = 0;
	if (_camera.cameraXY.y < 0) _camera.cameraXY.y = 0;

	if (_camera.cameraXY.x + _camera.cameraSizeX > _camera.world_Size.x)
		_camera.cameraXY.x = _camera.world_Size.x - _camera.cameraSizeX;
	if (_camera.cameraXY.y + _camera.cameraSizeY > _camera.world_Size.y)
		_camera.cameraXY.y = _camera.world_Size.y - _camera.cameraSizeY;

	if (_camera.nextCameraXY.x < 0) _camera.nextCameraXY.x = 0;
	if (_camera.nextCameraXY.y < 0) _camera.nextCameraXY.y = 0;
	if (_camera.nextCameraXY.x + _camera.cameraSizeX > _camera.world_Size.x)
		_camera.nextCameraXY.x = _camera.world_Size.x - _camera.cameraSizeX;
	if (_camera.nextCameraXY.y + _camera.cameraSizeY > _camera.world_Size.y)
		_camera.nextCameraXY.y = _camera.world_Size.y - _camera.cameraSizeY;
}
