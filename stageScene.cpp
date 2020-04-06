#include "stdafx.h"
#include "stageScene.h"


stageScene::stageScene()
{
	addStageImage();	// 이미지 추가 함수
}


stageScene::~stageScene()
{
}

HRESULT stageScene::init()
{
	_wm = new worldMap;
	_wm->init();

	_pm = new playerManager;
	_pm->init();

	_em = new EnemyManager;
	_em->init();

	_wf = new waterFall;
	_wf->init();

	_pm->setWorldObjectAddressLink(_wm->getWorldObject());
	_wm->getWorldObject()->setPlayerManagerAddressLink(_pm);

	_em->AddressLink(_pm);
	_pm->setMemoryAddressLink(_em);

	posSetting();	// UI의 좌표를 설정한다. (캐릭터 상태 + 인벤토리)

	_UI_State[PT_ERIC].image->setFrameX(1);	// 이미지는 죽은 이미지로 교체한다.
	_UI_State[PT_BALEOG].image->setFrameX(0);	// 이미지는 죽은 이미지로 교체한다.
	_UI_State[PT_OLAF].image->setFrameX(0);	// 이미지는 죽은 이미지로 교체한다.

	_playerAllDeadTimer = 0;
	_giveUpStart = false;
	_boomCount = 0;
	_fadeIn = 200;
	return S_OK;
}

void stageScene::release()
{
}

void stageScene::update()
{
	EFFECTMANAGER->update();			//이펙트 매니저 업데이트

	// 이 값이 true라면 게임오버에서 되 돌아온것
	if (SCENEMANAGER->get_playerDead_Reset())
	{
		_pm->setEricDead();
		_pm->setBaleogDead();
		_pm->setOlafDead();
		SCENEMANAGER->clear_YouDie();
		SCENEMANAGER->playerDead_Reset_END();
	}
	
	// 기브업 메시지를 보고 있을땐 멈춰있는다.
	if (!_giveUpStart && !CAMERAMANAGER->get_Camera_Move())
	{
		_wm->update();
		_pm->update();
		_em->update();
		_wf->update();
	
	}
	testStateImage();  // 캐릭터 전환 테스트
	setting_InvenSelectPos();

	RECT ttemp;
	_ERC = _pm->getPlayerEric().rc;
	_BRC = _pm->getPlayerBaleog().rc;
	_ORC = _pm->getPlayerOlaf().rc;

	//cout << "======================================================================" << endl;
	//cout << "에릭" << (_ERC.left + _ERC.right) / 2 << " " << (_ERC.top + _ERC.bottom) / 2 << endl;
	//cout << "벨로그" << (_BRC.left + _BRC.right) / 2 << " " << (_BRC.top + _BRC.bottom) / 2 << endl;
	//cout << "올라프" << (_ORC.left + _ORC.right) / 2 << " " << (_ORC.top + _ORC.bottom) / 2 << endl;
	//cout << "엔딩" << _UI_Ending.rc.left << " " << _UI_Ending.rc.top << endl;
	//cout << "도착" << _CC << endl;

	if (IntersectRect(&ttemp, &_UI_Ending.rc, &_ERC) &&
		IntersectRect(&ttemp, &_UI_Ending.rc, &_BRC) &&
		IntersectRect(&ttemp, &_UI_Ending.rc, &_ORC))
	{
		SCENEMANAGER->set_SceneState(SS_CLEAR);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		//EFFECTMANAGER->play("Enemy_die", _ptMouse.x, _ptMouse.y);
	}
}

void stageScene::render()
{
	// 테스트용 배경이다. 이후에는 지울것!
	//IMAGEMANAGER->findImage("Test_BG")->render(getMemDC(), 0, 0);

	
	//CAMERAMANAGER->get_WorImage()->render(getMemDC(), 0, 0);
	_wm->render();
	_em->render();
	_pm->render();
	_wf->render();

	
	CAMERAMANAGER->get_WorImage()->render(getMemDC(), 0, 0, CAMERAMANAGER->get_Camera_X(), CAMERAMANAGER->get_Camera_Y(),
		CAMERAMANAGER->get_CameraSizeX(), CAMERAMANAGER->get_CameraSizeY());

	IMAGEMANAGER->findImage("OBJECT")->render(getMemDC(), 0, 0, CAMERAMANAGER->get_Camera_X(), CAMERAMANAGER->get_Camera_Y(),
		CAMERAMANAGER->get_CameraSizeX(), CAMERAMANAGER->get_CameraSizeY());
	if (_wm->getWorldObject()->getIsBoomShow())
	{
		_boomCount++;
		if (_boomCount >= 200)
		{
			_fadeIn++;
			if (_fadeIn >= 207 && !_isChange)
			{
				_fadeIn = 50;
				_fadeIn++;
				_isChange = true;

			}
			if (_fadeIn >= 57 && _isChange)
			{
				_fadeIn = 200;
				_fadeIn++;
				_isChange = false;
			}
			IMAGEMANAGER->findImage("IsBoom")->alphaRender(getMemDC(), 0, 0, _fadeIn);
		}
	}
	IMAGEMANAGER->findImage("UI_Image")->render(getMemDC(), 0, WINSIZEY - (WINSIZEY - 573));

	// 쓰레기통 출력
	_UI_Garbage.image->render(getMemDC(), _UI_Garbage.rc.left, _UI_Garbage.rc.top);

	// 이미지 상태 출력
	for (int i = 0; i < 3; ++i)
	{
		//Rectangle(getMemDC(), _UI_State[i].rc);
		_UI_State[i].image->frameRender(getMemDC(), _UI_State[i].rc.left, _UI_State[i].rc.top,
			_UI_State[i].image->getFrameX(), 0);
	}

	//// 인벤토리 렉트 출력
	//for (int i = 0; i < 3; ++i)
	//{
	//	for (int j = 0; j < 4; ++j)
	//	{
	//		//Rectangle(getMemDC(), _UI_Inventory[i][j].rc);
	//		IMAGEMANAGER->findImage("Select_Image")->render(getMemDC(), _UI_Inventory[i][j].rc.left, _UI_Inventory[i][j].rc.top);
	//	}
	//}

	// 플레이어의 생명력을 출력==========================================
	// 플레이어의 현재 체력의 개수만큼 출력한다.
	// 플레이어가 체력이 떨어지면 그만큼 줄어든다.
	if (!_pm->getPlayerEric().isDead)
	{
		for (int i = 0; i < _pm->getPlayerEric().hp; ++i)
		{
			_UI_HP[PT_ERIC].image->render(getMemDC(), _UI_HP[PT_ERIC].rc.left + (i * 18), _UI_HP[PT_ERIC].rc.top);
		}
	}
	
	if (!_pm->getPlayerBaleog().isDead)
	{
		for (int i = 0; i < _pm->getPlayerBaleog().hp; ++i)
		{
			_UI_HP[PT_BALEOG].image->render(getMemDC(), _UI_HP[PT_BALEOG].rc.left + (i * 18), _UI_HP[PT_BALEOG].rc.top);
		}
	}
	
	if (!_pm->getPlayerOlaf().isDead)
	{
		for (int i = 0; i < _pm->getPlayerOlaf().hp; ++i)
		{
		
			_UI_HP[PT_OLAF].image->render(getMemDC(), _UI_HP[PT_OLAF].rc.left + (i * 18), _UI_HP[PT_OLAF].rc.top);
		}
	}
	
	//==================================================================

	// 인벤토리
	for (int i = 0; i < 3; ++i)
	{
		if (_charNum == i && _pm->getItem() && !_pm->getTrade())
		{
			_banjjack++;
			if (_banjjack <= 20)
			{
				// 가져온 인벤 위치에 따라 달라진다.
				IMAGEMANAGER->findImage("Select_Image")->render(getMemDC(), _UI_Inventory[_charNum][_player_InvenPos[_charNum]].rc.left, _UI_Inventory[_charNum][_player_InvenPos[_charNum]].rc.top);
			}
			if (_banjjack >= 30)
			{
				_banjjack = 0;
			}
		}
		else
		{
			//IMAGEMANAGER->findImage("Select_Image")->render(getMemDC(), _UI_Inventory[i][0].rc.left, _UI_Inventory[i][0].rc.top);
			IMAGEMANAGER->findImage("Select_Image")->render(getMemDC(), _UI_Inventory[i][_player_InvenPos[i]].rc.left, _UI_Inventory[i][_player_InvenPos[i]].rc.top);
		}
	}

	// 아이템 출력
	for (int i = 0;i < _pm->get_vInven().size(); ++i)
	{
		if (0 > i) break;

		if (!_pm->get_vInven()[i].choice)
		{
			_pm->get_vInven()[i].image->render(getMemDC(), _UI_Inventory[_pm->get_vInven()[i].player][_pm->get_vInven()[i].invenNumber].rc.left, _UI_Inventory[_pm->get_vInven()[i].player][_pm->get_vInven()[i].invenNumber].rc.top);
		}
		else
		{
			_iBanCnt++;
			if (_iBanCnt <= 20)
			{
				_pm->get_vInven()[i].image->render(getMemDC(), _UI_Inventory[_pm->get_vInven()[i].player][_pm->get_vInven()[i].invenNumber].rc.left, _UI_Inventory[_pm->get_vInven()[i].player][_pm->get_vInven()[i].invenNumber].rc.top);
			}
			if (_iBanCnt >= 30)
			{
				_iBanCnt = 0;
			}
		}
	}
	
	
	// 기브업 출력 테스트

	//_UI_GiveUpPOS[0].image->render(getMemDC(), _UI_GiveUpPOS[0].rc.left, _UI_GiveUpPOS[0].rc.top);
	//_UI_GiveUpPOS[1].image->render(getMemDC(), _UI_GiveUpPOS[1].rc.left, _UI_GiveUpPOS[1].rc.top);
	//_UI_GiveUpPOS[2].image->render(getMemDC(), _UI_GiveUpPOS[2].rc.left, _UI_GiveUpPOS[2].rc.top);

	if (_giveUpStart)
	{
		_giveCnt++;
		// 기브업 센터 이미지는 계속 출력이 되고 있는다.
		_UI_GiveUpPOS[0].image->render(getMemDC(), _UI_GiveUpPOS[0].rc.left, _UI_GiveUpPOS[0].rc.top);

		if (!_giveUpSelect)	
		{
			_UI_GiveUpPOS[1].image->frameRender(getMemDC(), _UI_GiveUpPOS[1].rc.left, _UI_GiveUpPOS[1].rc.top, _gBanZZank, 0);
			_UI_GiveUpPOS[2].image->render(getMemDC(), _UI_GiveUpPOS[2].rc.left, _UI_GiveUpPOS[2].rc.top);
			if (_giveCnt >= 10)
			{
				_gBanZZank = !_gBanZZank;
				_giveCnt = 0;
			}
		}

		if (_giveUpSelect)	
		{
			_UI_GiveUpPOS[1].image->render(getMemDC(), _UI_GiveUpPOS[1].rc.left, _UI_GiveUpPOS[1].rc.top);
			_UI_GiveUpPOS[2].image->frameRender(getMemDC(), _UI_GiveUpPOS[2].rc.left, _UI_GiveUpPOS[2].rc.top, _gBanZZank, 0);
			if (_giveCnt >= 10)
			{
				_gBanZZank = !_gBanZZank;
				_giveCnt = 0;
			}
		}
		
	}

	EFFECTMANAGER->render();		//이펙트 매니저 렌더

}

void stageScene::posSetting()
{
	// 상태 UI 초기화
	_UI_State[PT_ERIC].image = new image;
	_UI_State[PT_ERIC].image = IMAGEMANAGER->findImage("R_State");

	_UI_State[PT_BALEOG].image = new image;
	_UI_State[PT_BALEOG].image = IMAGEMANAGER->findImage("A_State");

	_UI_State[PT_OLAF].image = new image;
	_UI_State[PT_OLAF].image = IMAGEMANAGER->findImage("S_State");


	_UI_State[PT_ERIC].pos.x = 185;
	_UI_State[PT_ERIC].pos.y = WINSIZEY - 95;

	_UI_State[PT_BALEOG].pos.x = 400;
	_UI_State[PT_BALEOG].pos.y = WINSIZEY - 95;

	_UI_State[PT_OLAF].pos.x = 615;
	_UI_State[PT_OLAF].pos.y = WINSIZEY - 95;

	_UI_State[PT_ERIC].rc = RectMakeCenter(_UI_State[PT_ERIC].pos.x, _UI_State[PT_ERIC].pos.y,
		_UI_State[PT_ERIC].image->getFrameWidth(), _UI_State[PT_ERIC].image->getFrameHeight());
	_UI_State[PT_ERIC].rc.left += 2;	// 삐져나온 위치 조정
	_UI_State[PT_ERIC].rc.right += 2;
	_UI_State[PT_ERIC].rc.top -= 2;
	_UI_State[PT_ERIC].rc.bottom -= 2;

	_UI_State[PT_BALEOG].rc = RectMakeCenter(_UI_State[PT_BALEOG].pos.x, _UI_State[PT_BALEOG].pos.y,
		_UI_State[PT_BALEOG].image->getFrameWidth(), _UI_State[PT_BALEOG].image->getFrameHeight());
	_UI_State[PT_BALEOG].rc.top -= 2;
	_UI_State[PT_BALEOG].rc.bottom -= 2;

	_UI_State[PT_OLAF].rc = RectMakeCenter(_UI_State[PT_OLAF].pos.x, _UI_State[PT_OLAF].pos.y,
		_UI_State[PT_OLAF].image->getFrameWidth(), _UI_State[PT_OLAF].image->getFrameHeight());
	_UI_State[PT_OLAF].rc.top -= 2;
	_UI_State[PT_OLAF].rc.bottom -= 2;

	// 시작 시엔 빨간놈이 선택되어 있다
	_UI_State[PT_ERIC].image->setFrameX(1);


	// 인벤토리 UI 초기화 (캐릭터마다 4개씩 있다.)
	for (int i = 0; i < 3; ++i)			// 캐릭터가 3개
	{
		for (int j = 0; j < 4; ++j)		// 인벤토리 4개
		{
			_UI_Inventory[i][j].image = new image;
			if (j < 2)
				_UI_Inventory[i][j].rc = RectMake(238 + (j * 48) + (i * 213), WINSIZEY - 138, 49, 48);			// 인벤토리 렉트
			else if (j < 4)
				_UI_Inventory[i][j].rc = RectMake(238 + ((j - 2) * 48) + (i * 213), WINSIZEY - 90, 49, 48);		// 인벤토리 렉트
			_UI_Inventory[i][j].pos.x = (_UI_Inventory[i][j].rc.left + _UI_Inventory[i][j].rc.right) / 2;		// 인벤토리 중심좌표
			_UI_Inventory[i][j].pos.y = (_UI_Inventory[i][j].rc.bottom + _UI_Inventory[i][j].rc.top) / 2;		// 인벤토리 중심좌표
		}
	}


	// 쓰레기통 UI 초기화
	_UI_Garbage.image = IMAGEMANAGER->findImage("GarbageBox");
	_UI_Garbage.rc = RectMake(764, _UI_State[PT_ERIC].rc.top, _UI_Garbage.image->getWidth(), _UI_Garbage.image->getHeight());
	_UI_Garbage.pos.x = (_UI_Garbage.rc.left + _UI_Garbage.rc.right) / 2;
	_UI_Garbage.pos.y = (_UI_Garbage.rc.top + _UI_Garbage.rc.bottom) / 2;

	// 캐릭터 전환 변수 (이 숫자가 바뀌면 그 캐릭터를 바라본다?)
	_charNum = PT_ERIC;


	// GiveUP 메뉴에 관한 내용
	// 캐릭터가 모두 죽었거나, Esc를 눌렀을때 들어가는 옵션이다.
	// 위치 초기화
	setting_GiveUpPos();

	// 캐릭터 생명 위치 초기화
	_UI_HP[PT_ERIC].image = new image;
	_UI_HP[PT_ERIC].image = IMAGEMANAGER->findImage("Life");

	_UI_HP[PT_BALEOG].image = new image;
	_UI_HP[PT_BALEOG].image = IMAGEMANAGER->findImage("Life");

	_UI_HP[PT_OLAF].image = new image;
	_UI_HP[PT_OLAF].image = IMAGEMANAGER->findImage("Life");

	_UI_HP[PT_ERIC].rc = RectMake(_UI_State[PT_ERIC].rc.left , _UI_State[PT_ERIC].rc.bottom+1, 12, 12);
	_UI_HP[PT_BALEOG].rc = RectMake(_UI_State[PT_BALEOG].rc.left , _UI_State[PT_BALEOG].rc.bottom+1, 12, 12);
	_UI_HP[PT_OLAF].rc = RectMake(_UI_State[PT_OLAF].rc.left , _UI_State[PT_OLAF].rc.bottom+1, 12, 12);


	// 엔딩 정보 초기화
	_UI_Ending.image = new image;
	_UI_Ending.rc = RectMake(CAMERAMANAGER->get_WorldSize_X() - 100, CAMERAMANAGER->get_WorldSize_Y() - 200, 100, 100);
}

void stageScene::addStageImage()
{
	// 스테이지 UI에서 사용할 이미지를 저장한다.
	IMAGEMANAGER->addImage("Test_BG", "./image/UI/UI_Inventory/Test_BG.bmp", 960, 573, false, RGB(0, 0, 0));
	IMAGEMANAGER->addFrameImage("A_State", "./image/UI/UI_Inventory/A_Player_Image.bmp", 305, 80, 3, 1, false, RGB(0, 0, 0));
	IMAGEMANAGER->addFrameImage("R_State", "./image/UI/UI_Inventory/R_Player_Image.bmp", 305, 80, 3, 1, false, RGB(0, 0, 0));
	IMAGEMANAGER->addFrameImage("S_State", "./image/UI/UI_Inventory/S_Player_Image.bmp", 305, 80, 3, 1, false, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("Life_Image", "./image/UI/UI_Inventory/Life_Point.bmp", 12, 12, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("GarbageBox", "./image/UI/UI_Inventory/Garbage_Box.bmp", 46, 47, false, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("Select_Image", "./image/UI/UI_Inventory/Select_Point.bmp", 50, 50, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("UI_Image", "./image/UI/UI_Inventory/UI_Image.bmp", 960, 185, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Give_Center", "./image/UI/GiveUp/GiveUp_Center.bmp", 350, 153, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Give_Yes", "./image/UI/GiveUp/GiveUp_Yes.bmp", 200, 30, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Give_No", "./image/UI/GiveUp/GiveUp_No.bmp", 200, 30, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Life", "./image/UI/UI_Inventory/Life_Point.bmp", 12, 12, true, RGB(255, 0, 255));
}

void stageScene::testStateImage()
{
	set_PlayerDead();
	CAMERAMANAGER->move_Camera();

	// 테스트용 컨트롤을 누른다면
	if (KEYMANAGER->isOnceKeyDown(VK_CONTROL))
	{
		//_UI_State[_charNum].image->setFrameX(0);
		//_charNum++;
		//if (_charNum > PT_Def) _charNum = PT_Run;
		//_UI_State[_charNum].image->setFrameX(1);

		_UI_State[_charNum].image->setFrameX(0);	// 캐릭터가 교체되면 기존에 있던 캐릭터는 0의 이미지로 수정
		_charNum++;									// 다음 캐릭터를 위해 숫자를 증가시켜준다.
		if (_charNum == 3)	_charNum = 0;			// 만약 캐릭터의 수보다 넘어가면
		if (_UI_State[_charNum].dead)
		{
			_charNum++;								// 만약 다음 캐릭터가 죽어있다면, 다음 캐릭터를 조회한다.
			if (_charNum == 3)	_charNum = 0;			// 만약 캐릭터의 수보다 넘어가면

			if (_UI_State[_charNum].dead)
				_charNum++;
			if (_charNum == 3)	_charNum = 0;			// 만약 캐릭터의 수보다 넘어가면
		}
		_UI_State[_charNum].image->setFrameX(1);

		// 상황에 맞게 카메라 좌표를 넘겨줘야한다.
		// 만약 캐릭터가 아니라면 0의 이미지를 출력해주도록 바꿔준다.
		// 캐릭터가 선택되어 있는 상황이라면 1의 이미지를 출력한다.

		// 캐릭터가 전환이 될때
		// 카메라 이동을 하라는 true의 값을 같이 넘겨준다. + 그 캐릭터의 좌표
		// 카메라 매니저에서는 원하는 위치까지 이동 한 후에 false로 바꿔준후
		// 그떄부터는 캐릭터의 좌표 기준으로 따라다닌다.
		// 카메라 이동이 끝나면 불값이 바뀌고 그러면 카메라를 따라디닐 수 있도록 위치를 전송해준다.
		switch (_charNum)
		{
			case 0:
				CAMERAMANAGER->get_Next_CameraXY(_pm->getPlayerEric().x, _pm->getPlayerEric().y, true);
				break;

			case 1:
				CAMERAMANAGER->get_Next_CameraXY(_pm->getPlayerBaleog().x, _pm->getPlayerBaleog().y, true);
				break;

			case 2:
				CAMERAMANAGER->get_Next_CameraXY(_pm->getPlayerOlaf().x, _pm->getPlayerOlaf().y, true);
				break;
		}
	}

	//CAMERAMANAGER->move_Camera();

	_pm->set_Playing(_charNum);


	if (!CAMERAMANAGER->get_Camera_Move())
	{
	
		switch (_charNum)
		{
			case 0:
				if(!_pm->getPlayerEric().isDead)
				CAMERAMANAGER->set_Camera_XY(_pm->getPlayerEric().x, _pm->getPlayerEric().y);
				break;
	
			case 1:
				if(!_pm->getPlayerBaleog().isDead)
				CAMERAMANAGER->set_Camera_XY(_pm->getPlayerBaleog().x, _pm->getPlayerBaleog().y);
				break;
	
			case 2:
				if(!_pm->getPlayerOlaf().isDead)
				CAMERAMANAGER->set_Camera_XY(_pm->getPlayerOlaf().x, _pm->getPlayerOlaf().y);
				break;
		}
	}

	//CAMERAMANAGER->move_Camera();
	

	//set_PlayerDead();
}

void stageScene::set_PlayerDead()
{
	// 캐릭터의 생존 유무를 받아서 저장한다.
	// 만약 캐릭터가 죽었다면 상태 이미지는 죽어있게 한다.
	_UI_State[PT_ERIC].dead = _pm->getPlayerEric().isDead;
	_UI_State[PT_BALEOG].dead = _pm->getPlayerBaleog().isDead;
	_UI_State[PT_OLAF].dead = _pm->getPlayerOlaf().isDead;

	// 만약 dead값이 true면 캐릭터가 죽었다는 뜻
	if (_UI_State[PT_ERIC].dead)
	{
		_UI_State[PT_ERIC].image->setFrameX(2);		// 이미지는 죽은 이미지로 교체한다.
		SCENEMANAGER->set_PlayerLife(PT_ERIC, true);		
	}

	if (_UI_State[PT_BALEOG].dead)
	{
		_UI_State[PT_BALEOG].image->setFrameX(2);	// 이미지는 죽은 이미지로 교체한다.
		SCENEMANAGER->set_PlayerLife(PT_BALEOG, true);		
	}

	if (_UI_State[PT_OLAF].dead)
	{
		_UI_State[PT_OLAF].image->setFrameX(2);		// 이미지는 죽은 이미지로 교체한다.
		SCENEMANAGER->set_PlayerLife(PT_OLAF, true);		
	}

	// 만약 캐릭터가 죽었을 경우에 다음 캐릭터로 바꾸는 기능
	if (_UI_State[_charNum].dead)
	{
		while (true)
		{
			// 캐릭터가 모두 죽은 상황은 게임 오버 화면으로 넘어간다.
			if (_UI_State[PT_ERIC].dead && _UI_State[PT_BALEOG].dead && _UI_State[PT_OLAF].dead)
			{
				_playerAllDeadTimer++;
				// 인터벌을 주고 넘어갈지 그냥 넘어갈지 정하고
				// 게임 오버 씬으로 넘어간다.
				// 임시로 인트로 씬으로 넘겼음

				//_wm->init();		// 새로 시작하니까 모든 정보를 초기화 해준다.
				//_pm->init();
				//_em->init();

				// 인터벌을 주고 GiveUp에서 Yes에서 엔터를 누르면 게임오버씬으로 넘어간다.
				// 만약 No를 눌렀다면 타이틀 화면으로 넘어간다.
				if (_playerAllDeadTimer >= 100)
				{
					SCENEMANAGER->set_SceneState(SS_GAMEOVER);
				}

				break;
			}

			_charNum++;
			if (_charNum == 3)	_charNum = 0;				// 만약 캐릭터의 개수를 초과했다면 다시 0번으로

			if (_charNum == 3)	_charNum = 0;				// 만약 캐릭터의 개수를 초과했다면 다시 0번으로

			if (!_UI_State[_charNum].dead)
			{
				_UI_State[_charNum].image->setFrameX(1);	// 캐릭터가 살이있으니까 캐릭터를 켜준다.

				switch (_charNum)
				{
				case 0:
					CAMERAMANAGER->get_Next_CameraXY(_pm->getPlayerEric().x, _pm->getPlayerEric().y, true);
					break;

				case 1:
					CAMERAMANAGER->get_Next_CameraXY(_pm->getPlayerBaleog().x, _pm->getPlayerBaleog().y, true);
					break;

				case 2:
					CAMERAMANAGER->get_Next_CameraXY(_pm->getPlayerOlaf().x, _pm->getPlayerOlaf().y, true);
					break;
				}
				break;										// 만약 다음 캐릭터가 죽지 않았다면 와일문 나가기
			}
			//if (_charNum == 3)	_charNum = 0;				// 만약 캐릭터의 개수를 초과했다면 다시 0번으로
		}
	}

	// 만약 플레이어가 게임 도중에 다시 하고 싶어질때
	if (KEYMANAGER->isOnceKeyDown(VK_ESCAPE))
	{
		// ESC키를 눌러서 켰다 껐다 할 수 있다.
		// _giveUpStart가 true라면 기브업 메시지가 출력되고 다른것들이 멈춘다.
		_giveUpStart = !_giveUpStart;

		// 다시 시도할지 안할지 고르는 창을 출력하게 만든다.
		// YES를 누르면 게임오버 화면으로 이동한다.
		// NO를 누르면 타이틀 화면으로 이동한다. 200103 AM 12:27 진행중 (미완)
		
		//SCENEMANAGER->set_SceneState(SS_GAMEOVER);
	}

	// 만약 ESC를 눌렀을 경우에는 기브업 메뉴 선택을 사용 가능하다.
	if (_giveUpStart)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_giveUpSelect = !_giveUpSelect;
		}

		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_giveUpSelect = !_giveUpSelect;
		}

		// false값이면 YES으로 렉트의 위치를 옴겨준다.
		if (!_giveUpSelect)
		{
			_UI_GiveSelectPos.pos.x = _UI_GiveUpPOS[GU_YES].pos.x;
			_UI_GiveSelectPos.pos.y = _UI_GiveUpPOS[GU_YES].pos.y;
		}

		// true값이면 NO으로 렉트의 위치를 옴겨준다.
		if (_giveUpSelect)
		{
			_UI_GiveSelectPos.pos.x = _UI_GiveUpPOS[GU_NO].pos.x;
			_UI_GiveSelectPos.pos.y = _UI_GiveUpPOS[GU_NO].pos.y;
		}

		_UI_GiveSelectPos.rc = RectMake(_UI_GiveSelectPos.pos.x, _UI_GiveSelectPos.pos.y, 10, 10);

		// YES나 NO랑 충돌 되어 있을때 엔터를 누르면 해당 명령을 실행을 한다.
		RECT temp;
		if (IntersectRect(&temp, &_UI_GiveSelectPos.rc, &_UI_GiveUpPOS[GU_YES].rc))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
			{
				_pm->init();
				_wm->init();
				_em->init();
				SCENEMANAGER->set_SceneState(SS_GAMEOVER);
			}
		}

		if (IntersectRect(&temp, &_UI_GiveSelectPos.rc, &_UI_GiveUpPOS[GU_NO].rc))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
			{
				//SCENEMANAGER->set_SceneState(SS_INTRO);
				_giveUpStart = false;
				_giveUpSelect = !_giveUpSelect;
			}
		}
	}

}

void stageScene::setting_InvenSelectPos()
{
	// 플레이어 매니저에서 받아온 인벤 셀렉 위치 정보의 주소를 저장한다.
	_player_InvenPos = _pm->getItemNum();
}

void stageScene::setting_GiveUpPos()
{
	//_tryPos[T_CENTER].rc = RectMake(WINSIZEX / 2 - 150, WINSIZEY / 2 - 100, 300, 127);
	//
	//_tryPos[T_YES].rc = RectMake(WINSIZEX / 2 - 120, WINSIZEY / 2 - 20, _tryPos[T_YES].image->getFrameWidth(), _tryPos[T_YES].image->getFrameHeight());
	//_tryPos[T_YES].pos.x = (_tryPos[T_YES].rc.left + _tryPos[T_YES].rc.right) / 2;
	//_tryPos[T_YES].pos.y = (_tryPos[T_YES].rc.top + _tryPos[T_YES].rc.bottom) / 2;
	//
	//_tryPos[T_NO].rc = RectMake(WINSIZEX / 2 + 60, WINSIZEY / 2 - 20, _tryPos[T_NO].image->getFrameWidth(), _tryPos[T_NO].image->getFrameHeight());
	//_tryPos[T_NO].pos.x = (_tryPos[T_NO].rc.left + _tryPos[T_NO].rc.right) / 2;
	//_tryPos[T_NO].pos.y = (_tryPos[T_NO].rc.top + _tryPos[T_NO].rc.bottom) / 2;
	//
	//// 트라이 셀렉 위치
	//_trySelect.pos.x = _tryPos[T_YES].pos.x;
	//_trySelect.pos.y = _tryPos[T_YES].pos.y;
	//_trySelect.rc = RectMake(_trySelect.pos.x, _trySelect.pos.y, 10, 10);

	// 기브업 이미지를 저장한다.
	_UI_GiveUpPOS[GU_CENTER].image = new image;
	_UI_GiveUpPOS[GU_CENTER].image = IMAGEMANAGER->findImage("Give_Center");

	_UI_GiveUpPOS[GU_YES].image = new image;;
	_UI_GiveUpPOS[GU_YES].image = IMAGEMANAGER->findImage("Give_Yes");

	_UI_GiveUpPOS[GU_NO].image = new image;;
	_UI_GiveUpPOS[GU_NO].image = IMAGEMANAGER->findImage("Give_No");

	_UI_GiveUpPOS[GU_CENTER].rc = RectMake(WINSIZEX / 2 - 150, WINSIZEY / 2 - 100, 300, 127);

	_UI_GiveUpPOS[GU_YES].rc = RectMake(WINSIZEX / 2 - 120, WINSIZEY / 2 - 10, _UI_GiveUpPOS[GU_YES].image->getFrameWidth(), _UI_GiveUpPOS[GU_YES].image->getFrameHeight());
	_UI_GiveUpPOS[GU_YES].pos.x = (_UI_GiveUpPOS[GU_YES].rc.left + _UI_GiveUpPOS[GU_YES].rc.right) / 2;
	_UI_GiveUpPOS[GU_YES].pos.y = (_UI_GiveUpPOS[GU_YES].rc.top + _UI_GiveUpPOS[GU_YES].rc.bottom) / 2;
	
	_UI_GiveUpPOS[GU_NO].rc = RectMake(WINSIZEX / 2 + 60, WINSIZEY / 2 - 10, _UI_GiveUpPOS[GU_NO].image->getFrameWidth(), _UI_GiveUpPOS[GU_NO].image->getFrameHeight());
	_UI_GiveUpPOS[GU_NO].pos.x = (_UI_GiveUpPOS[GU_NO].rc.left + _UI_GiveUpPOS[GU_NO].rc.right) / 2;
	_UI_GiveUpPOS[GU_NO].pos.y = (_UI_GiveUpPOS[GU_NO].rc.top + _UI_GiveUpPOS[GU_NO].rc.bottom) / 2;
	
	//// 트라이 셀렉 위치
	_UI_GiveSelectPos.pos.x = _UI_GiveUpPOS[GU_YES].pos.x;
	_UI_GiveSelectPos.pos.y = _UI_GiveUpPOS[GU_YES].pos.y;
	_UI_GiveSelectPos.rc = RectMake(_UI_GiveSelectPos.pos.x, _UI_GiveSelectPos.pos.y, 10, 10);

}
