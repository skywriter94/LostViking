#include "stdafx.h"
#include "PlayerOlaf.h"



PlayerOlaf::PlayerOlaf()
{
}
PlayerOlaf::~PlayerOlaf()
{
}



HRESULT PlayerOlaf::init(float x, float y)
{
	// 올라프 이미지, 좌표, 렉트, HP, 프레임 카운트, 상태, 프레임 스피드, 쉴드 상태 선언
	init_SettingOlafImg();
	_olaf.image = IMAGEMANAGER->findImage("Olaf_Idle_ShieldForward");
	_olaf.x = x;
	_olaf.y = y;
	_olaf.rc = RectMake(_olaf.x, _olaf.y, _olaf.image->getFrameWidth(), _olaf.image->getFrameHeight());
	_olaf.hp = 3;
	_olaf.frameCount = _olaf.currentFrameX = _olaf.currentFrameY = _olaf.gravity = 0;
	_olaf.state = STATE_IDLE;
	_olaf.posState = POSSTATE_GROUND;
	_olaf.frameSpeed = 10;
	_olaf.isDead = false;
	_olaf.isHit = false;
	_isShieldUp = false;
	
	_currentLadderState = 0;
	_beforeLadderState = 0;
	_olafStateTemp = (int)_olaf.state;
	_olafPosStateTemp = (int)_olaf.posState;
	_selectedLadderIdx = 0;
	_isItem = false;
	init_SetLadder();
	return S_OK;
}

void PlayerOlaf::release()
{

}

void PlayerOlaf::update()
{
	if (!_isItem)
	{
		if (_stopControl) KeyControl();
		SetOlafState();
		SetOlafPosState();
		PixelCollision();
		UpdateFrame();
	}

}

void PlayerOlaf::render()
{
	if (!_olaf.isDead)
	{
		if (KEYMANAGER->isToggleKey('T'))
		{
			for (int i = 0; i < 9; ++i)
			{
				Rectangle(CAMERAMANAGER->getWorDC(), _ladderRect[i]);
			}
			Rectangle(CAMERAMANAGER->getWorDC(), _olaf.rc);

			char checkRGB_BC[256];
			sprintf_s(checkRGB_BC, sizeof(checkRGB_BC), "BC R: %d, BC G: %d, BC B: %d", r_BC, g_BC, b_BC);
			TextOut(CAMERAMANAGER->getWorDC(), _olaf.x, _olaf.y - 20, checkRGB_BC, strlen(checkRGB_BC));

			char checkRGB_TC[256];
			sprintf_s(checkRGB_TC, sizeof(checkRGB_TC), "TC R: %d, TC G: %d, TC B: %d", r_TC, g_TC, b_TC);
			TextOut(CAMERAMANAGER->getWorDC(), _olaf.x, _olaf.y - 40, checkRGB_TC, strlen(checkRGB_TC));

			char checkState[256];
			sprintf_s(checkState, sizeof(checkState), "State: %d", _olaf.state);
			TextOut(CAMERAMANAGER->getWorDC(), _olaf.x, _olaf.y - 80, checkState, strlen(checkState));

			char checkPosState[256];
			sprintf_s(checkPosState, sizeof(checkPosState), "PosState: %d", _olaf.posState);
			TextOut(CAMERAMANAGER->getWorDC(), _olaf.x, _olaf.y - 100, checkPosState, strlen(checkPosState));

			char checkGravityAccel[128];
			sprintf_s(checkGravityAccel, sizeof(checkGravityAccel), "PosState: %f", _olaf.gravity);
			TextOut(CAMERAMANAGER->getWorDC(), _olaf.x, _olaf.y - 120, checkGravityAccel, strlen(checkGravityAccel));

			char checkDirection[128];
			sprintf_s(checkDirection, sizeof(checkDirection), "Direction : %d", _olaf.currentFrameY);
			TextOut(CAMERAMANAGER->getWorDC(), _olaf.x, _olaf.y - 120, checkDirection, strlen(checkDirection));
		}
		_olaf.image->frameRender(CAMERAMANAGER->getWorDC(), _olaf.x, _olaf.y, _olaf.currentFrameX, _olaf.currentFrameY);
	}
}



void PlayerOlaf::init_SettingOlafImg()
{
	// 올라프 움직이는 기본 이미지들
	IMAGEMANAGER->addFrameImage("Olaf_Idle_ShieldForward", "image/Characters/Olaf_Idle_ShieldForward.bmp", 93, 192, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Idle_ShieldUp", "image/Characters/Olaf_Idle_ShieldUp.bmp", 84, 192, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Move_ShieldForward", "image/Characters/Olaf_Move_ShieldForward.bmp", 776, 200, 8, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Move_ShieldUp", "image/Characters/Olaf_Move_ShieldUp.bmp", 522, 200, 6, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Push", "image/Characters/Olaf_Push.bmp", 392, 196, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Climing", "image/Characters/Olaf_Climing.bmp", 324, 186, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Climing_Start", "image/Characters/Olaf_Climing_Start.bmp", 186, 192, 2, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Fall", "image/Characters/Olaf_Fall.bmp", 192, 192, 2, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_SlowFall", "image/Characters/Olaf_SlowFall.bmp", 190, 196, 2, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_SlowFall_Land", "image/Characters/Olaf_SlowFall_Land.bmp", 171, 196, 2, 2, true, RGB(255, 0, 255));

	// 올라프 죽는 이미지들
	IMAGEMANAGER->addFrameImage("Olaf_Dead_Normal", "image/Characters/Olaf_Dead_Normal.bmp", 674, 200, 7, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Dead_Poison", "image/Characters/eric_poison.bmp", 630, 192, 7, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Dead_Mira", "image/Characters/Olaf_Dead_Mira.bmp", 252, 186, 3, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Dead_Crush", "image/Characters/Olaf_Dead_Crush.bmp", 270, 186, 3, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Dead_Pierce", "image/Characters/Olaf_Dead_Pierce.bmp", 93, 198, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Dead_Drown", "image/Characters/Olaf_Dead_Drown.bmp", 425, 190, 4, 2, true, RGB(255, 0, 255));

	// 올라프 기타 이미지들
	IMAGEMANAGER->addFrameImage("Olaf_FallHit", "image/Characters/Olaf_FallHit.bmp", 476, 198, 5, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Olaf_Hit", "image/Characters/Olaf_Hit.bmp", 94, 187, 1, 2, true, RGB(255, 0, 255));
}

void PlayerOlaf::init_SetLadder()
{
	_ladderRect[0] = RectMake(290, 279, 92, 524);
	_ladderRect[1] = RectMake(721, 615, 93, 528);
	_ladderRect[2] = RectMake(1057, 903, 93, 480);
	_ladderRect[3] = RectMake(1394, 1191, 93, 336);
	_ladderRect[4] = RectMake(1776, 1047, 93, 528);
	_ladderRect[5] = RectMake(2255, 1479, 93, 288);
	_ladderRect[6] = RectMake(2649, 279, 78, 432);
	_ladderRect[7] = RectMake(2737, 1479, 93, 432);
	_ladderRect[8] = RectMake(3600, 1335, 93, 864);
}

void PlayerOlaf::UpdateFrame()
{
	// 픽셀 충돌을 위한 선언
	_olaf.frameCount++;
	if (!(r_BC == 255 && g_BC == 0 && b_BC == 0)) // 사다리 픽셀인 경우
		_olaf.rc = RectMake(_olaf.x, _olaf.y, _olaf.image->getFrameWidth(), _olaf.image->getFrameHeight());
	else
		_olaf.rc = RectMake(_olaf.x, _olaf.y, _olaf.image->getFrameWidth(), _olaf.image->getFrameHeight() + 10);

	if (_olaf.frameCount > _olaf.frameSpeed)
	{
		if (_olaf.posState != POSSTATE_STEPLADDER)
		{
			_olaf.currentFrameX++;
		}
		else
		{
			if (KEYMANAGER->isStayKeyDown(VK_UP) || KEYMANAGER->isStayKeyDown(VK_DOWN))
			{
				_olaf.currentFrameX++;
			}
		}
		_olaf.image->setFrameX(_olaf.currentFrameX);
		if (_olaf.currentFrameX > _olaf.image->getMaxFrameX())
		{
			ResetAnimation1();
			if (_olaf.state == STATE_DIE || _olaf.state == STATE_POISON || _olaf.state == STATE_MIRRA ||
				_olaf.state == STATE_PRESSDIE || _olaf.state == STATE_TRAPDIE || _olaf.state == STATE_DROWNED)
			{
				_olaf.isHit = false;
				_olaf.isDead = true;
			}
			if (_olaf.state == STATE_FALLHIT)
			{
				_olaf.isHit = false;
				if (_olaf.hp != 0)
				{
					_olaf.state = STATE_IDLE;
				}
			}
			if (_olaf.state == STATE_HIT)
			{
				if (_olaf.hp != 0)
				{
					_olaf.state = STATE_IDLE;
					_olaf.isHit = false;
				}
				else
				{
					_olaf.state = STATE_DIE;
					ResetAnimation1();
				}
			}
			_olaf.currentFrameX = 0;
		}
		_olaf.frameCount = 0;
	}
}

void PlayerOlaf::KeyControl()
{
	if (!_olaf.isHit)
	{
		// 왼쪽 키
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			ResetAnimation2();
			if (_olaf.posState == POSSTATE_STEPLADDER)
			{
				_olaf.posState = POSSTATE_LADDERFALL;
				_olaf.state = STATE_IDLE;
			}
		}
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			ResetAnimation1();
			if (_olaf.posState == POSSTATE_GROUND)
			{
				_olaf.state = STATE_MOVE;
			}
			_olaf.x -= PLAYER_SPEED;
			_olaf.currentFrameY = 1;
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
		{
			ResetAnimation2();
			if (_olaf.posState == POSSTATE_GROUND) _olaf.state = STATE_IDLE;
		}

		// 오른쪽 키
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			ResetAnimation2();
			if (_olaf.posState == POSSTATE_STEPLADDER)
			{
				_olaf.posState = POSSTATE_LADDERFALL;
				_olaf.state = STATE_IDLE;
			}
		}
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			ResetAnimation1();
			if (_olaf.posState == POSSTATE_GROUND)
			{
				_olaf.state = STATE_MOVE;
			}
			_olaf.x += PLAYER_SPEED;
			_olaf.currentFrameY = 0;
		}
		if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
		{
			ResetAnimation2();
			if (_olaf.posState == POSSTATE_GROUND) _olaf.state = STATE_IDLE;
		}

		
		if (KEYMANAGER->isStayKeyDown('O'))
		{
			_olaf.y -= 5;
		}

		if (KEYMANAGER->isStayKeyDown('L'))
		{
			_olaf.y += 5;
		}

		// 방패 키
		if ((KEYMANAGER->isOnceKeyDown('D') || KEYMANAGER->isOnceKeyDown(VK_SPACE)) && _olaf.posState != POSSTATE_STEPLADDER)
		{
			ResetAnimation2();
			_isShieldUp = !_isShieldUp;
		}
	}
}

void PlayerOlaf::SetOlafState()
{
	switch (_olaf.state)
	{
		// 올라프 기본 움직임들
		case STATE_IDLE:
			if (!_isShieldUp)
			{
				_olaf.image = IMAGEMANAGER->findImage("Olaf_Idle_ShieldForward");
				_currentPosstate = 1;
			}
			else
			{
				_olaf.image = IMAGEMANAGER->findImage("Olaf_Idle_ShieldUp");
				_currentPosstate = 2;
			}
			ResetAnimation1();
			ResetAnimation4();
			break;

		case STATE_MOVE:
			if (_olaf.state == STATE_PUSH)
			{
				break;
			}
			if (!_isShieldUp)
			{
				_olaf.image = IMAGEMANAGER->findImage("Olaf_Move_ShieldForward");
				_currentPosstate = 3;
			}
			else
			{
				_olaf.image = IMAGEMANAGER->findImage("Olaf_Move_ShieldUp");
				_currentPosstate = 4;
			}
			ResetAnimation1();
			ResetAnimation4();
			break;

		// 올라프 죽는 조건들
		case STATE_DIE:
			_olaf.image = IMAGEMANAGER->findImage("Olaf_Dead_Normal");
			ResetAnimation1();
			break;

		case STATE_POISON:
			_olaf.image = IMAGEMANAGER->findImage("eric_poison");
			ResetAnimation1();
			break;

		case STATE_MIRRA:
			_olaf.image = IMAGEMANAGER->findImage("Olaf_Dead_Mira");
			ResetAnimation1();
			break;

		case STATE_PRESSDIE:
			_olaf.image = IMAGEMANAGER->findImage("Olaf_Dead_Crush");
			ResetAnimation1();
			break;

		case STATE_TRAPDIE:
			_olaf.image = IMAGEMANAGER->findImage("Olaf_Dead_Pierce");
			ResetAnimation1();
			break;

		case STATE_DROWNED:
			_olaf.image = IMAGEMANAGER->findImage("Olaf_Dead_Drown");
			ResetAnimation1();
			break;

		// 기타 조건들
		case STATE_FALLHIT:
			_olaf.image = IMAGEMANAGER->findImage("Olaf_FallHit");
			ResetAnimation1();
			break;

		case STATE_PUSH:
			_olaf.image = IMAGEMANAGER->findImage("Olaf_Push");
			ResetAnimation1();

		case STATE_HIT:
			_olaf.image = IMAGEMANAGER->findImage("Olaf_Hit");
			ResetAnimation1();
		default:
			break;
	}
}

void PlayerOlaf::SetOlafPosState()
{
	switch (_olaf.posState)
	{
		case POSSTATE_GROUND:
			if (_olaf.gravity > 0)
			{
				if (_olaf.gravity >= 13)
				{
					--_olaf.hp;
					_olaf.y -= (_olaf.gravity - 1);
					_olaf.gravity = 0;
					if (_olaf.hp > 0)
					{
						_olaf.state = STATE_FALLHIT;
					}
					else
					{
						_olaf.state = STATE_DIE;
					}
					_olaf.isHit = true;
				}
				else if (_olaf.gravity < 13)
				{
					_olaf.y -= (_olaf.gravity - 1);
					_olaf.gravity = 0;
				}
				_currentPosstate = 5;
			}
			ResetAnimation4();
			ResetAnimation1();
			break;

		case POSSTATE_AIR:
			if (_olaf.state == STATE_DIE || _olaf.state == STATE_POISON || _olaf.state == STATE_MIRRA ||
				_olaf.state == STATE_PRESSDIE || _olaf.state == STATE_TRAPDIE || _olaf.state == STATE_DROWNED || _olaf.state == STATE_FALLHIT)
			{
				break;
			}
			else
			{
				if (!_isShieldUp)
				{
					_olaf.image = IMAGEMANAGER->findImage("Olaf_Fall");
					_olaf.gravity += GRAVITY;
					_olaf.y += _olaf.gravity;
				}
				else
				{
					if (KEYMANAGER->isStayKeyDown(VK_LEFT) || KEYMANAGER->isStayKeyDown(VK_RIGHT))
						_olaf.image = IMAGEMANAGER->findImage("Olaf_SlowFall");
					else
						_olaf.image = IMAGEMANAGER->findImage("Olaf_SlowFall_Land");

					_olaf.gravity = 2;
					_olaf.y += _olaf.gravity;
				}
			}
			ResetAnimation1();
			break;

		case POSSTATE_STEPLADDER:
			if (_olaf.rc.top > _ladderRect[_selectedLadderIdx].top || _olaf.rc.bottom < _ladderRect[_selectedLadderIdx].top)
			{
				_olaf.image = IMAGEMANAGER->findImage("Olaf_Climing");
				_currentLadderState = 1;
			}
			else
			{
				_olaf.image = IMAGEMANAGER->findImage("Olaf_Climing_Start");
				_currentLadderState = 2;
			}
			ResetAnimation3();
			break;

		case POSSTATE_LADDERFALL:
			if (!_isShieldUp)
			{
				_olaf.image = IMAGEMANAGER->findImage("Olaf_Fall");
				_olaf.gravity += GRAVITY;
				_olaf.y += _olaf.gravity;
			}
			else
			{
				_olaf.image = IMAGEMANAGER->findImage("Olaf_SlowFall");
				_olaf.gravity = 2;
				_olaf.y += _olaf.gravity;
			}
			ResetAnimation1();
			break;
		
		case POSSTATE_LADDEREXIT:
			ResetAnimation1();
			break;
	}
}


void PlayerOlaf::PixelCollision()
{
	// 올라프 왼쪽 영역 픽셀 검색
	COLORREF getPixel_LT = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), _olaf.rc.left, _olaf.rc.top);
	int r_LT = GetRValue(getPixel_LT);
	int g_LT = GetGValue(getPixel_LT);
	int b_LT = GetBValue(getPixel_LT);
	COLORREF getPixel_LB = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), _olaf.rc.left, _olaf.rc.bottom);
	int r_LB = GetRValue(getPixel_LB);
	int g_LB = GetGValue(getPixel_LB);
	int b_LB = GetBValue(getPixel_LB);

	// 올라프 오른쪽 영역 픽셀 검색
	COLORREF getPixel_RT = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), _olaf.rc.right, _olaf.rc.top);
	int r_RT = GetRValue(getPixel_RT);
	int g_RT = GetGValue(getPixel_RT);
	int b_RT = GetBValue(getPixel_RT);
	COLORREF getPixel_RB = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), _olaf.rc.right, _olaf.rc.bottom);
	int r_RB = GetRValue(getPixel_RB);
	int g_RB = GetGValue(getPixel_RB);
	int b_RB = GetBValue(getPixel_RB);

	// 왼쪽 영역 처리
	if (_olaf.posState == POSSTATE_GROUND && _olaf.posState != POSSTATE_STEPLADDER) // 땅에 있을때
	{
		ResetAnimation1();
		if ((r_LT == 255 && g_LT == 255 && b_LT == 0))
		{
			_olaf.image = IMAGEMANAGER->findImage("Olaf_Push");
			_olaf.x += PLAYER_SPEED;
		}
	}
	else if (_olaf.posState != POSSTATE_GROUND)
	{
		ResetAnimation1();
		if ((r_LB == 255 && g_LB == 255 && b_LB == 0) || (r_LT == 255 && g_LT == 255 && b_LT == 0))
		{
			_olaf.x += PLAYER_SPEED;
		}
	}

	// 오른쪽 영역 처리
	if (_olaf.posState == POSSTATE_GROUND && _olaf.posState != POSSTATE_STEPLADDER) // 땅에 있을때
	{
		ResetAnimation1();
		if ((r_RT == 255 && g_RT == 255 && b_RT == 0))
		{
			_olaf.image = IMAGEMANAGER->findImage("Olaf_Push");
			_olaf.x -= PLAYER_SPEED;
		}
	}
	else if (_olaf.posState != POSSTATE_GROUND)
	{
		ResetAnimation1();
		if ((r_RB == 255 && g_RB == 255 && b_RB == 0) || (r_RT == 255 && g_RT == 255 && b_RT == 0))
		{
			_olaf.x -= PLAYER_SPEED;
		}
	}

	// 아래 영역 픽셀 검색 & 처리
	for (int i = _olaf.rc.bottom - 4; i < _olaf.rc.bottom + 10; ++i)
	{
		getPixel_BC = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), (_olaf.rc.right + _olaf.rc.left) / 2, i);
		r_BC = GetRValue(getPixel_BC);
		g_BC = GetGValue(getPixel_BC);
		b_BC = GetBValue(getPixel_BC);

		if (r_BC == 255 && g_BC == 255 && b_BC == 0) // 땅 픽셀인 경우
		{
			ResetAnimation1();
			_olaf.y = i - _olaf.image->getHeight() / 2;
			_olaf.posState = POSSTATE_GROUND;
			break;
		}
		else if(r_BC == 255 && g_BC == 0 && b_BC == 255) // 공중 픽셀인 경우
		{
			ResetAnimation1();
			_olaf.posState = POSSTATE_AIR;
		}
		else if (r_BC == 255 && g_BC == 0 && b_BC == 0) // 사다리 픽셀인 경우
		{
			ResetAnimation1();
			if (!_olaf.isHit)
			{
				if (_olaf.posState == POSSTATE_AIR)
				{
					ResetAnimation1();
					_olaf.posState = POSSTATE_GROUND;
					_olaf.y = i - _olaf.image->getHeight() / 2;
				}
				else if (_olaf.posState == POSSTATE_GROUND)
				{
					for (int i = 0; i < 9; ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_olaf.rc, &_ladderRect[i]))
						{

							if (KEYMANAGER->isStayKeyDown(VK_DOWN))
							{
								ResetAnimation1();
								_olaf.posState = POSSTATE_STEPLADDER;
							}
							_selectedLadderIdx = i;
							break;
						}
					}
				}
				else if (_olaf.posState == POSSTATE_STEPLADDER)
				{
					_olaf.gravity = 0;
					if ((_ladderRect[_selectedLadderIdx].bottom - _olaf.rc.bottom) >= 0)
					{
						_olaf.x = (_ladderRect[_selectedLadderIdx].right + _ladderRect[_selectedLadderIdx].left) / 2 - _olaf.image->getCenterX();
						if (KEYMANAGER->isStayKeyDown(VK_DOWN))
						{
							ResetAnimation1();
							_olaf.y += PLAYER_SPEED;
							break;
						}
						else if (KEYMANAGER->isStayKeyDown(VK_UP))
						{
							ResetAnimation1();
							_olaf.y -= PLAYER_SPEED;
							break;
						}
					}
				}
			}
		}
	}

	// 위 영역 픽셀 검색 & 처리
	for (int i = _olaf.rc.top - 10; i < _olaf.rc.top + 4; i++)
	{
		getPixel_TC = GetPixel(IMAGEMANAGER->findImage("BG")->getMemDC(), (_olaf.rc.right + _olaf.rc.left) / 2, i);
		r_TC = GetRValue(getPixel_TC);
		g_TC = GetGValue(getPixel_TC);
		b_TC = GetBValue(getPixel_TC);

		if ((r_TC == 255 && g_TC == 0 && b_TC == 0))
		{
			if (!_olaf.isHit)
			{
				for (int i = 0; i < 9; ++i)
				{
					RECT temp;
					if (IntersectRect(&temp, &_olaf.rc, &_ladderRect[i]))
					{
						if (KEYMANAGER->isOnceKeyDown(VK_UP) && _olaf.posState != POSSTATE_STEPLADDER)
						{
							ResetAnimation1();
							_selectedLadderIdx = i;
							_olaf.y -= PLAYER_SPEED * 3;
							_olaf.x = (_ladderRect[i].right + _ladderRect[i].left) / 2 - _olaf.image->getCenterX();
							_olaf.posState = POSSTATE_STEPLADDER;
						}
						break;
					}
				}
			}
		}
		break;
	}
}

void PlayerOlaf::ResetAnimation1()
{
	if ((int)_olaf.state != (int)_olafStateTemp)
	{
		_olaf.currentFrameX = 0;
		_olaf.image->setFrameX(0);
		_olaf.frameCount = 0;
	}
	_olafStateTemp = (int)_olaf.state;
}
void PlayerOlaf::ResetAnimation2()
{
	_olaf.currentFrameX = 0;
	_olaf.image->setFrameX(0);
	_olaf.frameCount = 0;
}
void PlayerOlaf::ResetAnimation3()
{
	if (_currentLadderState != _beforeLadderState)
	{
		_olaf.currentFrameX = 0;
		_olaf.image->setFrameX(0);
		_olaf.frameCount = 0;
	}
	_beforeLadderState = _currentLadderState;
}
void PlayerOlaf::ResetAnimation4()
{
	if (_currentPosstate != _beforePosstate)
	{
		_olaf.currentFrameX = 0;
		_olaf.image->setFrameX(0);
		_olaf.frameCount = 0;
	}
	_beforePosstate = _currentPosstate;
}
// 문제점 : 어떻게하면 검사해야하는 부분을 줄일 수 있을까? 지금 조건이 너무 많아서 이대로 가단 유지보수가 엄청 어려워 질수 있음. + 렉도 발생함