#include "stdafx.h"
#include "introScene.h"

introScene::introScene()
{
	addIntroImage();	// ��Ʈ�ο� ���� �̹����� �����ϴ� �Լ�
}

introScene::~introScene()
{
}

HRESULT introScene::init()
{
	_introScene.scene_Number = 0;					// ���� �� �ѹ�
	_introScene.scene_Image = new image;			// �̹��� ���� �Ҵ�
	_introScene.scene_State = false;				// �̹����� ������ �����ִ� ���� : false, �̹����� ������ ������� ���� : true
	_introScene.fade_In = 0;						// ���Ŀ��� ����� ��
	_introScene.scene_Cnt = 0;						// �̹����� ������ ���������� �̹����� ������� �Ҷ������� �ð�

	// ���� �� �ѹ��� �̿��Ͽ� �̹����� ã�� �ִ´�.
	sprintf_s(_introScene.scene_Str, "Intro%d", _introScene.scene_Number);
	_introScene.scene_Image = IMAGEMANAGER->findImage(_introScene.scene_Str);

	// �� ���ÿ��� ����� ��Ʈ
	_introScene.scene_Select_rc[SS_GAME_START] = RectMakeCenter(WINSIZEX / 2 - 170, WINSIZEY / 2 - 10, 36, 40);
	_introScene.scene_Select_rc[SS_GAME_PASSWORD] = RectMakeCenter(WINSIZEX / 2 - 170, WINSIZEY / 2 + 30, 36, 40);
	_introScene.scene_Select_Image = IMAGEMANAGER->findImage("Intro_Select");
	_introScene.scene_Select_Image->setX(_introScene.scene_Select_rc[SS_GAME_START].left);
	_introScene.scene_Select_Image->setY(_introScene.scene_Select_rc[SS_GAME_START].top);

	// �н����� �ʱ�ȭ
	for (int i = 0; i < 4; ++i)
	{
		_password[i].pass_Num_rc = RectMakeCenter((WINSIZEX / 2 - 100) + (i * 60), WINSIZEY / 2 + 65, 50, 50);
		_password[i].pass_Num_Image = new image;
		_password[i].pass_Num_Image = IMAGEMANAGER->findImage("PassNum");
	}

	// �н����� ������ YANG (30, 32, 21, 15)
	// ó�� �����Ҷ��� �⺻���� STAT�� �����ִ�.
	_password[0].save_Num_Pos = 25;
	_password[1].save_Num_Pos = 26;
	_password[2].save_Num_Pos = 32;
	_password[3].save_Num_Pos = 26;

	// �н����� ����Ʈ ��ġ�� ���� �����̴�.
	_passwordMove = 0;


	// �ӽ÷� ����� �̹��� DC
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
	setting_AlphaBlend();	// ���� ���� ����� ����
	Select_Key();			// ���� Ű ����
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

	// �н����带 �������� ����� �Ѵ�.
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

	// �н����带 Ʋ�������� ��µǴ� �̹���
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


	// �׽�Ʈ�� ��Ʈ
	// Rectangle(getMemDC(), _introScene.scene_Select_rc[SS_GAME_START]);
	// Rectangle(getMemDC(), _introScene.scene_Select_rc[SS_GAME_PASSWORD]);
}

void introScene::addIntroImage()
{
	char keyName[20];								// Ű���� ���� ����
	char imageAddress[30];							// �̹��� ��θ� ���� ����

	for (int i = 0; i < INTROIMAGESIZE; ++i)		// ��Ʈ�� �̹��� ������ŭ �ݺ��Ѵ�.
	{
		sprintf_s(keyName, "Intro%d", i);
		sprintf_s(imageAddress, "./image/UI/Intro/Intro_%d.bmp", i);

		IMAGEMANAGER->addImage(keyName, imageAddress, WINSIZEX, WINSIZEY, false, RGB(0, 0, 0));
	}

	// �н����� ��Ʈ �̹��� 
	IMAGEMANAGER->addFrameImage("PassNum", "./image/UI/password/password_Font.bmp", 1650, 50, 33, 1, false, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("PassImage", "./image/UI/password/password_Image.bmp", 252, 28, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("PassSelect", "./image/UI/password/password_Select.bmp", 55, 60, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("NoPass", "./image/UI/password/password_Nopass.bmp", 500, 37, false, RGB(0, 0, 0));

	// �⺻ ��� �̹���
	IMAGEMANAGER->addImage("Intro_BG", "./image/UI/Intro/Intro_BG.bmp", WINSIZEX, WINSIZEY, false, RGB(0, 0, 0));

	// ���� �̹���
	IMAGEMANAGER->addImage("Intro_Select", "./image/UI/Intro/Select_Icon.bmp", 36, 40, true, RGB(255, 0, 255));
}

void introScene::setting_AlphaBlend()
{
	// ��Ʈ�� �̹��� �ִ� ���� - 1 ������ ���� ó���Ѵ�.
	if (_introScene.scene_Number < INTROIMAGESIZE - 1)
	{
		// _introScene.scene_State�� ���°� false�϶��� �̹����� ������ ���̰� �Ѵ�.
		if (!_introScene.scene_State)
		{
			if (_introScene.scene_State < 255)	_introScene.fade_In += FADEINSPEED;

			// �ִ밪�� 255�� ���ų� �̻��̶�� 
			if (_introScene.fade_In >= 255)
			{
				_introScene.fade_In = 255;	// 255�� �Ѿ�� ��� 255�� �������ش�.

				_introScene.scene_Cnt++;

				// ���� ��� ī��Ʈ�� NEXTSCENECOUNT���� ũ�ų� ���ٸ�
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

		// _introScene.scene_State�� ���°� true�϶��� �̹����� ������ ������� �Ѵ�.
		if (_introScene.scene_State)
		{
			if (_introScene.fade_In > 0)  _introScene.fade_In -= FADEINSPEED;

			// �ּҰ��� 0�� ���ų� ���϶��
			if (_introScene.fade_In <= 0)
			{
				_introScene.fade_In = 0;	// 0���Ϸ� �Ѿ�� ��� 0���� �������ش�.

				// ���� ����� ���� �ʱ�ȭ
				_introScene.scene_State = !_introScene.scene_State;
				_introScene.scene_Number++;

				sprintf_s(_introScene.scene_Str, "Intro%d", _introScene.scene_Number);

				_introScene.scene_Image = IMAGEMANAGER->findImage(_introScene.scene_Str);
			}
		}
	}


	// ���� ��Ʈ�� �̹����� �������� �ִ� ��Ȳ���� Ư��Ű�� ������
	// ���� ��Ʈ�� �̹����� �Ѿ��. 
	// ����Ʈ ȭ�鿡���� ��� ���ϰ� ����ó�� �Ұ�
}

void introScene::Select_Key()
{
	// �� �ѹ��� 4�϶��� �����Ѵ�. (_introScene.input_Pass�� �н����� �Է��� ������ ���� ����)
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

		// ���͸� �Է��Ѵٸ� �ش� ��Ʈ�� ������ �̵��Ѵ�.
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			// _introScene.scene_Change_pos�� ���� false��� ���� ��ŸƮ�� ��ġ�ϰ� �ִ�. (Ű�Է¿��� �������ö� ����ִ°��� �ٲ�� ������)
			if (!_introScene.scene_Change_pos)
			{
				SCENEMANAGER->set_SceneState(SS_STAGE);		// ���¸� �ٲ��ָ鼭 ���� ��ȯ���ش�.
			}

			// ���� true�� ���� ������ �ִٸ�, �н����� ��ġ�� �ִ�.
			if (_introScene.scene_Change_pos)
			{
				// ���� �н����� �Է��� �����ٸ� �ش� ���� true
				_introScene.input_Pass = true;
			}
		}
	}

	// true�� ���� ������ �ִٸ�
	if (_introScene.input_Pass && !_passMiss)
	{
		// Ʋ�� ��й�ȣ�� �Է��ϸ� Ʋ�ȴٴ� �޽����� ��µȴ�.
		// ���� �ð��� �����ų� ���͸� ������ �ٽ� �������� ���ư���.
		// _introScene.input_Pass = false; 


		// �´� ��й�ȣ��� ���������� �̵��� �Ѵ�.
		// SCENEMANAGER->set_SceneState(SS_STAGE);


		// Ű�� �ѹ��� ��������
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_passwordMove--;							// �н����� ��Ʈ�� �̵� �����ش�. 0, 1, 2, 3
			if (_passwordMove < 0) _passwordMove = 3;	// ���� 0 ���϶�� ���� �������� 3�� ������ �ٲ��ش�.
		}

		// Ű�� ������ �־�����
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			_passCnt++;

			if (_passCnt == 10)							// ���͹��� �̿��Ͽ� �ʹ� ������ �Ѿ�°��� �������ش�.
			{
				_passwordMove--;
				if (_passwordMove < 0) _passwordMove = 3;

				_passCnt = 0;
			}
		}

		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			// �н����� ������ �̹������� �ش� ��ȣ�� ��ġ�� �̹����� ����Ҷ� ���

			_password[_passwordMove].save_Num_Pos++;	// �н����带 ����Ű�� ���� ���������ش�.
			if (_password[_passwordMove].save_Num_Pos > 31) _password[_passwordMove].save_Num_Pos = 0;	// �ƽ��� �ʰ������� ����
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

		Input_Key();		// Ű���� �Է��� ������ ó��

		// ���� �н����尡 �´ٸ� ���� ���������� �̵��� �Ѵ�.
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			if (_password[0].save_Num_Pos == 30 && _password[1].save_Num_Pos == 32 &&
				_password[2].save_Num_Pos == 21 && _password[3].save_Num_Pos == 15)
			{
				// ture�� ���ϸ� ���� �ٲٴ� �غ� �Ѵ�.
				_introScene.fade_In = 255;
				_changingScene = true;
			}

			else
			{
				_passMiss = true;	// �н����尡 Ʋ�ȴٸ� true�� ���� ����.
			}
		}
	}

	if (_passMiss)
	{
		// �н����尡 Ʋ������ ���͸� ������ �ٽ� ��Ʈ�� ���� ȭ������ �Ѿ��.
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
	// ����
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

	// ���ĺ�
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
