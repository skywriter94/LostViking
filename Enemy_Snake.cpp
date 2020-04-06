#include "stdafx.h"
#include "Enemy_Snake.h"



void Enemy_Snake::EnemyAction()
{
	switch (_enemyState)
	{
	case EnemyState::IDLE:
		RECT temp;
		//ī�޷� ������ ������ _enemyState�� SCOUT�� �ٲ�
		if (IntersectRect(&temp, &_enemyRect, &_cameraRect)) _enemyState = EnemyState::SCOUT;
		break;
	case EnemyState::SCOUT:
		Attack();			//���� �߰��ϴ� �Լ�
		if (!IntersectRect(&temp, &_enemyRect, &_cameraRect)) _enemyState = EnemyState::IDLE;				//ī�޶� ������ ������ IDLE���·� ����
		break;
	case EnemyState::ATTACK:
		UnAttack();				//���� ���ݹ��� ������ ������ SCOUT
		AttackDirection();		//���� ���� �÷��̾����� ����
		if (_frameX ==5)
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

void Enemy_Snake::Frame()
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
			if (_frameX > 1)
				_frameX = 0;
			_frameCount = 0;
		}
		break;
	case EnemyState::ATTACK:
		_frameCount++;
		if (_frameCount >= 15)
		{
			_frameX++;
			if (_frameX > 5)
				_frameX = 0;
			_frameCount = 0;
		}
		break;
	case EnemyState::DIE:
		_frameCount++;
		if (_frameCount >= 10)
		{
			_frameX++;
			if (_frameX > 10)
				_die = true;
			_frameCount = 0;
		}
		break;
	default:
		break;
	}
}