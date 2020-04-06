#include "stdafx.h"
#include "playerEric.h"


playerEric::playerEric()
{
}


playerEric::~playerEric()
{
}

HRESULT playerEric::init(float x, float y)
{
	IMAGEMANAGER->addFrameImage("eric_die", "./image/Characters/eric_die.bmp", 792, 192, 8, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_idle", "./image/Characters/eric_idle.bmp", 156, 186, 2, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_move", "./image/Characters/eric_move.bmp", 744, 192, 8, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_jump", "./image/Characters/eric_jump.bmp", 372, 192, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_push", "./image/Characters/eric_push.bmp", 372, 192, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_breath", "./image/Characters/eric_breath.bmp", 270, 192, 3, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_poison", "./image/Characters/eric_poison.bmp", 630, 192, 7, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_trapdie", "./image/Characters/eric_trapdie.bmp", 174, 186, 2, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_hitState", "./image/Characters/eric_hitState.bmp", 93, 192, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_pressdie", "./image/Characters/eric_pressdie.bmp", 384, 192, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_headbutt", "./image/Characters/eric_headbutt.bmp", 888, 192, 8, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_mirra", "./image/Characters/eric_mirraddie.bmp", 216, 180, 3, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_stepladder", "./image/Characters/eric_stepladder.bmp", 336, 105, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_headbuttend", "./image/Characters/eric_headbuttend.bmp", 990, 186, 11, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("eric_stepladderend", "./image/Characters/eric_stepladderend.bmp", 186, 78, 2, 1, true, RGB(255, 0, 255));

	_eric.currentFrameX = _eric.currentFrameY = _eric.frameCount = 0;		 // 프레임 초기값 
	_eric.frameSpeed = 15;												     // 프레임 스피드 초기값
	_eric.state = STATE_IDLE;												 // 상태값 초기값
	_eric.posState = POSSTATE_GROUND;										 // 위치   초기값
	_eric.image = IMAGEMANAGER->findImage("eric_idle");						 // 이미지 초기값
	_eric.x = x;															 // x 
	_eric.y = y;															 // y
	_eric.rc = RectMake(_eric.x, _eric.y, _eric.image->getFrameWidth(), _eric.image->getFrameHeight());   // RECT
	_eric.hp = 3;
	_eric.isDead = false;// 체력 
	//========================== 점프 관련 ==================================//
	_ericJump = false;
	_eric.jumpPower = 15;
	_eric.gravity = 0.3;
	//========================== 이동 관련 ==================================//
	_eric.movePower = 2;
	_eric.movePowerCount = 0;
	//========================== 공격 관련 ==================================//
	_ericAttack = false;
	_ericUnable = false;
	//========================== 특이 상황 관련 =============================//
	_breathCount = 0;
	_breathFrameCount = 0;
	_gravity = 0;			// ERIC AIR 상태일 때 중력 
	_slidePower = 7;		// 
	_isSlide = false;		// 슬라이딩 
	_stop = false;
	_isItem = false;
	//========================== 충돌처리 초기화 ============================//
	_eric.probeX = _eric.x + _eric.image->getFrameWidth() / 2;
	_eric.probeY = _eric.y + _eric.image->getFrameHeight() / 2;
	return S_OK;
}

void playerEric::release()
{
}

void playerEric::update()
{
	if (!_isItem)  // 아이템을 사용하면 전부 멈춘다 
	{
		ericFrameCount();				    // image frame 증가 
		setEricImage();				        // image 세팅 

		if (!_stop)	  // 죽는거 표시하기 위해서 
		{
			if (_stopControl)				// 케릭터 선택 BOOL값
			{
				if (!_ericUnable) key();	// 전투 불능 상태가 아니면 key값 
			}
			if (_eric.state == STATE_ERIC_HEADBUTTEND) ericAttackMove();

			ericJump();								// 점프 
			ericHit();								// 맞을 때 이미지 

			// 점프가 아니면 픽셀충돌, 점프중에도 픽셀충돌 
			if (_eric.posState == POSSTATE_AIR)
			{
				isJumpPixelCollision();
				_isSlide = false;
				_eric.y += _gravity;
				if (_gravity < 5)	_gravity += 0.7;
				if (_eric.state != STATE_ERIC_JUMP) // 떨어질 때 점프하면서 떨어지기 위한 
				{
					_eric.state = STATE_ERIC_JUMP;
					_eric.currentFrameX = 2;
					_eric.image->setFrameX(_eric.currentFrameX);
				}
			}
			else if (_eric.posState == POSSTATE_GROUND)// 에릭의 위치가 공기중이면 
			{
				PixelCollision();
			}
		}
		ericDie(); 	//  플레이어 사망
		_eric.rc = RectMake(_eric.x, _eric.y, _eric.image->getFrameWidth(), _eric.image->getFrameHeight());   // RECT 갱신

	}
}

void playerEric::render()
{
	_eric.image->frameRender(CAMERAMANAGER->getWorDC(), _eric.x, _eric.y, _eric.currentFrameX, _eric.currentFrameY);
}

void playerEric::move()
{
}

void playerEric::key()
{
	//만약에 좌우 키를 누르면 
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		_eric.frameSpeed = 10;
		_breathCount = 0;
		if (_eric.state != STATE_PUSH) _eric.currentFrameY = 0;
		if (_isSlide && _eric.state != STATE_PUSH)  _isSlideOn = true;		// 슬라이딩을 활성화 시키기 위한 
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		_eric.frameSpeed = 10;
		_breathCount = 0;
		if (_eric.state != STATE_PUSH) _eric.currentFrameY = 1;
		if (_isSlide && _eric.state != STATE_PUSH)   _isSlideOn = true;		// 슬라이딩을 활성화 시키기 위한 
	}

	// 사다리타기 구현
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		if (_eric.state != STATE_STEPLADDER)	_eric.currentFrameY = 0;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		if (_eric.state != STATE_STEPLADDER)	_eric.currentFrameY = 0;
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		_eric.state = STATE_STEPLADDER;
		_eric.currentFrameY = 0;
		_eric.y -= 2.5;
		_eric.frameCount += 2;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		_eric.state = STATE_STEPLADDER;
		_eric.currentFrameY = 0;
		_eric.y += 2.5;
		_eric.frameCount += 2;
	}

	// 왼쪽 키를 지속적으로 누르면 
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		// 왼쪽 픽셀 충돌 
		PixelLeftCollision();
		if (_eric.state != STATE_PUSH)
		{
			_breathCount++;						 // 오래 뛰면 숨 이미지가 나타남 
			//==========이동 관련된 코드 =========//
			_eric.movePowerCount++;
			if (_eric.movePower <= 8)
			{
				if (_eric.movePowerCount > 4)
				{
					_eric.movePower += 0.5;
					_eric.movePowerCount = 0;
				}
			}
			//==========상태에 따라서 적용 
			if (_eric.state == STATE_ERIC_HEADBUTT || _eric.state == STATE_ERIC_JUMP || _eric.state == STATE_PUSH)
			{
				_eric.x -= _eric.movePower;
			}
			else if (_eric.state == STATE_PUSH)
			{
				// 아무것도 아니어야함 
			}
			else
			{
				_eric.state = STATE_MOVE;
				_eric.x -= _eric.movePower;
			}
			// 만약 슬라이딩 이라면 슬라이딩 시켜라 
			if (_breathCount > 10 && _eric.state != STATE_PUSH) _isSlide = true;
		}
	}
	// 오른쪽 키를 지속적으로 누르면 
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		//오른쪽 픽셀 충돌 
		PixelRightCollision();
		if (_eric.state != STATE_PUSH)
		{
			_breathCount++;						 // 오래 뛰면 숨 이미지가 나타남 
			//==========이동 관련된 코드 =========//
			_eric.movePowerCount++;
			if (_eric.movePower <= 8)
			{
				if (_eric.movePowerCount > 4)
				{
					_eric.movePower += 0.5;
					_eric.movePowerCount = 0;
				}
			}
			//==========상태에 따라서 적용 
			if (_eric.state == STATE_ERIC_HEADBUTT || _eric.state == STATE_ERIC_JUMP || _eric.state == STATE_PUSH)
			{
				_eric.x += _eric.movePower;
			}
			else if (_eric.state == STATE_PUSH)
			{
				// 아무것도 아니어야함 
			}
			else
			{
				_eric.state = STATE_MOVE;
				_eric.x += _eric.movePower;	 // 0 이 오른쪽 
			}

			if (_breathCount > 10 && _eric.state != STATE_PUSH) _isSlide = true;
		}
	}

	//  좌우키를 때면 
	if (KEYMANAGER->isOnceKeyUp(VK_LEFT) || KEYMANAGER->isOnceKeyUp(VK_RIGHT))
	{
		// 숨 카운트가 150이상이면 숨 에릭 이미지를 띄운다 
		if (_breathCount > 150)
		{
			_eric.state = STATE_BREATH;
			_eric.frameSpeed = 15;
			_eric.currentFrameX = 0;
			_eric.image->setFrameX(_eric.currentFrameX);
			_eric.movePower = 2;
			_breathCount = 0; // 숨카운트 
		}
		else // 숨 카운트가 아니면 
		{
			if (_eric.state == STATE_ERIC_HEADBUTT || _eric.state == STATE_ERIC_JUMP)
			{
				_eric.movePower = 2;
			}
			else
			{
				_eric.state = STATE_IDLE;
				_eric.movePower = 2;
				_eric.frameSpeed = 15;
				_eric.currentFrameX = 0;
			}
			_breathCount = 0; // 숨카운트 
		}
	}
	//===============슬라이딩을 하기 위한 구분 
	if (_isSlide)
	{
		_slideCount++;
		if (_slideCount > 5)
		{
			_slideCount = 0;
			_isSlide = false;
		}
	}
	// 만약 슬라이딩이 켜지면 
	if (_isSlideOn)
	{
		if (_eric.state == STATE_MOVE)
		{
			if (_eric.currentFrameY == 0)
			{
				_eric.x -= _slidePower;
			}
			else
			{
				_eric.x += _slidePower;
			}

			if (_slidePower >= 0)
			{
				_slidePower -= 0.1;
			}
			else
			{
				_slidePower = 7;
				_isSlideOn = false;


			}
		}
	}
	// 공격 구분 
	if (KEYMANAGER->isOnceKeyDown('D'))
	{
		_eric.state = STATE_ERIC_HEADBUTT;
		_eric.frameSpeed = 6;
		_eric.currentFrameX = 0;
		_eric.image->setFrameX(0);
		_ericAttack = true;
	}
	// 점프 구분 
	if (!_ericJump)
	{
		if (KEYMANAGER->isOnceKeyDown('F'))
		{
			_ericJump = true;
			_eric.state = STATE_ERIC_JUMP;
			_eric.posState = POSSTATE_AIR; // 점프이면 air
			_eric.jumpPower = 14;
			_eric.frameCount = 0;
			_eric.frameSpeed = 15;
			_eric.currentFrameX = 0;
			_eric.image->setFrameX(0);
			_jumpStart = _eric.y;
		}
	}


	// 191229 PM 03:18 테스트용 이동키 추가 (삭제해야함)
	if (KEYMANAGER->isStayKeyDown('O'))
	{
		_eric.y -= 20;
	}
	if (KEYMANAGER->isStayKeyDown('L'))
	{
		_eric.y += 20;
	}
}
// ERIC 프레임 카운트
void playerEric::ericFrameCount()
{
	_eric.frameCount++; // 프레임 카운터 증가 
	
	// 상태가 만약 공격 상태
	if (_eric.state == STATE_ERIC_HEADBUTT)
	{
		if (_eric.frameCount >= _eric.frameSpeed)
		{
			if (!_ericAttack)
			{
				_eric.currentFrameX--;
			}
			else
			{
				_eric.currentFrameX++;
			}

			_eric.image->setFrameX(_eric.currentFrameX);

			if (_eric.currentFrameX >= _eric.image->getMaxFrameX())
			{
				_ericAttack = false;
			}

			if (_eric.currentFrameX < 0)
			{
				_eric.state = STATE_IDLE;
				_eric.currentFrameX = 0;
				_eric.frameSpeed = 15;
			}
			_eric.frameCount = 0;
		}
	}
	else if (_eric.state == STATE_ERIC_HEADBUTTEND)  // 박치기를 한 상태 
	{
		if (_eric.frameCount >= _eric.frameSpeed)
		{
			_eric.currentFrameX++;
			_eric.image->setFrameX(_eric.currentFrameX);

			if (_eric.currentFrameX > _eric.image->getMaxFrameX()) // 조금 느리게 하고 싶음
			{
				_ericUnable = false;
				_eric.currentFrameX = 0;
				_eric.state = STATE_IDLE;
			}
			_eric.frameCount = 0;
		}
	}
	else if (_eric.state == STATE_ERIC_JUMP)   // 점프를 한 상태 
	{
		if (_eric.frameCount >= _eric.frameSpeed)
		{
			if (_eric.currentFrameX == _eric.image->getMaxFrameX()) _eric.currentFrameX = _eric.currentFrameX - 2;
			if (_eric.currentFrameX < _eric.image->getMaxFrameX())
			{
				_eric.currentFrameX++;
				_eric.image->setFrameX(_eric.currentFrameX);
			}
			_eric.frameCount = 0;
		}
		if (_eric.posState == POSSTATE_GROUND)
		{
			_eric.state = STATE_IDLE;
			_eric.currentFrameX = 0;
			_eric.image->setFrameX(_eric.currentFrameX);
		}
	}
	else if (_eric.state == STATE_STEPLADDER)
	{
		_eric.frameCount--;
		if (_eric.frameCount >= _eric.frameSpeed)
		{
			_eric.currentFrameX++;
			_eric.image->setFrameX(_eric.currentFrameX);
			if (_eric.currentFrameX > _eric.image->getMaxFrameX())
			{
				_eric.currentFrameX = 0;
			}
			_eric.frameCount = 0;
		}
	}
	else  // 그 이외의 상태 
	{
		if (_eric.frameCount >= _eric.frameSpeed)
		{
			_eric.currentFrameX++;
			_eric.image->setFrameX(_eric.currentFrameX);


			if (_eric.currentFrameX > _eric.image->getMaxFrameX())
			{
				//죽는 상태라면 
				if (_eric.state == STATE_DIE ||
					_eric.state == STATE_MIRRA ||
					_eric.state == STATE_PRESSDIE ||
					_eric.state == STATE_POISON ||
					_eric.state == STATE_TRAPDIE)
				{
					_eric.isDead = true;
					_eric.x = -1000;
					_eric.y = -1000;
				}
				else
				{
					_eric.currentFrameX = 0;
				}
				//숨쉬기 위한 
				if (_eric.state == STATE_BREATH) _breathFrameCount++;
				if (_breathFrameCount > 3)
				{
					_eric.state = STATE_IDLE;
					_breathFrameCount = 0;
				}
			}
			_eric.frameCount = 0;
		}
	}
}
// ERIC 점프하는 함수 
void playerEric::ericJump()
{
	if (_ericJump)
	{
		if (_eric.posState != POSSTATE_GROUND)
		{
			_eric.y -= _eric.jumpPower;
			_eric.jumpPower -= _eric.gravity;
		}
		// 점프 초기값 && 공기중이라면 
		if (_jumpStart <= _eric.y && _eric.posState == POSSTATE_AIR)
		{
			_eric.y = _jumpStart;
			_ericJump = false;
			_eric.state = STATE_IDLE;
			_eric.currentFrameX = 0;
			_eric.image->setFrameX(0);
		}
	}

}
// ERIC 공격하면 밀려나는 함수
void playerEric::ericAttackMove()
{
	if (_eric.currentFrameY == 0)
	{
		_eric.x -= 0.5;
	}
	else
	{
		_eric.x += 0.5;
	}
	// 픽셀 충돌로 활성화 되지 않음 
	if (_eric.currentFrameX >= 0 && _eric.currentFrameX < 4)
	{
		_eric.y -= 1;
	}
	else if (_eric.currentFrameX > 4 && _eric.currentFrameX < _eric.image->getMaxFrameX() - 1)
	{
		_eric.y += 1;
	}
}
// ERIC 맞을 떄 함수 
void playerEric::ericHit()
{
	//맞을 때 히트값을 조정해주셈 
	if (_isHit)
	{
		_hitCount++;
		if (_eric.state != STATE_HIT)  _eric.state = STATE_HIT;
		if (_hitCount > 20)
		{
			_isHit = false;
			_ericUnable = false;
			_hitCount = 0;
			_eric.state = STATE_IDLE;
			_eric.frameCount = 0;
			_eric.currentFrameX = 0;
			_eric.image->setFrameX(_eric.currentFrameX);
		}
	}
}
// ERIC 이미지를 설정하는 함수  
void playerEric::setEricImage()
{
	if (_eric.hp == 0)
	{
		_eric.state = STATE_DIE;
	}
	switch (_eric.state)
	{
	case STATE_IDLE:
		_eric.image = IMAGEMANAGER->findImage("eric_idle");
		break;
	case STATE_MOVE:
		_eric.image = IMAGEMANAGER->findImage("eric_move");
		break;
	case STATE_ERIC_JUMP:
		_eric.image = IMAGEMANAGER->findImage("eric_jump");
		break;
	case STATE_ERIC_HEADBUTT:
		_eric.image = IMAGEMANAGER->findImage("eric_headbutt");
		break;
	case STATE_ERIC_HEADBUTTEND:
		_eric.image = IMAGEMANAGER->findImage("eric_headbuttend");
		break;
	case STATE_OLAF_GUARD:
		break;
	case STATE_OLAF_FLY:
		break;
	case STATE_BALEOG_ARROW:
		break;
	case STATE_BALEOG_SWORD:
		break;
	case STATE_BREATH:
		_eric.image = IMAGEMANAGER->findImage("eric_breath");
		break;
	case STATE_HIT:
		_eric.image = IMAGEMANAGER->findImage("eric_hitState");
		break;
	case STATE_PUSH:
		_eric.image = IMAGEMANAGER->findImage("eric_push");
		break;
	case STATE_DIE:
		_eric.image = IMAGEMANAGER->findImage("eric_die");
		break;
	case STATE_POISON:
		_eric.image = IMAGEMANAGER->findImage("eric_poison");
		break;
	case STATE_MIRRA:
		_eric.image = IMAGEMANAGER->findImage("eric_mirra");
		break;
	case STATE_PRESSDIE:
		_eric.image = IMAGEMANAGER->findImage("eric_pressdie");
		break;
	case STATE_TRAPDIE:
		_eric.image = IMAGEMANAGER->findImage("eric_trapdie");
		break;
	case STATE_STEPLADDER:
		_eric.image = IMAGEMANAGER->findImage("eric_stepladder");
		break;
	case STATE_STEPLADDEREND:
		_eric.image = IMAGEMANAGER->findImage("eric_stepladderend");
		break;
	}
}
//픽셀 충돌 
void playerEric::PixelCollision()
{
	_eric.probeY = _eric.y + _eric.image->getFrameHeight();

	for (int i = _eric.probeY - 6; i < _eric.probeY + 10; ++i)
	{
		COLORREF getPixel_Bottom = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), (_eric.rc.left + _eric.rc.right) / 2, i);

		COLORREF getPixel_TOP = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), (_eric.rc.left + _eric.rc.right) / 2, _eric.y);

		int r = GetRValue(getPixel_Bottom);
		int g = GetGValue(getPixel_Bottom);
		int b = GetBValue(getPixel_Bottom);

		int r_top = GetRValue(getPixel_TOP);
		int g_top = GetGValue(getPixel_TOP);
		int b_top = GetBValue(getPixel_TOP);

		if ((r == 255 && g == 0 && b == 0) || (r == 255 && g == 255 && b == 0))
		{
			if (!(r_top == 255 && g_top == 0 && b_top == 0))
			{
				if (!(_eric.state == STATE_STEPLADDER || _eric.state == STATE_STEPLADDEREND))
				{

					_eric.y = i - _eric.image->getFrameHeight();
					_eric.posState = POSSTATE_GROUND;
					if (_gravity > 0)
					{
						_gravity = 0;
					}
					break;
				}
			}
		}
		else
		{
			_eric.posState = POSSTATE_AIR;
		}
	}
}
void playerEric::PixelRightCollision()
{
	_eric.probeX = _eric.x + _eric.image->getFrameWidth(); // _eric.right  

	COLORREF getPixel_RIGHT = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), _eric.probeX + 2, _eric.y);

	int r = GetRValue(getPixel_RIGHT);
	int g = GetGValue(getPixel_RIGHT);
	int b = GetBValue(getPixel_RIGHT);

	if (!((r == 255 && g == 0 && b == 0) || (r == 255 && g == 0 && b == 255)))
	{
		if (_eric.posState == POSSTATE_GROUND)
		{
			if (_eric.state != STATE_PUSH)
			{
				_eric.state = STATE_PUSH;
				_eric.currentFrameX = 0;
				_eric.image->setFrameX(0);
			}
		}
		_eric.x = _eric.probeX - _eric.image->getFrameWidth();
	}
}
void playerEric::PixelLeftCollision()
{
	_eric.probeX = _eric.x - 3;

	COLORREF getPixel_LEFT = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), _eric.probeX, _eric.y);

	int r = GetRValue(getPixel_LEFT);
	int g = GetGValue(getPixel_LEFT);
	int b = GetBValue(getPixel_LEFT);

	if (!((r == 255 && g == 0 && b == 0) || (r == 255 && g == 0 && b == 255)))
	{
		if (_eric.posState == POSSTATE_GROUND)
		{
			if (_eric.state != STATE_PUSH)
			{
				_eric.state = STATE_PUSH;
				_eric.currentFrameX = 0;
				_eric.image->setFrameX(0);
			}
		}
		_eric.x = _eric.probeX + 6;
	}
}
void playerEric::isJumpPixelCollision()
{
	// 점프 중일 떄 왼쪽아래 모서리 픽셀 충돌 
	_eric.probeX = _eric.x - 3;

	COLORREF getPixel_AIR2 = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), _eric.probeX, _eric.y + _eric.image->getFrameHeight());

	int r2 = GetRValue(getPixel_AIR2);
	int g2 = GetGValue(getPixel_AIR2);
	int b2 = GetBValue(getPixel_AIR2);

	if ((r2 == 255 && g2 == 255 && b2 == 0))
	{
		if (_eric.posState == POSSTATE_AIR)
		{
			_eric.x = _eric.probeX + 8;
		}
	}

	// 점프 중일 떄 오른쪽아래 모서리 픽셀 충돌 
	_eric.probeX = _eric.x + _eric.image->getFrameWidth(); // _eric.right  

	COLORREF getPixel_AIR = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), _eric.probeX + 8, _eric.y + _eric.image->getFrameHeight());

	int r = GetRValue(getPixel_AIR);
	int g = GetGValue(getPixel_AIR);
	int b = GetBValue(getPixel_AIR);

	if ((r == 255 && g == 255 && b == 0))
	{
		if (_eric.posState == POSSTATE_AIR)
		{
			_eric.x = _eric.probeX - _eric.image->getFrameWidth() - 8;
		}
	}


	// 점프 중일 때 픽셀 충돌 (Y축)
	_eric.probeY = _eric.y + _eric.image->getFrameHeight();

	for (int i = _eric.probeY; i < _eric.probeY + 10; ++i)
	{
		COLORREF getPixel_Bottom = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), (_eric.rc.left + _eric.rc.right) / 2, i);

		int r_Bottom = GetRValue(getPixel_Bottom);
		int g_Bottom = GetGValue(getPixel_Bottom);
		int b_Bottom = GetBValue(getPixel_Bottom);

		if (!(r_Bottom == 255 && g_Bottom == 0 && b_Bottom == 255) && _eric.currentFrameX > 0)
		{
			_eric.y = i - _eric.image->getFrameHeight();
			_eric.posState = POSSTATE_GROUND;
			if (_gravity > 0)
			{
				_gravity = 0;
			}
			_eric.currentFrameX = 0;
			_eric.image->setFrameX(0);
			_ericJump = false;
			_eric.state = STATE_IDLE;
			break;
		}
		else
		{
			_eric.posState = POSSTATE_AIR;
		}
	}
}
//ERIC 죽었을 때
void playerEric::ericDie()
{
	if (_eric.hp <= 0)
	{
		_eric.state == STATE_DIE;
		_stop = true;
	}
}
