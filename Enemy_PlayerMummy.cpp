#include "stdafx.h"
#include "Enemy_PlayerMummy.h"

void Enemy_PlayerMummy::EnemyAction()
{
	_probeY = _y + _image->getFrameHeight() / 2;
	switch (_enemyState)
	{
	case EnemyState::IDLE:
		RECT temp;
		//카메라 안으로 들어가면 SCOUT상태로 변함
		if (IntersectRect(&temp, &_enemyRect, &_cameraRect)) _enemyState = EnemyState::SCOUT;
		break;
	case EnemyState::SCOUT:
		Scout();				//움직이다 절벽/벽 을 만나면 반대편으로 돌아가도록 하는 함수
		Move();					//좌우로 움직이게 하는 함수

		Discovery();

		if (!IntersectRect(&temp, &_enemyRect, &_cameraRect)) _enemyState = EnemyState::IDLE;				//카메라 밖으로 나가면 IDLE상태로 변함
		break;
	case EnemyState::DISCOVERY:
		Tracking();				//적을 추적하는 함수
		Attack();				//적이 공격범위안에 들어올시 _enemyState를 ATTACK로 변경해주는 함수
		if (!IntersectRect(&temp, &_enemyRect, &_cameraRect)) _enemyState = EnemyState::IDLE;				//카메라 밖으로 나가면 IDLE상태로 변함
		break;
	case EnemyState::ATTACK:
		UnAttack();				//공격범위 밖으로 나가면 SCOUT
		if (_frameX >= 3 && _frameX <= 5)
		{
			_enemyAttackRect = _enemyAttackRangeRect;
		}
		else
		{
			_enemyAttackRect = RectMakeCenter(0, 0, 0, 0);
		}
		if (!IntersectRect(&temp, &_enemyRect, &_cameraRect)) _enemyState = EnemyState::IDLE;				//카메라 밖으로 나가면 IDLE상태로 변함
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

void Enemy_PlayerMummy::Frame()
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
			if (_frameX > 5)
				_frameX = 0;
			_frameCount = 0;
		}
		break;
	case EnemyState::DISCOVERY:
		_frameCount++;
		if (_frameCount >= 10)
		{
			_frameX++;
			if (_frameX > 5)
				_frameX = 0;
			_frameCount = 0;
		}
		break;
	case EnemyState::ATTACK:
		_frameCount++;
		if (_frameCount >= 10)
		{
			_frameX++;
			if (_frameX > 5)
				_frameX = 0;
			_frameCount = 0;
		}
		break;
	case EnemyState::DIE:
		/*_frameCount++;
		if (_frameCount >= 10)
		{
			if (_frameX < 10) _frameX++;
			_frameCount = 0;
			if (_frameX > 10)_die = true;
		}*/
		break;
	default:
		break;
	}
}
