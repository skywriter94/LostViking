#include "stdafx.h"
#include "gameOverScene.h"

gameOverScene::gameOverScene()
{

}

gameOverScene::~gameOverScene()
{
}

HRESULT gameOverScene::init()
{
	setting_Image();		// �ʿ��� �̹����� �����Ѵ�.
	setting_Pos();
	setting_init();

	return S_OK;
}

void gameOverScene::release()
{
}

void gameOverScene::update()
{
	KEYANIMANAGER->update();
	restart_Select();		// ����� ���� ������
	letsGo_Ship();			// �踦 �����̴� �Լ�
}

void gameOverScene::render()
{
	_bg.image->aniRender(getMemDC(), _bg.rc.left, _bg.rc.top, _bg.ani);			// ��׶���
	_ship.image->aniRender(getMemDC(), _ship.rc.left, _ship.rc.top, _ship.ani);	// ��

	if (_ReStartGame.Re_Eric)	// ������ ����ִٸ� false�� ���� ���´�.
	{
		_goPlayer.image->frameRender(getMemDC(), _goPlayerRC[GOP_Eric].left, _goPlayerRC[GOP_Eric].top, 1, 0);
	}

	if (!_ReStartGame.Re_Eric && _PlayerRE[GOP_Eric])		// ������ �׾��ִٸ� ture�� ���� ���´�. ���͸� ������ �������� ����ϸ� �ȵȴ�.
	{
		_goPCnt++;

		// ���� ���͸� �����ٸ� ������ ġ�� �� �ڿ� �� �ڸ��� ������ ��Ƴ���.
		_lightNing[GOP_Eric].image->aniRender(getMemDC(), _goPlayerRC[GOP_Eric].left + 30, -70, _lightNing[GOP_Eric].ani);

		if (_goPCnt == 50)
		{
			// ���͹��� �ְ� �� �Ŀ� ������ ���ڸ��� ��Ÿ����.
			_goPlayer.image->frameRender(getMemDC(), _goPlayerRC[GOP_Eric].left, _goPlayerRC[GOP_Eric].top, 1, 0);
			_ReStartGame.Re_Eric = true;	// ������ ��Ƴ��ٴ� ǥ��
			_PlayerRE[GOP_Eric] = false;
			_goPCnt = 0;
		}
	}

	if (_ReStartGame.Re_Baleog)	// ���αװ� ����ִٸ� false�� ���� ���´�.
	{
		_goPlayer.image->frameRender(getMemDC(), _goPlayerRC[GOP_Baleog].left, _goPlayerRC[GOP_Baleog].top, 2, 0);
	}
	
	if (!_ReStartGame.Re_Baleog && _PlayerRE[GOP_Baleog])	// ���αװ� �׾��ִٸ� true�� ���� ���´�. ���Ͱ� ������ �������� ����ϸ� �ȵȴ�.
	{
		_goPCnt++;

		_lightNing[GOP_Baleog].image->aniRender(getMemDC(), _goPlayerRC[GOP_Baleog].left + 38, -20, _lightNing[GOP_Baleog].ani);

		if (_goPCnt == 50)
		{
			_goPlayer.image->frameRender(getMemDC(), _goPlayerRC[GOP_Baleog].left, _goPlayerRC[GOP_Baleog].top, 2, 0);
			_ReStartGame.Re_Baleog = true;	// ���αװ� ��Ƴ��ٴ� ǥ��
			_PlayerRE[GOP_Baleog] = false;
			_goPCnt = 0;
		}
	}
	
	if (_ReStartGame.Re_Olaf)	// �ö����� ����ִٸ� false�� ���� ���´�.
	{
		_goPlayer.image->frameRender(getMemDC(), _goPlayerRC[GOP_Olaf].left, _goPlayerRC[GOP_Olaf].top, 0, 0);
	}

	if (!_ReStartGame.Re_Olaf && _PlayerRE[GOP_Olaf])	// ���� �׾��ִٸ� ������� �ʴ´�.
	{
		_goPCnt++;

		_lightNing[GOP_Olaf].image->aniRender(getMemDC(), _goPlayerRC[GOP_Olaf].left + 38, -15, _lightNing[GOP_Olaf].ani);

		if (_goPCnt == 50)
		{
			_goPlayer.image->frameRender(getMemDC(), _goPlayerRC[GOP_Olaf].left, _goPlayerRC[GOP_Olaf].top, 0, 0);
			_ReStartGame.Re_Olaf = true;	// �ö����� ��Ƴ��ٴ� ǥ��
			_PlayerRE[GOP_Olaf] = false;
			_goPCnt = 0;
		}
	}


	// �׽�Ʈ Ʈ���� â
	//Rectangle(getMemDC(), _tryPos[0].rc);

	//Rectangle(getMemDC(), _tryPos[1].rc);

	//Rectangle(getMemDC(), _tryPos[2].rc);


	if (_ResetTimer >= 200)
	{
		if (!_SelectMove)	// ����Ʈ�� YES�� ������ YES�� ��¦�����Ѵ�.
		{
			_SelectCnt++;

			_tryPos[T_CENTER].image->render(getMemDC(), _tryPos[T_CENTER].rc.left, _tryPos[T_CENTER].rc.top);
			_tryPos[T_YES].image->frameRender(getMemDC(), _tryPos[T_YES].rc.left, _tryPos[T_YES].rc.top, _changeLight, 0);
			_tryPos[T_NO].image->render(getMemDC(), _tryPos[T_NO].rc.left, _tryPos[2].rc.top);

			if (_SelectCnt >= 10)
			{
				_changeLight = !_changeLight;
				_SelectCnt = 0;
			}
		}

		if (_SelectMove)	// ����Ʈ�� NO�� ������ NO�� ��¦�����Ѵ�.
		{
			_SelectCnt++;

			_tryPos[T_CENTER].image->render(getMemDC(), _tryPos[T_CENTER].rc.left, _tryPos[T_CENTER].rc.top);
			_tryPos[T_YES].image->render(getMemDC(), _tryPos[T_YES].rc.left, _tryPos[1].rc.top);
			_tryPos[T_NO].image->frameRender(getMemDC(), _tryPos[T_NO].rc.left, _tryPos[2].rc.top, _changeLight, 0);

			if (_SelectCnt >= 10)
			{
				_changeLight = !_changeLight;
				_SelectCnt = 0;
			}
		}
	}
}

void gameOverScene::setting_Image()
{
	// ���ӿ������� �ʿ��� �̹����� �����Ѵ�.
	IMAGEMANAGER->addFrameImage("GO_BG", "./image/UI/GameOver/GameOver_BackGround.bmp", 6720, 758, 7, 1, false, RGB(0, 0, 0));
	IMAGEMANAGER->addFrameImage("GO_Player", "./image/UI/GameOver/GameOver_Player.bmp", 600, 300, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("GO_Ship", "./image/UI/GameOver/GameOver_Ship_Image.bmp", 600, 178, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("GO_LightNing", "./image/UI/GameOver/GameOver_LightNing.bmp", 1200, 550, 9, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Try_Center", "./image/UI/GameOver/Try_Center.bmp", 350, 127, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Try_Yes", "./image/UI/GameOver/Try_Yes.bmp", 200, 30, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Try_No", "./image/UI/GameOver/Try_No.bmp", 200, 30, 2, 1, true, RGB(255, 0, 255));

	// �̹��� ������ �־��ش�.
	_bg.image = new image;
	_bg.image = IMAGEMANAGER->findImage("GO_BG");

	_ship.image = new image;
	_ship.image = IMAGEMANAGER->findImage("GO_Ship");

	for (int i = 0; i < 3; ++i)
	{
		_lightNing[i].image = new image;
		_lightNing[i].image = IMAGEMANAGER->findImage("GO_LightNing");
	}

	_goPlayer.image = new image;
	_goPlayer.image = IMAGEMANAGER->findImage("GO_Player");


	int GO_BackGround[] = { 0,1,2,3,4,5,6 };
	// �ִ� Ű��, �̹��� Ű��, �迭, �迭����, �����Ӽӵ�, ��������
	KEYANIMANAGER->addArrayFrameAnimation("Ani_background", "GO_BG", GO_BackGround, 7, 5, true);
	
	int GO_Ship[] = { 0,1,2,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("Ani_ship", "GO_Ship", GO_Ship, 5, 6, true);

	int GO_LightNing[] = { 0,1,2,3,4,5,6,7,8 };
	KEYANIMANAGER->addArrayFrameAnimation("Ani_LightNing", "GO_LightNing", GO_LightNing, 9, 7, false);


	// �ִϸ��̼� ���� �־��ش�.
	_bg.ani = KEYANIMANAGER->findAnimation("Ani_background");
	
	_ship.ani = KEYANIMANAGER->findAnimation("Ani_ship");

	for (int i = 0; i < 3; ++i)
	{
		_lightNing[i].ani = KEYANIMANAGER->findAnimation("Ani_LightNing");
	}


	_bg.ani->start();			// �� �׶��� �ִϸ��̼��� �����̵��� �Ѵ�.
	_ship.ani->start();			// ���� �ִϸ��̼��� �����̵��� �Ѵ�.


	// Ʈ���� �̹����� �ʱ�ȭ �Ѵ�.
	for (int i = 0; i < 3; ++i)
	{
		_tryPos[i].image = new image;
	}

	_tryPos[T_CENTER].image = IMAGEMANAGER->findImage("Try_Center");
	_tryPos[T_YES].image = IMAGEMANAGER->findImage("Try_Yes");
	_tryPos[T_NO].image = IMAGEMANAGER->findImage("Try_No");
}

void gameOverScene::setting_Pos()
{
	_bg.rc = RectMake(0, 0, WINSIZEX, WINSIZEY);							// ��׶��� ��ġ �ʱ�ȭ
	_ship.rc = RectMake(WINSIZEX / 2 - 30, WINSIZEY / 2 - 80, 430, 128);	// ���� ��ġ �ʱ�ȭ

	_goPlayerRC[GOP_Eric] = RectMake(45, WINSIZEY / 2 - 200, 100, 337);		// �÷��̾� ��ġ �ʱ�ȭ
	_goPlayerRC[GOP_Baleog] = RectMake(150, WINSIZEY / 2 - 150, 100, 337);
	_goPlayerRC[GOP_Olaf] = RectMake(300, WINSIZEY / 2 - 140, 100, 337);


	// Ʈ���� �޽����� ��ġ
	_tryPos[T_CENTER].rc = RectMake(WINSIZEX / 2 - 150, WINSIZEY / 2 - 100, 300, 127);

	_tryPos[T_YES].rc = RectMake(WINSIZEX / 2 - 120, WINSIZEY / 2 - 20, _tryPos[T_YES].image->getFrameWidth(), _tryPos[T_YES].image->getFrameHeight());
	_tryPos[T_YES].pos.x = (_tryPos[T_YES].rc.left + _tryPos[T_YES].rc.right) / 2;
	_tryPos[T_YES].pos.y = (_tryPos[T_YES].rc.top + _tryPos[T_YES].rc.bottom) / 2;
	
	_tryPos[T_NO].rc = RectMake(WINSIZEX / 2 + 60, WINSIZEY / 2 - 20, _tryPos[T_NO].image->getFrameWidth(), _tryPos[T_NO].image->getFrameHeight());
	_tryPos[T_NO].pos.x = (_tryPos[T_NO].rc.left + _tryPos[T_NO].rc.right) / 2;
	_tryPos[T_NO].pos.y = (_tryPos[T_NO].rc.top + _tryPos[T_NO].rc.bottom) / 2;

	// Ʈ���� ���� ��ġ
	_trySelect.pos.x = _tryPos[T_YES].pos.x;
	_trySelect.pos.y = _tryPos[T_YES].pos.y;
	_trySelect.rc = RectMake(_trySelect.pos.x, _trySelect.pos.y, 10, 10);

	_ReStartGame.Re_Eric = false;
	_ReStartGame.Re_Baleog = false;
	_ReStartGame.Re_Olaf = false;
}

void gameOverScene::setting_init()
{
	_goPCnt = 0;
	_ResetTimer = 0;
	_changeLight = false;
	_SelectMove = false;
	_SelectCnt = 0;
	_PlayerRE[GOP_Eric] = false;
	_PlayerRE[GOP_Baleog] = false;
	_PlayerRE[GOP_Olaf] = false;
	_ReStartGame.Re_Eric = false;
	_ReStartGame.Re_Baleog = false;
	_ReStartGame.Re_Olaf = false;
}

void gameOverScene::letsGo_Ship()
{
	_ship.timer++;

	_ship.rc.left += 1;			// ��� õõ�� ���������� �̵��Ѵ�.
	_ship.rc.right += 1;

	if (_ship.timer == 15)		// Ÿ�̸��� ���� 15��� �� ���ǹ��� ���´�.
	{
		_ship.rc.top -= 1;		// Y������ ���ݾ� �̵��Ѵ�.
		_ship.rc.top -= 1;
		_ship.timer = 0;
	}

	// �踦 ���� ��ġ�� �ʱ�ȭ
	if (KEYMANAGER->isOnceKeyDown(VK_F9))
	{
		_ship.rc = RectMake(WINSIZEX / 2 - 30, WINSIZEY / 2 - 80, 430, 128);
	}
}

void gameOverScene::restart_Select()
{
	// �÷��̾ ��� ��Ƴ� �ڿ� �޽��� ����� �Ѵ�.
	// ���� �޽����� �����⵵ ���� ���͸� �����ٸ� �ٷ� ���������� �̵��Ѵ�.
	// �޽������� Yes�� ������ ���������� ����.
	// �޽������� No�� ������ Ÿ��Ʋ ����ȭ������ ����.
	//_goPLife = SCENEMANAGER->get_PlayerLife();

	// ĳ���Ͱ� ����ִٸ� ���� ������ �غ� �Ѵ�.
	if (!SCENEMANAGER->get_PlayerLife_Eric())	_ReStartGame.Re_Eric = true;	// Stage������ �� ���� �޾Ƽ� �÷��̾ �ǻ츮��
	if (!SCENEMANAGER->get_PlayerLife_Baleog())	_ReStartGame.Re_Baleog = true;	// �÷��̾ ��Ƴ��� ����Ʈ�� ���� ��� ���� �ʴ´�.
	if (!SCENEMANAGER->get_PlayerLife_Olaf())	_ReStartGame.Re_Olaf = true;	// �÷��̾ ��� ��Ƴ��� init�� �̿��Ͽ� �ʵ带 �ʱ�ȭ�Ѵ�.



	//cout << "�ܺ� �÷��̾� ���� (0 : ����, 1 : ����)" << endl;
	//cout << SCENEMANAGER->get_PlayerLife_Eric() << endl;
	//cout << SCENEMANAGER->get_PlayerLife_Baleog() << endl;
	//cout << SCENEMANAGER->get_PlayerLife_Olaf() << endl;
	//cout << endl;
	//cout << "���� �÷��̾� ���� (0 : ����, 1 : ����)" << endl;
	//cout << _ReStartGame.Re_Eric << endl;
	//cout << _ReStartGame.Re_Baleog << endl;
	//cout << _ReStartGame.Re_Olaf << endl;
	//cout << _PlayerRE[GOP_Eric] << endl;
	//cout << _PlayerRE[GOP_Baleog] << endl;
	//cout << _PlayerRE[GOP_Olaf] << endl;
	//cout << "===========================================================" << endl;


	// ���� �׾������� ���͸� �����ٸ� �츮�� �۾��� �����Ѵ�.
	if (SCENEMANAGER->get_PlayerLife_Eric() && !_ReStartGame.Re_Eric)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN) && !_PlayerRE[GOP_Eric])
		{
			_PlayerRE[GOP_Eric] = true;
			_lightNing[GOP_Eric].ani->start();	// ����Ʈ���� �ִϸ��̼��� �����̵��� �Ѵ�.
		}
	}

	else if (SCENEMANAGER->get_PlayerLife_Baleog() && !_ReStartGame.Re_Baleog)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN) && !_PlayerRE[GOP_Baleog])
		{
			_PlayerRE[GOP_Baleog] = true;
			_lightNing[GOP_Baleog].ani->start();	// ����Ʈ���� �ִϸ��̼��� �����̵��� �Ѵ�.
		}
	}

	else if (SCENEMANAGER->get_PlayerLife_Olaf() && !_ReStartGame.Re_Olaf)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN) && !_PlayerRE[GOP_Olaf])
		{
			_PlayerRE[GOP_Olaf] = true;
			_lightNing[GOP_Olaf].ani->start();	// ����Ʈ���� �ִϸ��̼��� �����̵��� �Ѵ�.
		}
	}

	// ���������� �̵����� Ÿ��Ʋ�� �̵����� �����ϴ� ȭ�� �ʿ�
	// ĳ���Ͱ� ��� ��� �ִٸ� ���� �ð��� ���� �� ���� �̹����� ���´�.
	if (_ReStartGame.Re_Eric && _ReStartGame.Re_Baleog && _ReStartGame.Re_Olaf)
	{
		RECT temp;

		if (!_SelectMove)	// ���� ��Ʈ�� YES ��Ʈ�� �浹 ���¶��
		{
			//_trySelect.Collision = false;

			// ���� ���͸� ������ ���� ȭ�鿡 ���� ���� Ʈ����â YES�� �������� ���������� �̵�
			if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
			{
				SCENEMANAGER->set_SceneState(SS_STAGE);
				init();
				SCENEMANAGER->playerDead_Reset();
			}

		}

		if (_SelectMove)
		{
			//_trySelect.Collision = true;

			// ���� ���͸� ������ ���� ȭ�鿡 ���� ���� Ʈ����â YES�� �������� ���������� �̵�
			if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
			{
				SCENEMANAGER->set_SceneState(SS_INTRO);
				init();
			}
		}

		// ���� Ű
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_SelectMove = !_SelectMove;
		}

		// ������ Ű
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_SelectMove = !_SelectMove;
		}

	

		_ResetTimer++;	// ĳ���Ͱ� ��� ��Ƴ��� ���� �ð� �ڿ� Ʈ���� �޽��� ���
	}
}
