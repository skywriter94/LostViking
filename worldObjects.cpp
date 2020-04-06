#include "stdafx.h"
#include "worldObjects.h"
#include "playerManager.h"

worldObjects::worldObjects()
{
}

worldObjects::~worldObjects()
{
}

HRESULT worldObjects::init()
{
	//�����������������ᰡ�� ���� ��ġ ��Ʈ��������������������������������������������
	_niddle[0].rc = RectMake(240, 2210, 570, 40);
	_niddle[1].rc = RectMake(1111, 2210, 2385, 40);
	_niddle[2].rc = RectMake(1686, 38, 180, 39);
	for (int i = 0; i < 3; i++)
	{//���� Ÿ��
		_niddle[i].trap = TRAP_NIDDLE;
		_niddle[i].isCollision = false;
	}//�������������డ�� ������ ���Ϳ� ���ڡ�������������������
	for (int i = 0; i < 3; i++) { _vTrap.push_back(_niddle[i]); }
	//�����������������ᵶ ���� ��ġ ��Ʈ��������������������������������������������
	IMAGEMANAGER->addFrameImage("poision", "./image/ImageCollection/poision.bmp", 130, 56, 5, 1, true, RGB(255, 0, 255));
	//��ǥ
	_poision[0].x = 1307;
	_poision[1].x = 1550;
	_poision[2].x = 1985;
	_poision[3].x = 2470;
	_poision[0].y = 1150;
	_poision[1].y = 1150;
	_poision[2].y = 1150;
	_poision[3].y = 1350;
	//��Ʈ
	_poision[0].rc = RectMake(1307, 1150, 26, 34);
	_poision[1].rc = RectMake(1550, 1150, 26, 34);
	_poision[2].rc = RectMake(1985, 1150, 26, 34);
	_poision[3].rc = RectMake(2470, 1350, 26, 34);
	for (int i = 0; i < 4; i++)
	{
		_poision[i].frameX = 0;
		_poision[i].frameY = 0;
		_poision[i].image = new image;
		_poision[i].image = IMAGEMANAGER->findImage("poision");
		_poision[i].isCollision = false;
		//���� Ÿ��
		_poision[i].trap = TRAP_POISION;
		_poision[i].trap = TRAP_POISION;
	}//�������������൶ ������ ���Ϳ� ���ڡ�������������������
	for (int i = 0; i < 4; i++) { _vTrap.push_back(_poision[i]); }
	//���������������������� Ŭ������ �ű��������������������������������������������
	/*IMAGEMANAGER->addFrameImage("Water_Fall", "./image/ImageCollection/water_Fall.bmp", 564, 602, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("long_Water_Fall", "./image/ImageCollection/long_Water_Fall.bmp", 1152, 1467, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("short_Water_Fall", "./image/ImageCollection/short_Water_Fall.bmp", 564, 622, 4, 1, true, RGB(255, 0, 255));
	_waterFall[0].image = IMAGEMANAGER->findImage("Water_Fall");
	_waterFall[1].image = IMAGEMANAGER->findImage("long_Water_Fall");
	_waterFall[2].image = IMAGEMANAGER->findImage("short_Water_Fall");
	for (int i = 0; i < 3; i++)
	{
		_waterFall[i].frameX = 0;
		_waterFall[i].frameY = 0;
	}*/
	//�����������������Ắ �� ���� ��ġ��������������������������������������������
	_isUpDown = false;
	_breakableWallOne.image = IMAGEMANAGER->addFrameImage("Stone_Door_5", "./image/ImageCollection/Stone_Door_5.bmp", 384, 240, 8, 1, true, RGB(255, 0, 255));
	_breakableWallTwo.image = IMAGEMANAGER->addFrameImage("Stone_Door_4", "./image/ImageCollection/Stone_Door_4.bmp", 336, 192, 7, 1, true, RGB(255, 0, 255));
	_unbreakableWallOne.image = IMAGEMANAGER->addFrameImage("BrownDoor_1", "./image/ImageCollection/BrownDoor.bmp", 315, 192, 7, 1, true, RGB(255, 0, 255));
	_unbreakableWallTwo.image = IMAGEMANAGER->addFrameImage("BrownDoor_2", "./image/ImageCollection/BrownDoor.bmp", 315, 192, 7, 1, true, RGB(255, 0, 255));
	_flyingBoard.image = IMAGEMANAGER->addImage("Moving_Flatform", "./image/ImageCollection/Moving_Flatform.bmp", 192, 45, false, RGB(0, 0, 0));
	_flyingBoard.rc = RectMake(1679, 325, 192, 45);
	//���� ��ǥ
	_flyingBoard.x = 1679;
	_flyingBoard.y = 325;
	//����Ÿ��
	_flyingBoard.trap = TRAP_BORAD;
	//�μ����� �� ��ǥ �� �浹
	_breakableWallOne.x = 1324;
	_breakableWallOne.y = 235;
	_breakableWallOne.isCollision = false;
	_breakableWallTwo.x = 3055;
	_breakableWallTwo.y = 85;
	_breakableWallTwo.isCollision = false;
	//�μ����� �� ������ ����;
	_breakableWallOne.frameX = 0;
	_breakableWallOne.frameY = 0;
	_breakableWallTwo.frameX = 0;
	_breakableWallTwo.frameY = 0;
	//�μ����� �� Ÿ��
	_breakableWallOne.trap = TRAP_WALL;
	_breakableWallTwo.trap = TRAP_WALL;
	//�μ����� �� ��Ʈ
	_breakableWallOne.rc = RectMake(_breakableWallOne.x, _breakableWallOne.y, 48, 243);
	_breakableWallTwo.rc = RectMake(_breakableWallTwo.x, _breakableWallTwo.y, 48, 192);
	//�Ⱥμ����� �� ��ǥ �� �浹
	_unbreakableWallOne.x = 2090;
	_unbreakableWallOne.y = 85;
	_unbreakableWallOne.isCollision = false;
	_unbreakableWallTwo.x = 3695;
	_unbreakableWallTwo.y = 2055;
	_unbreakableWallTwo.isCollision = false;
	//��	�μ����� �� ������ ����;
	_unbreakableWallOne.frameX = 0;
	_unbreakableWallOne.frameY = 0;
	_unbreakableWallTwo.frameX = 0;
	_unbreakableWallTwo.frameY = 0;
	//�Ⱥμ����� �� Ÿ��
	_unbreakableWallOne.trap = TRAP_RED_UNBREAKABLE_WALL;
	_unbreakableWallTwo.trap = TRAP_BLUE_UNBREAKABLE_WALL;
	//�Ⱥμ����� �� ��Ʈ
	_unbreakableWallOne.rc = RectMake(_unbreakableWallOne.x, _unbreakableWallOne.y, 45, 195);
	_unbreakableWallTwo.rc = RectMake(_unbreakableWallTwo.x, _unbreakableWallTwo.y, 45, 195);
	//������������������� ���Ϳ� ���ڡ�������������������
	_vTrap.push_back(_flyingBoard);
	_vTrap.push_back(_breakableWallOne);
	_vTrap.push_back(_breakableWallTwo);
	_vTrap.push_back(_unbreakableWallOne);
	_vTrap.push_back(_unbreakableWallTwo);
	//����������������������� ��ġ��������������������������������������������
	_Items[0].image = IMAGEMANAGER->addImage("Boom", "./image/ImageCollection/Boom_Item.bmp", 50, 50, true, RGB(255, 0, 255));
	_Items[1].image = IMAGEMANAGER->addImage("Boom", "./image/ImageCollection/Boom_Item.bmp", 50, 50, true, RGB(255, 0, 255));
	_Items[2].image = IMAGEMANAGER->addImage("oneTomato", "./image/ImageCollection/item_hp2.bmp", 50, 50, true, RGB(255, 0, 255));
	_Items[3].image = IMAGEMANAGER->addImage("twoTomato", "./image/ImageCollection/item_hp2.bmp", 50, 50, true, RGB(255, 0, 255));
	_Items[4].image = IMAGEMANAGER->addImage("BlueKey", "./image/ImageCollection/BlueKey.bmp", 50, 50, true, RGB(255, 0, 255));
	_Items[5].image = IMAGEMANAGER->addImage("BlueLocker", "./image/ImageCollection/BlueKey_Hole.bmp", 48, 48, false, RGB(0, 0, 0));
	_Items[6].image = IMAGEMANAGER->addImage("RedKey", "./image/ImageCollection/RedKey.bmp", 50, 50, true, RGB(255, 0, 255));
	_Items[7].image = IMAGEMANAGER->addImage("RedLocker", "./image/ImageCollection/RedKey_Hole.bmp", 48, 48, false, RGB(0, 0, 0));
	_Items[8].image = IMAGEMANAGER->addFrameImage("BOOM", "./image/ImageCollection/Boom.bmp", 264, 84, 3, 1, true, RGB(255, 0, 255));
	_isBoomEffects.image = IMAGEMANAGER->addImage("IsBoom", "./image/ImageCollection/isBoom.bmp", 960, 758, false, RGB(0, 0, 0));
	//��ź ���̵� ��
	_isBoomEffects.fadeIn = 200;
	//��ź ��ǥ
	_Items[0].x = 750;
	_Items[0].y = 60;
	_Items[1].x = 3600;
	_Items[1].y = 665;
	//�丶�� ��ǥ
	_Items[2].x = 1560;
	_Items[2].y = 155;
	_Items[3].x = 25;
	_Items[3].y = 640;
	//�Ķ� ���� �� �ڹ��� ��ǥ
	_Items[4].x = 2085;		//�Ķ� ����
	_Items[4].y = 995;
	_Items[5].x = 3520;		//�Ķ� �ڹ���
	_Items[5].y = 2105;
	//���� ���� �� �ڹ��� ��ǥ
	_Items[6].x = 1820;		//���� ����
	_Items[6].y = 555;
	_Items[7].x = 2040;		//���� �ڹ���
	_Items[7].y = 185;
	//����
	_Items[0].item = ITEM_BOMB;
	_Items[1].item = ITEM_BOMB;
	_Items[2].item = ITEM_TOMATO;
	_Items[3].item = ITEM_TOMATO;
	_Items[4].item = ITEM_BLUEKEY;
	_Items[5].item = ITEM_BLUELOCKER;
	_Items[6].item = ITEM_REDKEY;
	_Items[7].item = ITEM_REDLOCKER;
	_Items[8].item = ITEM_BOOM;
	//��ź ������ ����Ʈ
	_Items[8].frameX = 0;
	_Items[8].frameY = 0;
	//��������������������� ���Ϳ� ���ڡ�������������������
	for (int i = 0; i < 8; i++)
	{
		_Items[i].rc = RectMake(_Items[i].x, _Items[i].y, _Items->image->getWidth(), _Items->image->getHeight());
		_Items[i].isCollision = false;
		_Items[i].isUse = false;
		_vItem.push_back(_Items[i]);
	}
	//_vTrap.push_back(_Items[5]);
	//_vTrap.push_back(_Items[7]);
	//��������������������Ῡ����������������������������������������������
	_frameCount = _boomCount = 0;
	_isBoomShow = false;
	return S_OK;
}

void worldObjects::update()
{
	_frameCount++;
	move();
	frameWork();

	for (_viTrap = _vTrap.begin(); _viTrap != _vTrap.end(); ++_viTrap)
	{
		if (_viTrap->trap == TRAP_POISION)
		{
			_viTrap->rc = RectMake(_viTrap->x, _viTrap->y + (_viTrap->image->getFrameHeight() / 2),
				_viTrap->image->getFrameWidth(), _viTrap->image->getFrameHeight() / 2);
		}
		else if (_viTrap->trap == TRAP_BORAD)
		{
			_viTrap->rc = RectMake(_viTrap->x, _viTrap->y, 192, 45);
		}
		else if (_viTrap->trap == TRAP_WALL)
		{
			_viTrap->rc = RectMake(_viTrap->x, _viTrap->y,
				_viTrap->image->getFrameWidth(), _viTrap->image->getFrameHeight());
		}
	}
}

void worldObjects::release()
{
}

void worldObjects::render()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{//����������������������� �̹��� ��¡��������������������������������
		if (!_viItem->isCollision)
		{
			_viItem->image->render(CAMERAMANAGER->getWorDC(), _viItem->x, _viItem->y);
			if (KEYMANAGER->isStayKeyDown('Q'))
			{
				Rectangle(CAMERAMANAGER->getWorDC(), _viItem->rc);
			}
		}
	}
	//�����������������ᰡ�� ���� ��Ʈ��¡�������������������������������������������
	for (int i = 0; i < 3; i++)
	{
		if (KEYMANAGER->isStayKeyDown('Q'))
		{
			Rectangle(CAMERAMANAGER->getWorDC(), _niddle[i].rc);

			Rectangle(CAMERAMANAGER->getWorDC(), _EXIT.rc);

		}
	}
	//�����������������Ắ �� ���� �̹��� ��¡�������������������������������������������
	if (KEYMANAGER->isStayKeyDown('Q'))
	{//�����������������Ắ �浹 ��Ʈ ��¡�������������������������������������������
		Rectangle(CAMERAMANAGER->getWorDC(), _breakableWallOne.rc);
		Rectangle(CAMERAMANAGER->getWorDC(), _breakableWallTwo.rc);
		Rectangle(CAMERAMANAGER->getWorDC(), _unbreakableWallOne.rc);
		Rectangle(CAMERAMANAGER->getWorDC(), _unbreakableWallTwo.rc);
	}
	for (_viTrap = _vTrap.begin(); _viTrap != _vTrap.end(); ++_viTrap)
	{
		if (_viTrap->trap == TRAP_POISION)
		{//�����������������ᵶ ���� �̹��� ��¡�������������������������������������������
			if (!_viTrap->isCollision)
			{//�����������������ᵶ ���� ��Ʈ ��¡�������������������������������������������
				if (KEYMANAGER->isStayKeyDown('Q')) { Rectangle(CAMERAMANAGER->getWorDC(), _viTrap->rc); }
				_viTrap->image->frameRender(CAMERAMANAGER->getWorDC(), _viTrap->x, _viTrap->y, _viTrap->frameX, _viTrap->frameY);
			}
		}
		else if (_viTrap->trap == TRAP_BORAD)
		{//��������������������� �̹��� ��¡�������������������������������������������
			_viTrap->image->render(CAMERAMANAGER->getWorDC(), _viTrap->x, _viTrap->y);
			//���� ��Ʈ ���
			if (KEYMANAGER->isStayKeyDown('Q')) { Rectangle(CAMERAMANAGER->getWorDC(), _viTrap->rc); }
		}
		else if (_viTrap->trap == TRAP_WALL)
		{//������������������μ����� �� �̹��� ��¡�������������������������������������������
			_viTrap->image->frameRender(CAMERAMANAGER->getWorDC(), _viTrap->x, _viTrap->y, _viTrap->frameX, _viTrap->frameY);
			//�μ����� �� ��Ʈ ���
			if (KEYMANAGER->isStayKeyDown('Q')) { Rectangle(CAMERAMANAGER->getWorDC(), _viTrap->rc); }
		}
		else if (_viTrap->trap == TRAP_RED_UNBREAKABLE_WALL || _viTrap->trap == TRAP_BLUE_UNBREAKABLE_WALL)
		{//������������������μ����� �� �̹��� ��¡�������������������������������������������
			_viTrap->image->frameRender(CAMERAMANAGER->getWorDC(), _viTrap->x, _viTrap->y, _viTrap->frameX, _viTrap->frameY);
			//�μ����� �� ��Ʈ ���
			if (KEYMANAGER->isStayKeyDown('Q')) { Rectangle(CAMERAMANAGER->getWorDC(), _viTrap->rc); }
		}
	}
	//IMAGEMANAGER->findImage("OBJECT")->render(CAMERAMANAGER->getWorDC(), 0, 0);
	////���������������������� Ŭ������ �ű��������������������������������������������
	//IMAGEMANAGER->findImage("Water_Fall")->frameRender(CAMERAMANAGER->getWorDC(),
	//	1150, 148, _waterFall[0].frameX, _waterFall[0].frameY);
	//IMAGEMANAGER->findImage("long_Water_Fall")->frameRender(CAMERAMANAGER->getWorDC(),
	//	815, 832, _waterFall[1].frameX, _waterFall[1].frameY);
	//IMAGEMANAGER->findImage("short_Water_Fall")->frameRender(CAMERAMANAGER->getWorDC(),
	//	3263, -60, _waterFall[2].frameX, _waterFall[2].frameY);
	if (_isBoomShow)
	{//��������������������ź�� �����Ҷ� �̹��� ��¡�������������������������������������������
		_Items[8].image->frameRender(CAMERAMANAGER->getWorDC(), _Items[8].x, _Items[8].y + 30, _Items[8].frameX, 0);
		if (KEYMANAGER->isStayKeyDown('Q')) { Rectangle(CAMERAMANAGER->getWorDC(), _Items[8].rc); }
	//	if (_boomCount >= 200)
	//	{
	//		_isBoomEffects.fadeIn++;
	//		if (_isBoomEffects.fadeIn >= 207 && !_isChange)
	//		{
	//			_isBoomEffects.fadeIn = 50;
	//			_isBoomEffects.fadeIn++;
	//			_isChange = true;
	//		
	//		}
	//		if (_isBoomEffects.fadeIn >= 57 && _isChange)
	//		{
	//			_isBoomEffects.fadeIn = 200;
	//			_isBoomEffects.fadeIn++;
	//			_isChange = false;
	//		}
	//		_isBoomEffects.image->alphaRender(CAMERAMANAGER->getWorDC(), CAMERAMANAGER->get_Camera_X(), CAMERAMANAGER->get_Camera_Y(), _isBoomEffects.fadeIn);
	//	}
	}
}

void worldObjects::frameWork()
{
	RECT temp;
	for (_viTrap = _vTrap.begin(); _viTrap != _vTrap.end(); ++_viTrap)
	{
		if (_frameCount % 2 == 0)
		{
			if (_viTrap->trap == TRAP_POISION)
			{//�����������������ᵶ�� ���ÿ� �浹���� �ʾҴٸ� �����ӡ�������������������������������������������
				if (!IntersectRect(&temp, &_viTrap->rc, &_niddle[1].rc))
				{
					_viTrap->image->setFrameX(_viTrap->frameX);
					if (_viTrap->frameX >= 0) { _viTrap->frameX = 0; }
				}//�����������������ᵶ�� ���ÿ� �浹�ߴٸ� �����ӡ�������������������������������������������
				else if (IntersectRect(&temp, &_viTrap->rc, &_niddle[1].rc))
				{
					_viTrap->image->setFrameX(_viTrap->frameX);
					_viTrap->frameX++;
				}
				if (_viTrap->frameX > _viTrap->image->getMaxFrameX())
				{
					_viTrap->y = 1150;
					_viTrap->isCollision = false;
				}
			}
		}
		if (_frameCount % 6 == 0)
		{
			if (_viTrap->trap == TRAP_WALL)
			{
				if (!_viTrap->isCollision)
				{//������������������μ����� ���� �÷��̾��� Ư�� Ű�� �浹���� �ʾҴٸ� �����ӡ�������������������������������������������
					_viTrap->image->setFrameX(_viTrap->frameX);
					if (_viTrap->frameX >= 0) { _viTrap->frameX = 0; }
				}
				else if (_viTrap->isCollision)
				{//������������������μ����� ���� �÷��̾��� Ư�� Ű�� �浹�ߴٸ� �����ӡ�������������������������������������������
					_viTrap->image->setFrameX(_viTrap->frameX);
					_viTrap->frameX++;
					if (_viTrap->frameX > _viTrap->image->getMaxFrameX())
					{
						_viTrap->y = -300;
						_viTrap->isCollision = false;
					}
				}
			}
		}
		if (_frameCount % 6 == 0)
		{
			if (_viTrap->trap == TRAP_RED_UNBREAKABLE_WALL ||
				_viTrap->trap == TRAP_BLUE_UNBREAKABLE_WALL)
			{
				if (!_viTrap->isCollision)
				{//������������������Ⱥμ����� ���� ���踦 ������� �ʾҴٸ� �����ӡ�������������������������������������������
					_viTrap->image->setFrameX(_viTrap->frameX);
					if (_viTrap->frameX >= 0) { _viTrap->frameX = 0; }
				}
				else if (_viTrap->isCollision)
				{//������������������Ⱥμ����� ���� ���踦 ����ߴٸ� �����ӡ�������������������������������������������
					_viTrap->image->setFrameX(_viTrap->frameX);
					_viTrap->frameX++;
					if (_viTrap->frameX > _viTrap->image->getMaxFrameX())
					{
						_viTrap->y = -300;
						//_viTrap->isCollision = false;
					}
				}
			}
		}
	}
	//��������������������ź �����ӡ�������������������������������������������
	if (_isBoomShow)
	{
		_boomCount++;
		{
			if (_boomCount <= 200)
			{
				if (_frameCount % 6 == 0)
				{
					_Items[8].image->setFrameX(_Items[8].frameX);
					_Items[8].frameX++;
					if (_Items[8].frameX > 1) { _Items[8].frameX = 0; }
				}
			}
			else if (_boomCount >= 250)
			{
				_isBoomShow = false;
				_boomCount = 0;
			}
			else
			{
				_Items[8].image->setFrameX(_Items[8].frameX);
				_Items[8].frameX++;
				if (_Items[8].frameX > 2)
				{ 
					_Items[8].frameX = 2;
				}
			}
		}

	}
	////���������������������� Ŭ������ �ű��������������������������������������������
	//for (int i = 0; i < 3; i++)
	//{
	//	if (_frameCount % 4 == 0)
	//	{
	//		_waterFall[i].image->setFrameX(_waterFall[i].frameX);
	//		_waterFall[i].frameX++;
	//		if (_waterFall[i].frameX > _waterFall[i].image->getMaxFrameX()) { _waterFall[i].frameX = 0; }
	//	}
	//}
}

void worldObjects::move()
{
	RECT temp;
	for (_viTrap = _vTrap.begin(); _viTrap != _vTrap.end(); ++_viTrap)
	{
		if (_viTrap->trap == TRAP_POISION)
		{//�����������������ᵶ ���� �����ӡ�������������������������������������������
			if (!IntersectRect(&temp, &_viTrap->rc, &_niddle[1].rc))
			{
				_viTrap->y += 5;
			}
		}
		else if (_viTrap->trap == TRAP_BORAD)
		{//��������������������� �����ӡ�������������������������������������������
			if (!_isUpDown)
			{
				_viTrap->y += 3;
				if (_viTrap->y + 45 >= 625)
				{
					_isUpDown = true;
				}
			}
			if (_isUpDown)
			{
				_viTrap->y -= 3;
				if (_viTrap->y <= 85)
				{
					_isUpDown = false;
				}
			}
		}
	}
}

void worldObjects::MakeBoom(float x, float y)
{
	_isBoomShow = true;
	_Items[8].x = x;
	_Items[8].y = y;
	_Items[8].rc = RectMake(x - 100, y - 100, _Items[8].image->getFrameWidth() + 200, _Items[8].image->getFrameHeight() + 200);

}
