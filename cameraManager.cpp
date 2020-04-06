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
	// 월드맵의 키값을 받아와서 맵의 가로, 세로의 크기를 저장한다.
	_camera.world_Size.x = IMAGEMANAGER->findImage(strKey)->getWidth();
	_camera.world_Size.y = IMAGEMANAGER->findImage(strKey)->getHeight();
}


void cameraManager::set_Camera_XY(int x, int y)
{
	_camera.cameraXY.x = x - _camera.cameraSizeX / 2.f;
	_camera.cameraXY.y = y - _camera.cameraSizeY / 2.f;

	Camera_Correction();	// 카메라가 맵 밖으로 나갔을때 예외처리
}

void cameraManager::set_Camera_XY(float x, float y)
{
	_camera.cameraXY.x = x - _camera.cameraSizeX / 2.f;
	_camera.cameraXY.y = y - _camera.cameraSizeY / 2.f;

	Camera_Correction();	// 카메라가 맵 밖으로 나갔을때 예외처리
}

void cameraManager::set_Camera_XY(POINT xy)
{
	_camera.cameraXY.x = xy.x - _camera.cameraSizeX / 2.f;
	_camera.cameraXY.y = xy.y - _camera.cameraSizeY / 2.f;

	Camera_Correction();	// 카메라가 맵 밖으로 나갔을때 예외처리
}

void cameraManager::set_Camera_XY(POINTFLOAT xy)
{
	_camera.cameraXY.x = xy.x - _camera.cameraSizeX / 2.f;
	_camera.cameraXY.y = xy.y - _camera.cameraSizeY / 2.f;

	Camera_Correction();	// 카메라가 맵 밖으로 나갔을때 예외처리
}

void cameraManager::set_Camera_XY(RECT rc)
{
	// 받아온 rc의 x, y 중심좌표를 구해서, 카메라 사이즈 절반 만큼을 빼주면
	// 카메라의 left, top 좌표가 구해진다.
	_camera.cameraXY.x = ((rc.right + rc.left) / 2) - _camera.cameraSizeX / 2;
	_camera.cameraXY.y = ((rc.bottom + rc.top) / 2) - _camera.cameraSizeY / 2;

	Camera_Correction();	// 카메라가 맵 밖으로 나갔을때 예외처리
}

void cameraManager::get_Next_CameraXY(float x, float y, bool isMove)
{
	// 다음 카메라 좌표를 받아와서 저장하고
	// 카메라 이동을 하는 bool 값에 true의 값을 넣는다.
	// 모든 이동이 끝난 후에는 false로 바뀌고 
	// 그 값을 스테이지씬에서 받아서 캐릭터 좌표를 계속 보내준다.
	
	_camera.nextCameraXY.x = x - _camera.cameraSizeX / 2;
	_camera.nextCameraXY.y = y - _camera.cameraSizeY / 2;
	_camera.isMove = isMove;
}

void cameraManager::move_Camera()
{
	// true의 값으로 변경 되었으면 다음 카메라까지 이동을 시작한다.
	if (_camera.isMove)
	{
		// 만약 0이하라면 다음 캐릭터가 오른쪽에 있다.
		if (_camera.cameraXY.x - _camera.nextCameraXY.x <= 0)
		{
			if (-(_camera.cameraXY.x - _camera.nextCameraXY.x) > _camera.world_Size.x / 3) _camera.fastMove = true;
			
			if (!_camera.fastMove)	_camera.cameraXY.x += 10;
			if (_camera.fastMove)	_camera.cameraXY.x += 30;
			
			Camera_Correction();	// 카메라가 맵 밖으로 나갔을때 예외처리

			if (_camera.cameraXY.x - _camera.nextCameraXY.x >= 0)
				_camera.ClearX = true;
		}

		// 만약 0이상이라면 다음 캐릭터가 왼쪽에 있다.
		if (_camera.cameraXY.x - _camera.nextCameraXY.x > 0)
		{
			if ((_camera.cameraXY.x - _camera.nextCameraXY.x) > _camera.world_Size.x / 3) _camera.fastMove = true;

			if (!_camera.fastMove)	_camera.cameraXY.x -= 10;
			if (_camera.fastMove)	_camera.cameraXY.x -= 30;
			
			Camera_Correction();	// 카메라가 맵 밖으로 나갔을때 예외처리

			if (_camera.cameraXY.x - _camera.nextCameraXY.x <= 0)
				_camera.ClearX = true;
		}

		// 만약 0이하라면 다음 캐릭터가 아래에 있다.
		if (_camera.cameraXY.y - _camera.nextCameraXY.y <= 0)
		{
			if (-(_camera.cameraXY.y - _camera.nextCameraXY.y) > _camera.world_Size.y / 3) _camera.fastMove = true;
			
			if (!_camera.fastMove)	_camera.cameraXY.y += 10;
			if (_camera.fastMove)	_camera.cameraXY.y += 30;
			
			Camera_Correction();	// 카메라가 맵 밖으로 나갔을때 예외처리

			if (_camera.cameraXY.y - _camera.nextCameraXY.y >= 0)
				_camera.ClearY = true;
		}

		// 만약 0이하라면 다음 캐릭터가 위에 있다.
		if (_camera.cameraXY.y - _camera.nextCameraXY.y > 0)
		{
			if ((_camera.cameraXY.y - _camera.nextCameraXY.y) > _camera.world_Size.y / 3) _camera.fastMove = true;
			
			if (!_camera.fastMove)	_camera.cameraXY.y -= 10;
			if (_camera.fastMove)	_camera.cameraXY.y -= 30;
			
			Camera_Correction();	// 카메라가 맵 밖으로 나갔을때 예외처리

			if (_camera.cameraXY.y - _camera.nextCameraXY.y <= 0)
				_camera.ClearY = true;
		}

		// 카메라의 이동이 모두 끝났을때는 바로바로 캐릭터를 따라다닌다.
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
	// 만약 카메라가 맵을 넘어가려고 할때 보정을 해준다.
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
