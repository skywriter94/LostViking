#include "stdafx.h"
#include "Enemy_Scorpion.h"

void Enemy_Scorpion::EnemyAction()
{
	_probeY = _y + _image->getFrameHeight() / 2;
	switch (_enemyState)
	{
	case EnemyState::IDLE:
		RECT temp;
		//ī�޶� ������ ���� SCOUT���·� ����
		if (IntersectRect(&temp, &_enemyRect, &_cameraRect)) _enemyState = EnemyState::SCOUT;
		break;
	case EnemyState::SCOUT:

		Scout();				//�����̴� ����/�� �� ������ �ݴ������� ���ư����� �ϴ� �Լ�
		Move();					//�¿�� �����̰� �ϴ� �Լ�

		Attack();				//���� �߰��ϴ� �Լ�
		if (!IntersectRect(&temp, &_enemyRect, &_cameraRect)) _enemyState = EnemyState::IDLE;				//ī�޶� ������ ������ IDLE���·� ����
		break;
	case EnemyState::ATTACK:
		UnAttack();				//���ݹ��� ������ ������ SCOUT
		AttackDirection();		//���� ���� �÷��̾����� ����
		if (_frameX ==6)
		{
			_isFire = true;
			_enemyState = EnemyState::SCOUT;
		}
		else
		{
			_isFire = false;
		}
		if (!IntersectRect(&temp, &_enemyRect, &_cameraRect)) _enemyState = EnemyState::IDLE;				//ī�޶� ������ ������ IDLE���·� ����
		break;
	case EnemyState::DIE:

		_die = true;
		break;
	default:
		break;
	}

	switch (_enemyLR)
	{
	case EnemyLR::LEFT:
		_frameY = 1;
		break;
	case EnemyLR::RIGHT:
		_frameY = 0;
		break;
	default:
		break;
	}

	platformColision();
}

void Enemy_Scorpion::Frame()
{
	switch (_enemyState)
	{
	case EnemyState::IDLE:
		_frameCount = 0;
		_frameX = 0;
		break;
	case EnemyState::SCOUT:
		_frameCount++;
		if (_frameCount >= 10)
		{
			_frameX++;
			if (_frameX > 3)
				_frameX = 0;
			_frameCount = 0;
		}
		break;
	case EnemyState::ATTACK:
		_frameCount++;
		if (_frameCount >= 10)
		{
			_frameX++;
			if (_frameX > 6)
				_frameX = 0;
			_frameCount = 0;
		}
		break;
	case EnemyState::DIE:
		break;
	default:
		break;
	}
}