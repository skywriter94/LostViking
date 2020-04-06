#include "stdafx.h"
#include "Enemy_Bullet.h"


HRESULT Enemy_Bullet::init()
{
	_bulletMax = 10;
	for (int i = 0;i < _bulletMax;i++)
	{
		tagBullet Bullet;
		ZeroMemory(&Bullet, sizeof(Bullet));
		Bullet.image = new image;
		Bullet.image = IMAGEMANAGER->addImage("Enemy_Bullet", "./image./Enemy/Enemy_Bullet.bmp", 12, 12, true, RGB(255, 0, 255));
		Bullet.isFire = false;

		_vBullet.push_back(Bullet);
	}
	return S_OK;
}

void Enemy_Bullet::release()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		SAFE_RELEASE(_viBullet->image);
		SAFE_DELETE(_viBullet->image);
	}
	_vBullet.clear();
}

void Enemy_Bullet::update()
{
	bulletMove();
}

void Enemy_Bullet::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (!_viBullet->isFire)continue;
		if (KEYMANAGER->isToggleKey(VK_F1))
		{
			Rectangle(getMemDC(), _viBullet->rect);
		}
		_viBullet->image->render(CAMERAMANAGER->getWorDC(), _viBullet->rect.left, _viBullet->rect.top);
	}
}

void Enemy_Bullet::bulletFire(float x, float y, float angle)
{
	for (_viBullet = _vBullet.begin();_viBullet != _vBullet.end();++_viBullet)
	{
		if (_viBullet->isFire)continue;

		_viBullet->isFire = true;
		_viBullet->angle = angle;
		_viBullet->x =x;
		_viBullet->y =y;
		_viBullet->probeX = _viBullet->x;
		_viBullet->probeY = _viBullet->y;
		_viBullet->rect = RectMakeCenter(_viBullet->x, _viBullet->y,_viBullet->image->getWidth(),_viBullet->image->getHeight());

		break;
	}
}

void Enemy_Bullet::bulletMove()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (!_viBullet->isFire)continue;

		_viBullet->x += cosf(_viBullet->angle) * 5;
		_viBullet->y -= sinf(_viBullet->angle) * 5;
		_viBullet->probeX = _viBullet->x;
		_viBullet->probeY = _viBullet->y;
		_viBullet->rect = RectMakeCenter(_viBullet->x, _viBullet->y, _viBullet->image->getWidth(), _viBullet->image->getHeight());

		probe();
	}
}


void Enemy_Bullet::probe()
{
	for (int i = _viBullet->probeX - _viBullet->image->getWidth();i < _viBullet->probeX + _viBullet->image->getWidth();++i)
	{
		
			COLORREF COLOR = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), i, _viBullet->probeY);

			int r = GetRValue(COLOR);
			int g = GetGValue(COLOR);
			int b = GetBValue(COLOR);

			if ((r == 255 && g == 255 && b == 0) || (r == 255 && g == 0 && b == 0)||(r==255&&g==255&&b==255)||(r==0&g==0&&b==0))
			{
				_viBullet->isFire = false;
				break;
			}
		
	}
	for (int j = _viBullet->probeY - _viBullet->image->getHeight();j < _viBullet->probeY + _viBullet->image->getWidth();++j)
	{
		COLORREF COLOR = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), _viBullet->probeX, j);

		int r = GetRValue(COLOR);
		int g = GetGValue(COLOR);
		int b = GetBValue(COLOR);

		if ((r == 255 && g == 255 && b == 0) || (r == 255 && g == 0 && b == 0) || (r == 255 && g == 255 && b == 255) || (r == 0 & g == 0 && b == 0))
		{
			_viBullet->isFire = false;
			break;
		}
	}

}

void Enemy_Bullet::removeBullet(int Num)
{
	_viBullet = _vBullet.begin() + Num;
	_viBullet->isFire = false;
}