#include "stdafx.h"
#include "introScene.h"

introScene::introScene()
{
	addIntroImage();	// 인트로에 쓰일 이미지를 셋팅하는 함수
}

introScene::~introScene()
{
}

HRESULT introScene::init()
{
	_introScene.scene_Number = 0;					// 현재 씬 넘버
	_introScene.scene_Image = new image;			// 이미지 공간 할당
	_introScene.scene_State = false;				// 이미지를 서서히 보여주는 상태 : false, 이미지를 서서히 사라지는 상태 : true
	_introScene.fade_In = 0;						// 알파에서 사용할 값
	_introScene.scene_Cnt = 0;						// 이미지가 완전히 보여졌을때 이미지를 사라지게 할때까지의 시간

	// 현재 씬 넘버를 이용하여 이미지를 찾아 넣는다.
	sprintf_s(_introScene.scene_Str, "Intro%d", _introScene.scene_Number);
	_introScene.scene_Image = IMAGEMANAGER->findImage(_introScene.scene_Str);

	// 씬 선택에서 사용할 렉트
	_introScene.scene_Select_rc[SS_GAME_START] = RectMakeCenter(WINSIZEX / 2 - 170, WINSIZEY / 2 - 10, 36, 40);
	_introScene.scene_Select_rc[SS_GAME_PASSWORD] = RectMakeCenter(WINSIZEX / 2 - 170, WINSIZEY / 2 + 30, 36, 40);
	_introScene.scene_Select_Image = IMAGEMANAGER->findImage("Intro_Select");
	_introScene.scene_Select_Image->setX(_introScene.scene_Select_rc[SS_GAME_START].left);
	_introScene.scene_Select_Image->setY(_introScene.scene_Select_rc[SS_GAME_START].top);

	// 패스워드 초기화
	for (int i = 0; i < 4; ++i)
	{
		_password[i].pass_Num_rc = RectMakeCenter((WINSIZEX / 2 - 100) + (i * 60), WINSIZEY / 2 + 65, 50, 50);
		_password[i].pass_Num_Image = new image;
		_password[i].pass_Num_Image = IMAGEMANAGER->findImage("PassNum");
	}

	// 패스워드 정답은 YANG (30, 32, 21, 15)
	// 처음 시작할때는 기본으로 STAT에 맞춰있다.
	_password[0].save_Num_Pos = 25;
	_password[1].save_Num_Pos = 26;
	_password[2].save_Num_Pos = 32;
	_password[3].save_Num_Pos = 26;

	// 패스워드 셀렉트 위치는 제일 왼쪽이다.
	_passwordMove = 0;


	// 임시로 사용할 이미지 DC
	_sceneImage = new image;
	_sceneImage = IMAGEMANAGER->addImage("sceneTemp", WINSIZEX, WINSIZEY);
	_sceneDC = _sceneImage->getMemDC();

	return S_OK;
}

void introScene::release()
{
}

void introScene::update()
{
	setting_AlphaBlend();	// 알파 블랜에 사용할 연산
	Select_Key();			// 선택 키 설정
}

void introScene::render()
{
	IMAGEMANAGER->findImage("Intro_BG")->render(getMemDC(), 0, 0);

	if (!_changingScene)
	{
		if (_introScene.scene_Number < INTROIMAGESIZE - 1)
		{
			_introScene.scene_Image->alphaRender(getMemDC(), _introScene.fade_In);
		}

		else
		{
			_introScene.scene_Image->render(getMemDC(), 0, 0);

			_introScene.scene_Select_Image->render(getMemDC(),
				_introScene.scene_Select_Image->getX(), _introScene.scene_Select_Image->getY());
		}
	}

	// 패스워드를 눌렀을때 출력을 한다.
	if (_introScene.input_Pass && !_passMiss && !_changingScene)
	{
		IMAGEMANAGER->findImage("Intro3")->render(getMemDC(), 0, 0);
		IMAGEMANAGER->findImage("PassImage")->render(getMemDC(), WINSIZEX / 2 - 135, WINSIZEY / 2);
		for (int i = 0; i < 4; ++i)
		{
			Rectangle(getMemDC(), _password[i].pass_Num_rc);
			_password[i].pass_Num_Image->frameRender(getMemDC(), _password[i].pass_Num_rc.left, _password[i].pass_Num_rc.top,
				_password[i].save_Num_Pos, 0);
		}
		IMAGEMANAGER->findImage("PassSelect")->render(getMemDC(), _password[_passwordMove].pass_Num_rc.left, _password[_passwordMove].pass_Num_rc.top - 5);
	}

	// 패스워드를 틀렸을때만 출력되는 이미지
	if (_passMiss)
	{
		IMAGEMANAGER->findImage("Intro3")->render(getMemDC(), 0, 0);
		IMAGEMANAGER->findImage("NoPass")->render(getMemDC(), WINSIZEX / 2 - 260, WINSIZEY / 2);
	}

	if (_changingScene)
	{
		IMAGEMANAGER->findImage("Intro3")->render(_sceneDC, 0, 0);
		IMAGEMANAGER->findImage("PassImage")->render(_sceneDC, WINSIZEX / 2 - 135, WINSIZEY / 2);
		for (int i = 0; i < 4; ++i)
		{
			
			//Rectangle(getMemDC(), _password[i].pass_Num_rc);
			_password[i].pass_Num_Image->frameRender(_sceneDC, _password[i].pass_Num_rc.left, _password[i].pass_Num_rc.top,
				_password[i].save_Num_Pos, 0);
		}

		IMAGEMANAGER->findImage("PassSelect")->render(_sceneDC, _password[_passwordMove].pass_Num_rc.left, _password[_passwordMove].pass_Num_rc.top - 5);
		IMAGEMANAGER->findImage("sceneTemp")->alphaRender(getMemDC(), 0, 0, _introScene.fade_In);

		if (_introScene.fade_In <= 0)
		{
			_introScene.fade_In = 0;
			_changingScene = false;
			_introScene.input_Pass = false;
			SCENEMANAGER->set_SceneState(SS_STAGE);
		}
	}

	if (KEYMANAGER->isToggleKey('P'))
	{
		Rectangle(getMemDC(), _introScene.scene_Select_rc[SS_GAME_START]);
		Rectangle(getMemDC(), _introScene.scene_Select_rc[SS_GAME_PASSWORD]);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (KEYMANAGER->isToggleKey('O'))
		{
			Rectangle(getMemDC(), _password[i].pass_Num_rc);
		}
	}


	// 테스트용 렉트
	// Rectangle(getMemDC(), _introScene.scene_Select_rc[SS_GAME_START]);
	// Rectangle(getMemDC(), _introScene.scene_Select_rc[SS_GAME_PASSWORD]);
}

void introScene::addIntroImage()
{
	char keyName[20];								// 키값을 담을 변수
	char imageAddress[30];							// 이미지 경로를 담을 변수

	for (int i = 0; i < INTROIMAGESIZE; ++i)		// 인트로 이미지 개수만큼 반복한다.
	{
		sprintf_s(keyName, "Intro%d", i);
		sprintf_s(imageAddress, "./image/UI/Intro/Intro_%d.bmp", i);

		IMAGEMANAGER->addImage(keyName, imageAddress, WINSIZEX, WINSIZEY, false, RGB(0, 0, 0));
	}

	// 패스워드 폰트 이미지 
	IMAGEMANAGER->addFrameImage("PassNum", "./image/UI/password/password_Font.bmp", 1650, 50, 33, 1, false, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("PassImage", "./image/UI/password/password_Image.bmp", 252, 28, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("PassSelect", "./image/UI/password/password_Select.bmp", 55, 60, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("NoPass", "./image/UI/password/password_Nopass.bmp", 500, 37, false, RGB(0, 0, 0));

	// 기본 배경 이미지
	IMAGEMANAGER->addImage("Intro_BG", "./image/UI/Intro/Intro_BG.bmp", WINSIZEX, WINSIZEY, false, RGB(0, 0, 0));

	// 선택 이미지
	IMAGEMANAGER->addImage("Intro_Select", "./image/UI/Intro/Select_Icon.bmp", 36, 40, true, RGB(255, 0, 255));
}

void introScene::setting_AlphaBlend()
{
	// 인트로 이미지 최대 개수 - 1 까지는 알파 처리한다.
	if (_introScene.scene_Number < INTROIMAGESIZE - 1)
	{
		// _introScene.scene_State의 상태가 false일때는 이미지를 서서히 보이게 한다.
		if (!_introScene.scene_State)
		{
			if (_introScene.scene_State < 255)	_introScene.fade_In += FADEINSPEED;

			// 최대값인 255와 같거나 이상이라면 
			if (_introScene.fade_In >= 255)
			{
				_introScene.fade_In = 255;	// 255를 넘어갔을 경우 255로 보정해준다.

				_introScene.scene_Cnt++;

				// 만약 장면 카운트가 NEXTSCENECOUNT보다 크거나 같다면
				if (_introScene.scene_Cnt >= NEXTSCENECOUNT)
				{
					_introScene.scene_Cnt = 0;
					_introScene.scene_State = !_introScene.scene_State;

					if (_introScene.scene_Number == 3)
					{
						_introScene.scene_State = false;
						_introScene.scene_Number++;

						sprintf_s(_introScene.scene_Str, "Intro%d", _introScene.scene_Number);

						_introScene.scene_Image = IMAGEMANAGER->findImage(_introScene.scene_Str);
					}
				}
			}
		}

		// _introScene.scene_State의 상태가 true일때는 이미지를 서서히 사라지게 한다.
		if (_introScene.scene_State)
		{
			if (_introScene.fade_In > 0)  _introScene.fade_In -= FADEINSPEED;

			// 최소값인 0과 같거나 이하라면
			if (_introScene.fade_In <= 0)
			{
				_introScene.fade_In = 0;	// 0이하로 넘어갔을 경우 0으로 보정해준다.

				// 다음 장면을 위한 초기화
				_introScene.scene_State = !_introScene.scene_State;
				_introScene.scene_Number++;

				sprintf_s(_introScene.scene_Str, "Intro%d", _introScene.scene_Number);

				_introScene.scene_Image = IMAGEMANAGER->findImage(_introScene.scene_Str);
			}
		}
	}


	// 만약 인트로 이미지가 보여지고 있는 상황에서 특정키를 누르면
	// 다음 인트로 이미지로 넘어간다. 
	// 셀렉트 화면에서는 사용 못하게 예외처리 할것
}

void introScene::Select_Key()
{
	// 씬 넘버가 4일때만 실행한다. (_introScene.input_Pass는 패스워드 입력을 누르지 않은 상태)
	if (_introScene.scene_Number == 4 && !_introScene.input_Pass && !_passMiss)
	{

		if (KEYMANAGER->isOnceKeyDown(VK_UP) || KEYMANAGER->isOnceKeyDown('W'))
		{
			if (!_introScene.scene_Change_pos)
			{
				_introScene.scene_Select_Image->setX(_introScene.scene_Select_rc[SS_GAME_PASSWORD].left);
				_introScene.scene_Select_Image->setY(_introScene.scene_Select_rc[SS_GAME_PASSWORD].top);
			}

			if (_introScene.scene_Change_pos)
			{
				_introScene.scene_Select_Image->setX(_introScene.scene_Select_rc[SS_GAME_START].left);
				_introScene.scene_Select_Image->setY(_introScene.scene_Select_rc[SS_GAME_START].top);
			}

			_introScene.scene_Change_pos = !_introScene.scene_Change_pos;
		}

		if (KEYMANAGER->isOnceKeyDown(VK_DOWN) || KEYMANAGER->isOnceKeyDown('S'))
		{
			if (!_introScene.scene_Change_pos)
			{
				_introScene.scene_Select_Image->setX(_introScene.scene_Select_rc[SS_GAME_PASSWORD].left);
				_introScene.scene_Select_Image->setY(_introScene.scene_Select_rc[SS_GAME_PASSWORD].top);
			}

			if (_introScene.scene_Change_pos)
			{
				_introScene.scene_Select_Image->setX(_introScene.scene_Select_rc[SS_GAME_START].left);
				_introScene.scene_Select_Image->setY(_introScene.scene_Select_rc[SS_GAME_START].top);
			}

			_introScene.scene_Change_pos = !_introScene.scene_Change_pos;
		}

		// 엔터를 입력한다면 해당 렉트의 씬으로 이동한다.
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			// _introScene.scene_Change_pos의 값이 false라면 게임 스타트에 위치하고 있다. (키입력에서 빠져나올때 들어있는값이 바뀌기 때문에)
			if (!_introScene.scene_Change_pos)
			{
				SCENEMANAGER->set_SceneState(SS_STAGE);		// 상태를 바꿔주면서 씬을 전환해준다.
			}

			// 만약 true의 값을 가지고 있다면, 패스워드 위치에 있다.
			if (_introScene.scene_Change_pos)
			{
				// 만약 패스워드 입력을 누른다면 해당 값을 true
				_introScene.input_Pass = true;
			}
		}
	}

	// true의 값을 가지고 있다면
	if (_introScene.input_Pass && !_passMiss)
	{
		// 틀린 비밀번호를 입력하면 틀렸다는 메시지가 출력된다.
		// 일정 시간이 지나거나 엔터를 누르면 다시 선택으로 돌아간다.
		// _introScene.input_Pass = false; 


		// 맞는 비밀번호라면 스테이지로 이동을 한다.
		// SCENEMANAGER->set_SceneState(SS_STAGE);


		// 키를 한번만 눌렀을때
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_passwordMove--;							// 패스워드 렉트를 이동 시켜준다. 0, 1, 2, 3
			if (_passwordMove < 0) _passwordMove = 3;	// 만약 0 이하라면 제일 오른쪽인 3의 값으로 바꿔준다.
		}

		// 키를 누르고 있었을때
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			_passCnt++;

			if (_passCnt == 10)							// 인터벌을 이용하여 너무 빠르게 넘어가는것을 보정해준다.
			{
				_passwordMove--;
				if (_passwordMove < 0) _passwordMove = 3;

				_passCnt = 0;
			}
		}

		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			// 패스워드 프레임 이미지에서 해당 번호에 위치한 이미지를 출력할때 사용

			_password[_passwordMove].save_Num_Pos++;	// 패스워드를 가리키는 값을 증가시켜준다.
			if (_password[_passwordMove].save_Num_Pos > 31) _password[_passwordMove].save_Num_Pos = 0;	// 맥스를 초과했을때 보정
		}

		if (KEYMANAGER->isStayKeyDown(VK_UP))
		{
			_passCnt++;
			if (_passCnt == 10)
			{
				_password[_passwordMove].save_Num_Pos++;
				if (_password[_passwordMove].save_Num_Pos > 31) _password[_passwordMove].save_Num_Pos = 0;

				_passCnt = 0;
			}
		}

		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_passwordMove++;
			if (_passwordMove > 3) _passwordMove = 0;
		}

		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			_passCnt++;
			if (_passCnt == 10)
			{
				_passwordMove++;
				if (_passwordMove > 3) _passwordMove = 0;

				_passCnt = 0;
			}
		}

		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			_password[_passwordMove].save_Num_Pos--;
			if (_password[_passwordMove].save_Num_Pos < 0) _password[_passwordMove].save_Num_Pos = 31;
		}

		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			_passCnt++;
			if (_passCnt == 10)
			{
				_password[_passwordMove].save_Num_Pos--;
				if (_password[_passwordMove].save_Num_Pos < 0) _password[_passwordMove].save_Num_Pos = 31;

				_passCnt = 0;
			}
		}

		if (KEYMANAGER->isOnceKeyUp(VK_LEFT) || KEYMANAGER->isOnceKeyUp(VK_UP) ||
			KEYMANAGER->isOnceKeyUp(VK_RIGHT) || KEYMANAGER->isOnceKeyUp(VK_DOWN))
		{
			_passCnt = 0;
		}

		Input_Key();		// 키보드 입력을 했을때 처리

		// 만약 패스워드가 맞다면 다음 스테이지로 이동을 한다.
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			if (_password[0].save_Num_Pos == 30 && _password[1].save_Num_Pos == 32 &&
				_password[2].save_Num_Pos == 21 && _password[3].save_Num_Pos == 15)
			{
				// ture로 변하면 씬을 바꾸는 준비를 한다.
				_introScene.fade_In = 255;
				_changingScene = true;
			}

			else
			{
				_passMiss = true;	// 패스워드가 틀렸다면 true의 값이 들어간다.
			}
		}
	}

	if (_passMiss)
	{
		// 패스워드가 틀렸을때 엔터를 누르면 다시 인트로 셀렉 화면으로 넘어간다.
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			_introScene.input_Pass = false;
			_passMiss = false;
		}
	}


	if (_changingScene)
	{
		_introScene.fade_In -= 5;
	}

}

void introScene::Input_Key()
{
	// 숫자
	if (KEYMANAGER->isOnceKeyDown('0'))
	{
		_password[_passwordMove].save_Num_Pos = 0;
	}

	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		_password[_passwordMove].save_Num_Pos = 1;
	}

	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		_password[_passwordMove].save_Num_Pos = 2;
	}

	if (KEYMANAGER->isOnceKeyDown('3'))
	{
		_password[_passwordMove].save_Num_Pos = 3;
	}

	if (KEYMANAGER->isOnceKeyDown('4'))
	{
		_password[_passwordMove].save_Num_Pos = 4;
	}

	if (KEYMANAGER->isOnceKeyDown('5'))
	{
		_password[_passwordMove].save_Num_Pos = 5;
	}

	if (KEYMANAGER->isOnceKeyDown('6'))
	{
		_password[_passwordMove].save_Num_Pos = 6;
	}

	if (KEYMANAGER->isOnceKeyDown('7'))
	{
		_password[_passwordMove].save_Num_Pos = 7;
	}

	if (KEYMANAGER->isOnceKeyDown('8'))
	{
		_password[_passwordMove].save_Num_Pos = 8;
	}

	if (KEYMANAGER->isOnceKeyDown('9'))
	{
		_password[_passwordMove].save_Num_Pos = 9;
	}

	// 알파벳
	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		_password[_passwordMove].save_Num_Pos = 32;
	}

	if (KEYMANAGER->isOnceKeyDown('B'))
	{
		_password[_passwordMove].save_Num_Pos = 11;
	}

	if (KEYMANAGER->isOnceKeyDown('C'))
	{
		_password[_passwordMove].save_Num_Pos = 12;
	}

	if (KEYMANAGER->isOnceKeyDown('D'))
	{
		_password[_passwordMove].save_Num_Pos = 13;
	}

	if (KEYMANAGER->isOnceKeyDown('F'))
	{
		_password[_passwordMove].save_Num_Pos = 14;
	}

	if (KEYMANAGER->isOnceKeyDown('G'))
	{
		_password[_passwordMove].save_Num_Pos = 15;
	}

	if (KEYMANAGER->isOnceKeyDown('H'))
	{
		_password[_passwordMove].save_Num_Pos = 16;
	}

	if (KEYMANAGER->isOnceKeyDown('J'))
	{
		_password[_passwordMove].save_Num_Pos = 17;
	}

	if (KEYMANAGER->isOnceKeyDown('K'))
	{
		_password[_passwordMove].save_Num_Pos = 18;
	}

	if (KEYMANAGER->isOnceKeyDown('L'))
	{
		_password[_passwordMove].save_Num_Pos = 19;
	}

	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		_password[_passwordMove].save_Num_Pos = 20;
	}

	if (KEYMANAGER->isOnceKeyDown('N'))
	{
		_password[_passwordMove].save_Num_Pos = 21;
	}

	if (KEYMANAGER->isOnceKeyDown('P'))
	{
		_password[_passwordMove].save_Num_Pos = 22;
	}

	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		_password[_passwordMove].save_Num_Pos = 23;
	}

	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		_password[_passwordMove].save_Num_Pos = 24;
	}

	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		_password[_passwordMove].save_Num_Pos = 25;
	}

	if (KEYMANAGER->isOnceKeyDown('T'))
	{
		_password[_passwordMove].save_Num_Pos = 26;
	}

	if (KEYMANAGER->isOnceKeyDown('V'))
	{
		_password[_passwordMove].save_Num_Pos = 27;
	}

	if (KEYMANAGER->isOnceKeyDown('W'))
	{
		_password[_passwordMove].save_Num_Pos = 28;
	}

	if (KEYMANAGER->isOnceKeyDown('X'))
	{
		_password[_passwordMove].save_Num_Pos = 29;
	}

	if (KEYMANAGER->isOnceKeyDown('Y'))
	{
		_password[_passwordMove].save_Num_Pos = 30;
	}

	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		_password[_passwordMove].save_Num_Pos = 31;
	}
}
