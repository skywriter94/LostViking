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
	setting_Image();		// 필요한 이미지를 세팅한다.
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
	restart_Select();		// 재시작 할지 안할지
	letsGo_Ship();			// 배를 움직이는 함수
}

void gameOverScene::render()
{
	_bg.image->aniRender(getMemDC(), _bg.rc.left, _bg.rc.top, _bg.ani);			// 백그라운드
	_ship.image->aniRender(getMemDC(), _ship.rc.left, _ship.rc.top, _ship.ani);	// 배

	if (_ReStartGame.Re_Eric)	// 에릭이 살아있다면 false의 값을 갖는다.
	{
		_goPlayer.image->frameRender(getMemDC(), _goPlayerRC[GOP_Eric].left, _goPlayerRC[GOP_Eric].top, 1, 0);
	}

	if (!_ReStartGame.Re_Eric && _PlayerRE[GOP_Eric])		// 에릭이 죽어있다면 ture의 값을 갖는다. 엔터를 누르기 전까지는 출력하면 안된다.
	{
		_goPCnt++;

		// 만약 엔터를 누른다면 번개가 치고 난 뒤에 그 자리에 에릭이 살아난다.
		_lightNing[GOP_Eric].image->aniRender(getMemDC(), _goPlayerRC[GOP_Eric].left + 30, -70, _lightNing[GOP_Eric].ani);

		if (_goPCnt == 50)
		{
			// 인터벌을 주고 그 후에 에릭이 그자리에 나타난다.
			_goPlayer.image->frameRender(getMemDC(), _goPlayerRC[GOP_Eric].left, _goPlayerRC[GOP_Eric].top, 1, 0);
			_ReStartGame.Re_Eric = true;	// 에릭이 살아났다는 표시
			_PlayerRE[GOP_Eric] = false;
			_goPCnt = 0;
		}
	}

	if (_ReStartGame.Re_Baleog)	// 벨로그가 살아있다면 false의 값을 갖는다.
	{
		_goPlayer.image->frameRender(getMemDC(), _goPlayerRC[GOP_Baleog].left, _goPlayerRC[GOP_Baleog].top, 2, 0);
	}
	
	if (!_ReStartGame.Re_Baleog && _PlayerRE[GOP_Baleog])	// 벨로그가 죽어있다면 true의 값을 갖는다. 엔터가 누르기 전까지는 출력하면 안된다.
	{
		_goPCnt++;

		_lightNing[GOP_Baleog].image->aniRender(getMemDC(), _goPlayerRC[GOP_Baleog].left + 38, -20, _lightNing[GOP_Baleog].ani);

		if (_goPCnt == 50)
		{
			_goPlayer.image->frameRender(getMemDC(), _goPlayerRC[GOP_Baleog].left, _goPlayerRC[GOP_Baleog].top, 2, 0);
			_ReStartGame.Re_Baleog = true;	// 벨로그가 살아났다는 표시
			_PlayerRE[GOP_Baleog] = false;
			_goPCnt = 0;
		}
	}
	
	if (_ReStartGame.Re_Olaf)	// 올라프가 살아있다면 false의 값을 갖는다.
	{
		_goPlayer.image->frameRender(getMemDC(), _goPlayerRC[GOP_Olaf].left, _goPlayerRC[GOP_Olaf].top, 0, 0);
	}

	if (!_ReStartGame.Re_Olaf && _PlayerRE[GOP_Olaf])	// 만약 죽어있다면 출력하지 않는다.
	{
		_goPCnt++;

		_lightNing[GOP_Olaf].image->aniRender(getMemDC(), _goPlayerRC[GOP_Olaf].left + 38, -15, _lightNing[GOP_Olaf].ani);

		if (_goPCnt == 50)
		{
			_goPlayer.image->frameRender(getMemDC(), _goPlayerRC[GOP_Olaf].left, _goPlayerRC[GOP_Olaf].top, 0, 0);
			_ReStartGame.Re_Olaf = true;	// 올라프가 살아났다는 표시
			_PlayerRE[GOP_Olaf] = false;
			_goPCnt = 0;
		}
	}


	// 테스트 트라이 창
	//Rectangle(getMemDC(), _tryPos[0].rc);

	//Rectangle(getMemDC(), _tryPos[1].rc);

	//Rectangle(getMemDC(), _tryPos[2].rc);


	if (_ResetTimer >= 200)
	{
		if (!_SelectMove)	// 셀렉트가 YES에 있을때 YES가 반짝여야한다.
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

		if (_SelectMove)	// 셀렉트가 NO에 있을때 NO가 반짝여야한다.
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
	// 게임오버씬에 필요한 이미지를 저장한다.
	IMAGEMANAGER->addFrameImage("GO_BG", "./image/UI/GameOver/GameOver_BackGround.bmp", 6720, 758, 7, 1, false, RGB(0, 0, 0));
	IMAGEMANAGER->addFrameImage("GO_Player", "./image/UI/GameOver/GameOver_Player.bmp", 600, 300, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("GO_Ship", "./image/UI/GameOver/GameOver_Ship_Image.bmp", 600, 178, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("GO_LightNing", "./image/UI/GameOver/GameOver_LightNing.bmp", 1200, 550, 9, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Try_Center", "./image/UI/GameOver/Try_Center.bmp", 350, 127, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Try_Yes", "./image/UI/GameOver/Try_Yes.bmp", 200, 30, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Try_No", "./image/UI/GameOver/Try_No.bmp", 200, 30, 2, 1, true, RGB(255, 0, 255));

	// 이미지 사진을 넣어준다.
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
	// 애니 키값, 이미지 키값, 배열, 배열길이, 프레임속도, 루프여부
	KEYANIMANAGER->addArrayFrameAnimation("Ani_background", "GO_BG", GO_BackGround, 7, 5, true);
	
	int GO_Ship[] = { 0,1,2,3,4 };
	KEYANIMANAGER->addArrayFrameAnimation("Ani_ship", "GO_Ship", GO_Ship, 5, 6, true);

	int GO_LightNing[] = { 0,1,2,3,4,5,6,7,8 };
	KEYANIMANAGER->addArrayFrameAnimation("Ani_LightNing", "GO_LightNing", GO_LightNing, 9, 7, false);


	// 애니메이션 값을 넣어준다.
	_bg.ani = KEYANIMANAGER->findAnimation("Ani_background");
	
	_ship.ani = KEYANIMANAGER->findAnimation("Ani_ship");

	for (int i = 0; i < 3; ++i)
	{
		_lightNing[i].ani = KEYANIMANAGER->findAnimation("Ani_LightNing");
	}


	_bg.ani->start();			// 백 그라운드 애니메이션을 움직이도록 한다.
	_ship.ani->start();			// 배의 애니메이션을 움직이도록 한다.


	// 트라이 이미지를 초기화 한다.
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
	_bg.rc = RectMake(0, 0, WINSIZEX, WINSIZEY);							// 백그라운드 위치 초기화
	_ship.rc = RectMake(WINSIZEX / 2 - 30, WINSIZEY / 2 - 80, 430, 128);	// 배의 위치 초기화

	_goPlayerRC[GOP_Eric] = RectMake(45, WINSIZEY / 2 - 200, 100, 337);		// 플레이어 위치 초기화
	_goPlayerRC[GOP_Baleog] = RectMake(150, WINSIZEY / 2 - 150, 100, 337);
	_goPlayerRC[GOP_Olaf] = RectMake(300, WINSIZEY / 2 - 140, 100, 337);


	// 트라이 메시지의 위치
	_tryPos[T_CENTER].rc = RectMake(WINSIZEX / 2 - 150, WINSIZEY / 2 - 100, 300, 127);

	_tryPos[T_YES].rc = RectMake(WINSIZEX / 2 - 120, WINSIZEY / 2 - 20, _tryPos[T_YES].image->getFrameWidth(), _tryPos[T_YES].image->getFrameHeight());
	_tryPos[T_YES].pos.x = (_tryPos[T_YES].rc.left + _tryPos[T_YES].rc.right) / 2;
	_tryPos[T_YES].pos.y = (_tryPos[T_YES].rc.top + _tryPos[T_YES].rc.bottom) / 2;
	
	_tryPos[T_NO].rc = RectMake(WINSIZEX / 2 + 60, WINSIZEY / 2 - 20, _tryPos[T_NO].image->getFrameWidth(), _tryPos[T_NO].image->getFrameHeight());
	_tryPos[T_NO].pos.x = (_tryPos[T_NO].rc.left + _tryPos[T_NO].rc.right) / 2;
	_tryPos[T_NO].pos.y = (_tryPos[T_NO].rc.top + _tryPos[T_NO].rc.bottom) / 2;

	// 트라이 셀렉 위치
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

	_ship.rc.left += 1;			// 배는 천천히 오른쪽으로 이동한다.
	_ship.rc.right += 1;

	if (_ship.timer == 15)		// 타이머의 값이 15라면 이 조건문에 들어온다.
	{
		_ship.rc.top -= 1;		// Y축으로 조금씩 이동한다.
		_ship.rc.top -= 1;
		_ship.timer = 0;
	}

	// 배를 시작 위치로 초기화
	if (KEYMANAGER->isOnceKeyDown(VK_F9))
	{
		_ship.rc = RectMake(WINSIZEX / 2 - 30, WINSIZEY / 2 - 80, 430, 128);
	}
}

void gameOverScene::restart_Select()
{
	// 플레이어가 모두 살아난 뒤에 메시지 출력을 한다.
	// 만약 메시지가 나오기도 전에 엔터를 누른다면 바로 스테이지로 이동한다.
	// 메시지에서 Yes를 누르면 스테이지로 간다.
	// 메시지에서 No를 누르면 타이틀 선택화면으로 간다.
	//_goPLife = SCENEMANAGER->get_PlayerLife();

	// 캐릭터가 살아있다면 새로 시작할 준비를 한다.
	if (!SCENEMANAGER->get_PlayerLife_Eric())	_ReStartGame.Re_Eric = true;	// Stage씬에서 이 값을 받아서 플레이어를 되살리고
	if (!SCENEMANAGER->get_PlayerLife_Baleog())	_ReStartGame.Re_Baleog = true;	// 플레이어가 살아나면 라이트닝 씬이 출력 되지 않는다.
	if (!SCENEMANAGER->get_PlayerLife_Olaf())	_ReStartGame.Re_Olaf = true;	// 플레이어가 모두 살아나면 init을 이용하여 필드를 초기화한다.



	//cout << "외부 플레이어 상태 (0 : 살음, 1 : 죽음)" << endl;
	//cout << SCENEMANAGER->get_PlayerLife_Eric() << endl;
	//cout << SCENEMANAGER->get_PlayerLife_Baleog() << endl;
	//cout << SCENEMANAGER->get_PlayerLife_Olaf() << endl;
	//cout << endl;
	//cout << "내부 플레이어 상태 (0 : 죽음, 1 : 살음)" << endl;
	//cout << _ReStartGame.Re_Eric << endl;
	//cout << _ReStartGame.Re_Baleog << endl;
	//cout << _ReStartGame.Re_Olaf << endl;
	//cout << _PlayerRE[GOP_Eric] << endl;
	//cout << _PlayerRE[GOP_Baleog] << endl;
	//cout << _PlayerRE[GOP_Olaf] << endl;
	//cout << "===========================================================" << endl;


	// 만약 죽어있을때 엔터를 눌렀다면 살리는 작업을 시작한다.
	if (SCENEMANAGER->get_PlayerLife_Eric() && !_ReStartGame.Re_Eric)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN) && !_PlayerRE[GOP_Eric])
		{
			_PlayerRE[GOP_Eric] = true;
			_lightNing[GOP_Eric].ani->start();	// 라이트닝의 애니메이션을 움직이도록 한다.
		}
	}

	else if (SCENEMANAGER->get_PlayerLife_Baleog() && !_ReStartGame.Re_Baleog)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN) && !_PlayerRE[GOP_Baleog])
		{
			_PlayerRE[GOP_Baleog] = true;
			_lightNing[GOP_Baleog].ani->start();	// 라이트닝의 애니메이션을 움직이도록 한다.
		}
	}

	else if (SCENEMANAGER->get_PlayerLife_Olaf() && !_ReStartGame.Re_Olaf)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN) && !_PlayerRE[GOP_Olaf])
		{
			_PlayerRE[GOP_Olaf] = true;
			_lightNing[GOP_Olaf].ani->start();	// 라이트닝의 애니메이션을 움직이도록 한다.
		}
	}

	// 스테이지로 이동할지 타이틀로 이동할지 선택하는 화면 필요
	// 캐릭터가 모두 살아 있다면 일정 시간이 지난 뒤 선택 이미지가 나온다.
	if (_ReStartGame.Re_Eric && _ReStartGame.Re_Baleog && _ReStartGame.Re_Olaf)
	{
		RECT temp;

		if (!_SelectMove)	// 셀렉 렉트와 YES 렉트가 충돌 상태라면
		{
			//_trySelect.Collision = false;

			// 만약 엔터를 누르면 아직 화면에 뜨지 않은 트라이창 YES가 눌러지고 스테이지로 이동
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

			// 만약 엔터를 누르면 아직 화면에 뜨지 않은 트라이창 YES가 눌러지고 스테이지로 이동
			if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
			{
				SCENEMANAGER->set_SceneState(SS_INTRO);
				init();
			}
		}

		// 왼쪽 키
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_SelectMove = !_SelectMove;
		}

		// 오른쪽 키
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_SelectMove = !_SelectMove;
		}

	

		_ResetTimer++;	// 캐릭터가 모두 살아나도 일정 시간 뒤에 트라이 메시지 출력
	}
}
