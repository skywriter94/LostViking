#include "stdafx.h"
#include "image.h"

#pragma comment(lib, "msimg32.lib")

image::image()
	: _imageInfo(NULL),
	_fileName(NULL),
	_trans(false),
	_transColor(RGB(0,0,0))
{
}


image::~image()
{
}

HRESULT image::init(int width, int height)
{
	//����۰� �����ϸ� �޸� ����
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);	// �� DC���� �ϳ��� �����
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//���� ������ �����ؼ� ����۰� �������� �ʾҴٸ�
	if (_imageInfo == NULL)
	{
		//�޸� ����
		release();

		//�׸��� �����ߴ� �޼����� ȣ���ض�
		return E_FAIL;
	}

	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;


	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::init(const DWORD resID, int width, int height, BOOL trans, COLORREF transColor)
{
	//ȣ������ �̹��� ������ �ʱ�ȭ �Ǿ����� �ʴٸ� ������ ���Ѷ�
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_RESOURCE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);	//�� DC������ �����Ѵ�
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	
	_trans = FALSE;
	_transColor = RGB(0,0,0);

	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	if (_imageInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::init(const char * fileName, int width, int height, BOOL trans, COLORREF transColor)
{
	//ã�����ϴ� �̹��� ���� �̸��� ������ ���и� ����ض�
	if (fileName == NULL) return E_FAIL;

	//ȣ������ �̹��� ������ �ʱ�ȭ �Ǿ����� �ʴٸ� ������ ���Ѷ�
	if (_imageInfo != NULL) release();
	
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);	//�� DC������ �����Ѵ�
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//�����̸��� ���̸� �˾ƿ´�
	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	if (_imageInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::init(const char * fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL trans, COLORREF transColor)
{
	//ã�����ϴ� �̹��� ���� �̸��� ������ ���и� ����ض�
	if (fileName == NULL) return E_FAIL;

	//ȣ������ �̹��� ������ �ʱ�ȭ �Ǿ����� �ʴٸ� ������ ���Ѷ�
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);	//�� DC������ �����Ѵ�
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x - (width / 2);
	_imageInfo->y = y - (height / 2);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	//�����̸��� ���̸� �˾ƿ´�
	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	if (_imageInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::init(const char * fileName, int width, int height, int frameX, int frameY, BOOL trans, COLORREF transColor)
{
	//ã�����ϴ� �̹��� ���� �̸��� ������ ���и� ����ض�
	if (fileName == NULL) return E_FAIL;

	//ȣ������ �̹��� ������ �ʱ�ȭ �Ǿ����� �ʴٸ� ������ ���Ѷ�
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);	//�� DC������ �����Ѵ�
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	//�����̸��� ���̸� �˾ƿ´�
	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	if (_imageInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::init_OB(int width, int height)
{
	//ȣ������ �̹��� ������ �ʱ�ȭ �Ǿ����� �ʴٸ� ������ ���Ѷ�
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_RESOURCE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);	//�� DC������ �����Ѵ�
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;


	_trans = FALSE;
	_transColor = RGB(0, 0, 0);

	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	if (_imageInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

void image::release()
{
	if (_imageInfo)
	{
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		SelectObject(_blendImage->hMemDC, _blendImage->hOBit);
		DeleteObject(_blendImage->hBit);
		DeleteDC(_blendImage->hMemDC);

		SAFE_DELETE(_imageInfo);
		SAFE_DELETE(_fileName);
		SAFE_DELETE(_blendImage);

		_trans = NULL;
		_transColor = RGB(0, 0, 0);
	}

}

void image::setTransColor(BOOL trans, COLORREF transColor)
{
	trans = trans;
	transColor = transColor;
}

void image::render(HDC hdc)
{
	//Ʈ���� �÷� ó���� �ؾ��ϳ�
	if (_trans)
	{
		//ȭ�鿡 �ѷ��ٶ� Ư�� �ȼ����� ���� ������ش�
		GdiTransparentBlt(
			hdc,					//����� ������ DC 
			_imageInfo->x,			//����� ��ǥ X
			_imageInfo->y,			//����� ��ǥ Y
			_imageInfo->width,		//����� ũ��
			_imageInfo->height,		//����� ũ��
			_imageInfo->hMemDC,		//������ DC
			0, 0,					//�����ؿ� ��ǥ
			_imageInfo->width,		//�����ؿ� ũ��
			_imageInfo->height,
			_transColor);			//�����ؿö� ������ Į��

	}
	//�ƴϳ�
	else
	{
		BitBlt(hdc, _imageInfo->x, _imageInfo->y, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void image::render(HDC hdc, int destX, int destY)
{
	//Ʈ���� �÷� ó���� �ؾ��ϳ�
	if (_trans)
	{
		//ȭ�鿡 �ѷ��ٶ� Ư�� �ȼ����� ���� ������ش�
		GdiTransparentBlt(
			hdc,					//����� ������ DC 
			destX,					//����� ��ǥ X
			destY,					//����� ��ǥ Y
			_imageInfo->width,		//����� ũ��
			_imageInfo->height,		//����� ũ��
			_imageInfo->hMemDC,		//������ DC
			0, 0,					//�����ؿ� ��ǥ
			_imageInfo->width,		//�����ؿ� ũ��
			_imageInfo->height,
			_transColor);			//�����ؿö� ������ Į��
		
	}
	//�ƴϳ�
	else
	{
		BitBlt(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	//Ʈ���� �÷� ó���� �ؾ��ϳ�
	if (_trans)
	{
		//ȭ�鿡 �ѷ��ٶ� Ư�� �ȼ����� ���� ������ش�
		GdiTransparentBlt(
			hdc,					//����� ������ DC 
			destX,					//����� ��ǥ X
			destY,					//����� ��ǥ Y
			sourWidth,				//����� ũ��
			sourHeight,				//����� ũ��
			_imageInfo->hMemDC,		//������ DC
			sourX, sourY,			//�����ؿ� ��ǥ
			sourWidth,				//�����ؿ� ũ��
			sourHeight,
			_transColor);			//�����ؿö� ������ Į��

	}
	//�ƴϳ�
	else
	{
		BitBlt(hdc, destX, destY, sourWidth, sourHeight,
			_imageInfo->hMemDC, sourX, sourY, SRCCOPY);
	}

}

void image::frameRender(HDC hdc, int destX, int destY)
{
	//Ʈ���� �÷� ó���� �ؾ��ϳ�
	if (_trans)
	{
		//ȭ�鿡 �ѷ��ٶ� Ư�� �ȼ����� ���� ������ش�
		GdiTransparentBlt(
			hdc,												//����� ������ DC 
			destX,												//����� ��ǥ X
			destY,												//����� ��ǥ Y
			_imageInfo->frameWidth,								//����� ũ��
			_imageInfo->frameHeight,							//����� ũ��
			_imageInfo->hMemDC,									//������ DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,									//�����ؿ� ��ǥ
			_imageInfo->frameWidth,									//�����ؿ� ũ��
			_imageInfo->frameHeight,
			_transColor);										//�����ؿö� ������ Į��

	}
	//�ƴϳ�
	else
	{
		BitBlt(hdc, destX, destY, 
			_imageInfo->frameWidth, 
			_imageInfo->frameHeight,
			_imageInfo->hMemDC, 
			_imageInfo->currentFrameX * _imageInfo->frameWidth, 
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}

void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	//Ʈ���� �÷� ó���� �ؾ��ϳ�
	if (_trans)
	{
		//ȭ�鿡 �ѷ��ٶ� Ư�� �ȼ����� ���� ������ش�
		GdiTransparentBlt(
			hdc,												//����� ������ DC 
			destX,												//����� ��ǥ X
			destY,												//����� ��ǥ Y
			_imageInfo->frameWidth,								//����� ũ��
			_imageInfo->frameHeight,							//����� ũ��
			_imageInfo->hMemDC,									//������ DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,									//�����ؿ� ��ǥ
			_imageInfo->frameWidth,									//�����ؿ� ũ��
			_imageInfo->frameHeight,
			_transColor);										//�����ؿö� ������ Į��

	}
	//�ƴϳ�
	else
	{
		BitBlt(hdc, destX, destY,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}

void image::loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY)
{
	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

	int sourWidth;
	int sourHeight;

	RECT rcDest;
	RECT rcSour;

	int drawAreaX = drawArea->left;					//�׷��� ������ left
	int drawAreaY = drawArea->top;					//�׷��� ������ top
	int drawAreaW = drawArea->right - drawAreaX;	//�׷��� ������ ����ũ��
	int drawAreaH = drawArea->bottom - drawAreaY;	//�׷��� ������ ����ũ��

	//Y�� �����Ҷ� ó��
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		//ȭ�鿡�� ���� ������ŭ�� ��´�
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;

		//�߶󳻼� �ٽ� ȭ�� ������ ���� ũ�⸦ ��´�
		sourHeight = rcSour.bottom - rcSour.top;

		//ȭ������� ���� ������ �ٽ� ȭ������� �ִ� �۾��� �ݺ��Ѵ�.
		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//�ѷ��� �������� �ѷ��ش�.
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//Y��ó�� X�൵ ����.
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			rcSour.left = (x + offSetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;

			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawAreaW)
			{
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			render(hdc, rcDest.left, rcDest.top,
				rcSour.left, rcSour.top,
				rcSour.right - rcSour.left,
				rcSour.bottom - rcSour.top);
		}
	}
}

void image::alphaRender(HDC hdc, BYTE alpha)
{
	//���ĺ����尡 ����Ǵ� ������ �����ϸ鼭 ���ô�
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, 0, 0, SRCCOPY);

		GdiTransparentBlt(
			_blendImage->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_transColor);

		AlphaBlend(hdc, _imageInfo->x, _imageInfo->y, _imageInfo->width,
			_imageInfo->height, _blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _blendFunc);

	}
	else
	{
		AlphaBlend(hdc, _imageInfo->x, _imageInfo->y, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	//���ĺ����尡 ����Ǵ� ������ �����ϸ鼭 ���ô�
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(
			_blendImage->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_transColor);

		AlphaBlend(hdc, destX, destY, _imageInfo->width,
			_imageInfo->height, _blendImage->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _blendFunc);

	}
	else
	{
		AlphaBlend(hdc,destX, destY, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _blendFunc);
	}
}

void image::scaleRender(HDC hdc, int destX, int destY, int scaleX, int scaleY)
{
	//���̹� ī�� ����� �Խ��ǿ���
	//Blt�� �˻��ؼ� �ߴ� �� �� �о�� �ٶ�
	//�׷����� �̰� �̿��ؼ� �����ӷ����� ���ǻ� �ʿ��� ��������� ���� ����ϱ� �ٶ�

	SetStretchBltMode(getMemDC(), COLORONCOLOR);

	//ȭ�鿡 �ѷ��ٶ� Ư�� �ȼ����� ���� ������ش�
	TransparentBlt(
		hdc,					//����� ������ DC 
		destX,					//����� ��ǥ X
		destY,					//����� ��ǥ Y
		_imageInfo->width * scaleX,		//����� ũ��
		_imageInfo->height * scaleY,		//����� ũ��
		_imageInfo->hMemDC,		//������ DC
		0, 0,					//�����ؿ� ��ǥ
		_imageInfo->width,		//�����ؿ� ũ��
		_imageInfo->height,
		_transColor);			//�����ؿö� ������ Į��
	
}

void image::aniRender(HDC hdc, int destX, int destY, animation * ani)
{
	render(hdc, destX, destY, ani->getFramePos().x, ani->getFramePos().y, ani->getFrameWidth(), ani->getFrameHeight());
}

void image::aniRender(HDC hdc, int destX, int destY, animation* ani, bool last)
{
	if (last)// ��� �����ϸ� 
	{
		if (ani->getFramePos().x >= ani->getMaxFrame())
		{
			render(hdc, destX, destY, ani->getMaxFrame(), ani->getFramePos().y, ani->getFrameWidth(), ani->getFrameHeight());
		}
		else
		{
			render(hdc, destX, destY, ani->getFramePos().x, ani->getFramePos().y, ani->getFrameWidth(), ani->getFrameHeight());
		}
	}
	else render(hdc, destX, destY, ani->getFramePos().x, ani->getFramePos().y, ani->getFrameWidth(), ani->getFrameHeight());

}