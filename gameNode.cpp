#include "stdafx.h"
#include "gameNode.h"


gameNode::gameNode()
{
}


gameNode::~gameNode()
{
}

HRESULT gameNode::init()
{

	_hdc = GetDC(_hWnd);
	_managerInit = false;

	
	return S_OK;
}

HRESULT gameNode::init(bool managerInit)
{
	_hdc = GetDC(_hWnd);
	_managerInit = managerInit;

	if (_managerInit)
	{
	
		KEYMANAGER->init();
		IMAGEMANAGER->init();
		TXTDATA->init();
		TIMEMANAGER->init();
		EFFECTMANAGER->init();
		SOUNDMANAGER->init();
		SCENEMANAGER->init();
		CAMERAMANAGER->init();
		KEYANIMANAGER->init();
	}

	return S_OK;
}

void gameNode::release()
{
	if (_managerInit)
	{
		
		KEYMANAGER->release();
		KEYMANAGER->releaseSingleton();
		IMAGEMANAGER->release();
		IMAGEMANAGER->releaseSingleton();
		TXTDATA->release();
		TXTDATA->releaseSingleton();
		TIMEMANAGER->release();
		TIMEMANAGER->releaseSingleton();
		EFFECTMANAGER->release();
		EFFECTMANAGER->releaseSingleton();
		SOUNDMANAGER->release();
		SOUNDMANAGER->releaseSingleton();
		SCENEMANAGER->release();
		SCENEMANAGER->releaseSingleton();
		CAMERAMANAGER->release();
		CAMERAMANAGER->releaseSingleton();
		KEYANIMANAGER->release();
		KEYANIMANAGER->releaseSingleton();

	}

	ReleaseDC(_hWnd, _hdc);
}

void gameNode::update()
{
	
}

void gameNode::render()
{
}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC			hdc;

	switch (iMessage)
	{
		case WM_CREATE:

		break;
	
		case WM_MOUSEMOVE:
			_ptMouse.x = static_cast<float>(LOWORD(lParam));
			_ptMouse.y = static_cast<float>(HIWORD(lParam));
		break;

		case WM_KEYDOWN:
		{
			switch (wParam)
			{

				//case VK_ESCAPE:	// ESC를 눌렀을때 프로그램을 종료하게 만든다.
				case WM_CLOSE:		// 그래서 다른키를 이용했다. (우상단 X버튼, 시스템 메뉴닫기, 알트+F4, 작업 관리자 등으로 종료 가능

					PostQuitMessage(0);
				break;

			}
		}
		break;


		case WM_DESTROY:
			PostQuitMessage(0);
		break;

	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
