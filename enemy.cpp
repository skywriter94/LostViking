#include "stdafx.h"
#include "Enemy.h"
#include"playerManager.h"

HRESULT Enemy::init(EnemyType enemyType, float x, float y)
{
	imageReset();
	
	_enemyState = EnemyState::IDLE;
	_enemyType = enemyType;
	//������ ���� ���� �ʱ�ȭ
	_frameX = _frameY = _frameCount = 0;
	switch (enemyType)
	{
	case EnemyType::MUMMY:
		_image = IMAGEMANAGER->findImage("Enemy_Mummy");
		_Attack_image = IMAGEMANAGER->findImage("Enemy_Mummy_Attack");
		_enemyAttackRangeRect = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	case EnemyType::SCORPION:
		_image = IMAGEMANAGER->findImage("Enemy_Scorpion");
		_Attack_image = IMAGEMANAGER->findImage("Enemy_Scorpion_Attack");
		_enemyAttackRangeRect = RectMakeCenter(_x, _y, 600, 200);
		break;
	case EnemyType::SNAKE:
		_image = IMAGEMANAGER->findImage("Enemy_Snake");
		_Attack_image = IMAGEMANAGER->findImage("Enemy_Snake_Attack");
		_enemyAttackRangeRect = RectMakeCenter(_x, _y, 600, 200);
		break;
	case EnemyType::PLAYERMUMMY:
		_image = IMAGEMANAGER->findImage("Enemy_PlayerMummy");
		_Attack_image = IMAGEMANAGER->findImage("Enemy_PlayerMummy_Attack");
		_enemyAttackRangeRect = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	default:
		break;
	}
	_die_Image = IMAGEMANAGER->findImage("Enemy_Die");

	_enemyHP = 3;

	_x = x;
	_y = y;

	_probeX = _x;
	_probeY = _y;

	_turn_Num = 0;
	_turn = false;
	_RIP = false;
	_enemyRect = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	_enemy_DISCOVERY_Rect = RectMakeCenter(_x, _y, 600, 200);
	_cameraRect = RectMake(CAMERAMANAGER->get_Camera_X(), CAMERAMANAGER->get_Camera_Y(), CAMERAMANAGER->get_CameraSizeX(), CAMERAMANAGER->get_CameraSizeY());
	return S_OK;
}

void Enemy::release()
{

}

void Enemy::update()
{
	EnemyAction();		//���� ������ ���� �ൿ�� �����ϴ� �Լ�
	Frame();			//���� �������� �����ϴ� �Լ�
	die();
	//playerLink();		//�÷��̾��� ��Ʈ�� �޾ƿ��� �Լ�
	switch (_enemyType)
	{
	case EnemyType::MUMMY:
		_enemyAttackRangeRect = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	case EnemyType::SCORPION:
		_enemyAttackRangeRect = RectMakeCenter(_x, _y, 600, 200);
		break;
	case EnemyType::SNAKE:
		_enemyAttackRangeRect = RectMakeCenter(_x, _y, 600, 200);
		break;
	case EnemyType::PLAYERMUMMY:
		_enemyAttackRangeRect = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
		break;
	default:
		break;
	}
	_cameraRect = RectMake(CAMERAMANAGER->get_Camera_X(), CAMERAMANAGER->get_Camera_Y(), CAMERAMANAGER->get_CameraSizeX(), CAMERAMANAGER->get_CameraSizeY());
	_enemy_DISCOVERY_Rect = RectMakeCenter(_x, _y, 600, 200);

	if (_enemyState != EnemyState::ATTACK)_enemyRect = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	else _enemyRect = RectMakeCenter(_x, _y, _Attack_image->getFrameWidth(), _Attack_image->getFrameHeight());
}

void Enemy::render()
{
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		//Rectangle(CAMERAMANAGER->getWorDC(), _ericRect);
		Rectangle(CAMERAMANAGER->getWorDC(), _enemy_DISCOVERY_Rect);
		Rectangle(CAMERAMANAGER->getWorDC(), _enemyRect);
	}
	//Rectangle(CAMERAMANAGER->getWorDC(), _enemyAttackRect);

	//���°� IDLE,SCOUT,DISCOVERY���¸� ��� ����
	if (_enemyState != EnemyState::DIE && _enemyState != EnemyState::ATTACK)
	{
		_image->frameRender(CAMERAMANAGER->getWorDC(), _enemyRect.left, _enemyRect.top, _frameX, _frameY);
	}
	if (_enemyState == EnemyState::ATTACK)
	{
		_Attack_image->frameRender(CAMERAMANAGER->getWorDC(), _enemyRect.left, _enemyRect.top, _frameX, _frameY);
		//_die_Image->frameRender(CAMERAMANAGER->getWorDC(), _enemyRect.left, _enemyRect.top, _frameX, _frameY);
	}
	if (_enemyState == EnemyState::DIE)
	{
		_die_Image->frameRender(CAMERAMANAGER->getWorDC(), _enemyRect.left, _enemyRect.top + 30, _frameX, _frameY);
	}
}

void Enemy::EnemyAction()
{
}

void Enemy::Frame()
{

}

void Enemy::Move()
{
	if (_turn_Num % 2 == 1)
	{
		_enemyLR = EnemyLR::LEFT;
		if (_frameX <= 3 && !_turn)
			_x -= 3;
	}
	else
	{
		_enemyLR = EnemyLR::RIGHT;
		if (_frameX <= 3&&!_turn)
			_x += 3;
	}
}

void Enemy::Scout()
{
	switch (_enemyLR)
	{
	case EnemyLR::LEFT:
		for (int i = _x - _image->getFrameWidth() / 2; i > _x - _image->getFrameWidth() / 2 - 30;--i)
		{
			//�ٴڿ� ������ ���ؿ´�
			COLORREF platformCOLOR = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), i, _probeY + 60);
			//���� ������ ���ؿ´�
			COLORREF wallCOLOR = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), i, _y - _image->getFrameHeight());

			int wallR = GetRValue(wallCOLOR);
			int wallG = GetGValue(wallCOLOR);
			int wallB = GetBValue(wallCOLOR);

			int platformR = GetRValue(platformCOLOR);
			int platformG = GetGValue(platformCOLOR);
			int platformB = GetBValue(platformCOLOR);
			if (_wall)
			{
				_turn_Num++;
				_turn = true;
			}
			else
			{
				_turn = false;
			}
			if (((platformR == 255 && platformG == 0 && platformB == 255) || (platformR == 255 && platformG == 255 && platformB == 255) || (platformR == 255 && platformG == 0 && platformB == 0) || (wallR == 255 && wallG == 255 && wallB == 0)))
			{
				_turn_Num++;
				_turn = true;
				break;
			}
			else
			{
				_turn = false;
				break;
			}
		}
		break;
	case EnemyLR::RIGHT:
		for (int i = _x + _image->getFrameWidth() / 2; i < _x + _image->getFrameWidth() / 2 + 70;++i)
		{
			//�ٴڿ� ������ ���ؿ´�
			COLORREF platformCOLOR = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), i, _probeY + 70);
			//���� ������ ���ؿ´�
			COLORREF wallCOLOR = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), i, _y - _image->getFrameHeight());

			int wallR = GetRValue(wallCOLOR);
			int wallG = GetGValue(wallCOLOR);
			int wallB = GetBValue(wallCOLOR);

			int platformR = GetRValue(platformCOLOR);
			int platformG = GetGValue(platformCOLOR);
			int platformB = GetBValue(platformCOLOR);

			if (((platformR == 255 && platformG == 0 && platformB == 255) || (platformR == 255 && platformG == 255 && platformB == 255) || (platformR == 255 && platformG == 0 && platformB == 0) || (wallR == 255 && wallG == 255 && wallB == 0)))
			{
				_turn_Num++;
				_turn = true;
				break;
			}
			else
			{
				_turn = false;
				break;
			}
		}
		break;
	default:
		break;
	}
}

void Enemy::Discovery()
{
	RECT temp;
	switch (_enemyType)
	{
	case EnemyType::MUMMY:
		if (IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_ericRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::ERIC;
			_enemyState = EnemyState::DISCOVERY;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
		}
		if (IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_olafRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::OLAF;
			_enemyState = EnemyState::DISCOVERY;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
		}

		if (IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_baleogRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::BALEOG;
			_enemyState = EnemyState::DISCOVERY;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
		}
		break;
	case EnemyType::SCORPION:
		if (IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_ericRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::ERIC;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
		}
		if (IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_olafRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::OLAF;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
		}

		if (IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_baleogRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::BALEOG;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
		}
		break;
	case EnemyType::SNAKE:
		if (IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_ericRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::ERIC;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
		}
		if (IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_olafRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::OLAF;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
		}

		if (IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_baleogRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::BALEOG;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
		}
		break;
	case EnemyType::PLAYERMUMMY:
		if (IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_ericRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::ERIC;
			_enemyState = EnemyState::DISCOVERY;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
		}
		if (IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_olafRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::OLAF;
			_enemyState = EnemyState::DISCOVERY;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
		}

		if (IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_baleogRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::BALEOG;
			_enemyState = EnemyState::DISCOVERY;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
		}
		break;
	default:
		break;
	}
}
void Enemy::UnDiscovery()
{
	RECT temp;
	if (!IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_ericRect))_enemyState = EnemyState::SCOUT;		//�÷��̾�(����)�� Ž������ ������ ������  SCOUT���·� ����
	if (!IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_baleogRect))_enemyState = EnemyState::SCOUT;	//�÷��̾�(���α�)�� Ž������ ������ ������  SCOUT���·� ����
	if (!IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_olafRect))_enemyState = EnemyState::SCOUT;		//�÷��̾�(�ö���)�� Ž������ ������ ������  SCOUT���·� ����
}
void Enemy::Tracking()
{
	RECT temp;
	switch (_discoveryPlayer)
	{
	case DISCOVERYPlayer::ERIC:
		//�÷��̾ �����ʿ� ������(����)
		if (_x < (_ericRect.left + _ericRect.right) / 2 && !_turn && !_wall)
		{
			_turn = true;
			_enemyLR = EnemyLR::RIGHT;
			if (_frameX <= 3)
				_x += 3;
		}
		//�÷��̾ ���ʿ� ������(����)
		if (_x > (_ericRect.left + _ericRect.right) / 2 && !_turn && !_wall)
		{
			_turn = true;
			_enemyLR = EnemyLR::LEFT;
			if (_frameX <= 3)
				_x -= 3;
		}
		if (!IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_ericRect))_enemyState = EnemyState::SCOUT;		//�÷��̾�(����)�� Ž������ ������ ������  SCOUT���·� ����
		break;
	case DISCOVERYPlayer::BALEOG:
		//�÷��̾ �����ʿ� ������(���α�)
		if (_x < (_baleogRect.left + _baleogRect.right) / 2 && !_turn && !_wall)
		{
			_turn = true;
			_enemyLR = EnemyLR::RIGHT;
			if (_frameX <= 3)
				_x += 3;
		}
		//�÷��̾ ���ʿ� ������(���α�)
		if (_x > (_baleogRect.left + _baleogRect.right) / 2 && !_turn && !_wall)
		{
			_turn = true;
			_enemyLR = EnemyLR::LEFT;
			if (_frameX <= 3)
				_x -= 3;
		}
		break;
		if (!IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_baleogRect))_enemyState = EnemyState::SCOUT;	//�÷��̾�(���α�)�� Ž������ ������ ������  SCOUT���·� ����
	case DISCOVERYPlayer::OLAF:
		//�÷��̾ �����ʿ� ������(�ö���)
		if (_x < (_olafRect.left + _olafRect.right) / 2 && !_turn && !_wall)
		{
			_turn = true;
			_enemyLR = EnemyLR::RIGHT;
			if (_frameX <= 3)
				_x += 3;
		}
		//�÷��̾ ���ʿ� ������(�ö���)
		if (_x > (_olafRect.left + _olafRect.right) / 2 && !_turn && !_wall)
		{
			_turn = true;
			_enemyLR = EnemyLR::LEFT;
			if (_frameX <= 3)
				_x -= 3;
		}
		if (!IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_olafRect))_enemyState = EnemyState::SCOUT;		//�÷��̾�(�ö���)�� Ž������ ������ ������  SCOUT���·� ����
		break;
	default:
		break;
	}



	Scout();
	//Move();
}
void Enemy::Attack(EnemyType enemyType)
{
	RECT temp;
	switch (enemyType)
	{
	case EnemyType::MUMMY:
		if (IntersectRect(&temp, &_enemyAttackRangeRect, &_ericRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::ERIC;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
			_frameX = 0;
		}
		if (IntersectRect(&temp, &_enemyAttackRangeRect, &_olafRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::OLAF;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
			_frameX = 0;
		}
		if (IntersectRect(&temp, &_enemyAttackRangeRect, &_baleogRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::BALEOG;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
			_frameX = 0;
		}
		break;
	case EnemyType::SCORPION:
		if (IntersectRect(&temp, &_enemyAttackRangeRect, &_ericRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::ERIC;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
			_frameX = 0;
		}
		if (IntersectRect(&temp, &_enemyAttackRangeRect, &_olafRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::OLAF;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
			_frameX = 0;
		}
		if (IntersectRect(&temp, &_enemyAttackRangeRect, &_baleogRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::BALEOG;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
			_frameX = 0;
		}
		break;
	case EnemyType::SNAKE:
		if (IntersectRect(&temp, &_enemyAttackRangeRect, &_ericRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::ERIC;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
			_frameX = 0;
		}
		if (IntersectRect(&temp, &_enemyAttackRangeRect, &_olafRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::OLAF;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
			_frameX = 0;
		}
		if (IntersectRect(&temp, &_enemyAttackRangeRect, &_baleogRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::BALEOG;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
			_frameX = 0;
		}
		break;
	case EnemyType::PLAYERMUMMY:
		if (IntersectRect(&temp, &_enemyAttackRangeRect, &_ericRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::ERIC;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
			_frameX = 0;
		}
		if (IntersectRect(&temp, &_enemyAttackRangeRect, &_olafRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::OLAF;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
			_frameX = 0;
		}
		if (IntersectRect(&temp, &_enemyAttackRangeRect, &_baleogRect))
		{
			_discoveryPlayer = DISCOVERYPlayer::BALEOG;
			_enemyState = EnemyState::ATTACK;	//�÷��̾ �߰��ϸ� DISCOVERY���·� ����
			_frameX = 0;
		}
		break;
	default:
		break;
	}
}

void Enemy::Attack()
{
	RECT temp;
	if (IntersectRect(&temp, &_enemyAttackRangeRect, &_ericRect))
	{
		_frameX = 0;
		_discoveryPlayer = DISCOVERYPlayer::ERIC;
		_enemyState = EnemyState::ATTACK;	//�÷��̾ ���ݹ��� �ȿ� ������ ATTACK���·� ����
	}
	if (IntersectRect(&temp, &_enemyAttackRangeRect, &_olafRect))
	{
		_frameX = 0;
		_discoveryPlayer = DISCOVERYPlayer::OLAF;
		_enemyState = EnemyState::ATTACK;	//�÷��̾ ���ݹ��� �ȿ� ������ ATTACK���·� ����
	}
	if (IntersectRect(&temp, &_enemyAttackRangeRect, &_baleogRect))
	{
		_frameX = 0;
		_discoveryPlayer = DISCOVERYPlayer::BALEOG;
		_enemyState = EnemyState::ATTACK;	//�÷��̾ ���ݹ��� �ȿ� ������ ATTACK���·� ����

	}
}

void Enemy::AttackDirection()
{
	RECT temp;
	switch (_discoveryPlayer)
	{
	case DISCOVERYPlayer::ERIC:
		//�÷��̾ �����ʿ� ������(����)
		if (_x < (_ericRect.left + _ericRect.right) / 2 && !_turn)
		{
			if (_frameX == 0)_enemyLR = EnemyLR::RIGHT;
		}
		//�÷��̾ ���ʿ� ������(����)
		if (_x > (_ericRect.left + _ericRect.right) / 2 && !_turn)
		{
			if (_frameX == 0)_enemyLR = EnemyLR::LEFT;
		}
		if (!IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_ericRect))_enemyState = EnemyState::SCOUT;		//�÷��̾�(����)�� Ž������ ������ ������  SCOUT���·� ����
		break;
	case DISCOVERYPlayer::BALEOG:
		//�÷��̾ �����ʿ� ������(���α�)
		if (_x < (_baleogRect.left + _baleogRect.right) / 2 && !_turn)
		{
			if (_frameX == 0)_enemyLR = EnemyLR::RIGHT;
		}
		//�÷��̾ ���ʿ� ������(���α�)
		if (_x > (_baleogRect.left + _baleogRect.right) / 2 && !_turn)
		{
			if (_frameX == 0)_enemyLR = EnemyLR::LEFT;
		}
		break;
		if (!IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_baleogRect))_enemyState = EnemyState::SCOUT;	//�÷��̾�(���α�)�� Ž������ ������ ������  SCOUT���·� ����
	case DISCOVERYPlayer::OLAF:
		//�÷��̾ �����ʿ� ������(�ö���)
		if (_x < (_olafRect.left + _olafRect.right) / 2 && !_turn)
		{
			if (_frameX==0)_enemyLR = EnemyLR::RIGHT;
		}
		//�÷��̾ ���ʿ� ������(�ö���)
		if (_x > (_olafRect.left + _olafRect.right) / 2 && !_turn)
		{
			if (_frameX == 0)_enemyLR = EnemyLR::LEFT;
		}
		if (!IntersectRect(&temp, &_enemy_DISCOVERY_Rect, &_olafRect))_enemyState = EnemyState::SCOUT;		//�÷��̾�(�ö���)�� Ž������ ������ ������  SCOUT���·� ����
		break;
	default:
		break;
	}
}

void Enemy::UnAttack()
{
	RECT temp;
	switch (_discoveryPlayer)
	{
	case DISCOVERYPlayer::ERIC:
		if (!IntersectRect(&temp, &_enemyAttackRangeRect, &_ericRect))
		{
			switch (_enemyType)
			{
			case EnemyType::MUMMY:
				if (_frameX ==0)
					_enemyState = EnemyState::SCOUT;	//�÷��̾ ���ݹ��� ������ ������ SCOUT���·� ����
					//_frameX = 0;
				break;
			case EnemyType::SCORPION:
				if (_frameX ==0)
					_enemyState = EnemyState::SCOUT;	//�÷��̾ ���ݹ��� ������ ������ SCOUT���·� ����
					_frameX = 0;
				break;
			case EnemyType::SNAKE:
				if (_frameX ==0)
					_enemyState = EnemyState::SCOUT;	//�÷��̾ ���ݹ��� ������ ������ SCOUT���·� ����
					//_frameX = 0;
				break;
			case EnemyType::PLAYERMUMMY:
				if (_frameX ==0)
					_enemyState = EnemyState::SCOUT;	//�÷��̾ ���ݹ��� ������ ������ SCOUT���·� ����
					//_frameX = 0;
				break;
			default:
				break;
			}
		}
		break;
	case DISCOVERYPlayer::BALEOG:
		if (!IntersectRect(&temp, &_enemyAttackRangeRect, &_olafRect))
		{
			switch (_enemyType)
			{
			case EnemyType::MUMMY:
				if (_frameX >= 5)
					_enemyState = EnemyState::SCOUT;	//�÷��̾ ���ݹ��� ������ ������ SCOUT���·� ����
					//_frameX = 0;
				break;
			case EnemyType::SCORPION:
				if (_frameX >= 6)
					_enemyState = EnemyState::SCOUT;	//�÷��̾ ���ݹ��� ������ ������ SCOUT���·� ����
				_frameX = 0;
				break;
			case EnemyType::SNAKE:
				if (_frameX >= 5)
					_enemyState = EnemyState::SCOUT;	//�÷��̾ ���ݹ��� ������ ������ SCOUT���·� ����
					//_frameX = 0;
				break;
			case EnemyType::PLAYERMUMMY:
				if (_frameX >= 5)
					_enemyState = EnemyState::SCOUT;	//�÷��̾ ���ݹ��� ������ ������ SCOUT���·� ����
					//_frameX = 0;
				break;
			default:
				break;
			}
		}
		break;
	case DISCOVERYPlayer::OLAF:
		if (!IntersectRect(&temp, &_enemyAttackRangeRect, &_baleogRect))
		{
			switch (_enemyType)
			{
			case EnemyType::MUMMY:
				if (_frameX >= 5)
					_enemyState = EnemyState::SCOUT;	//�÷��̾ ���ݹ��� ������ ������ SCOUT���·� ����
					//_frameX = 0;
				break;
			case EnemyType::SCORPION:
				if (_frameX >= 6)
					_enemyState = EnemyState::SCOUT;	//�÷��̾ ���ݹ��� ������ ������ SCOUT���·� ����
				_frameX = 0;
				break;
			case EnemyType::SNAKE:
				if (_frameX >= 5)
					_enemyState = EnemyState::SCOUT;	//�÷��̾ ���ݹ��� ������ ������ SCOUT���·� ����
					//_frameX = 0;
				break;
			case EnemyType::PLAYERMUMMY:
				if (_frameX >= 5)
					_enemyState = EnemyState::SCOUT;	//�÷��̾ ���ݹ��� ������ ������ SCOUT���·� ����
					//_frameX = 0;
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

void Enemy::die()
{
	if (_enemyHP <= 0 && !_RIP)
	{
		_RIP = true;
		_frameX = 0;
		_enemyState = EnemyState::DIE;
	}
}


void Enemy::setPlayerRect(RECT eric, RECT baleog, RECT olaf)
{
	_ericRect = eric;
	_baleogRect = baleog;
	_olafRect = olaf;
}

void Enemy::platformColision()
{
	//���� �ٴڿ� �ٿ��ֱ� ����
	for (int i = _probeY-10; i < _probeY + 100; ++i)
	{
		COLORREF getPixel_Bottom = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), _x, i);

		int r = GetRValue(getPixel_Bottom);
		int g = GetGValue(getPixel_Bottom);
		int b = GetBValue(getPixel_Bottom);

		if (r == 255 && g == 255 && b == 0)
		{
			_y = i - _image->getFrameHeight() / 2;
			break;
		}
	}

}