#include "stdafx.h"
#include "stageScene.h"


stageScene::stageScene()
{
	addStageImage();	// �̹��� �߰� �Լ�
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

	posSetting();	// UI�� ��ǥ�� �����Ѵ�. (ĳ���� ���� + �κ��丮)

	_UI_State[PT_ERIC].image->setFrameX(1);	// �̹����� ���� �̹����� ��ü�Ѵ�.
	_UI_State[PT_BALEOG].image->setFrameX(0);	// �̹����� ���� �̹����� ��ü�Ѵ�.
	_UI_State[PT_OLAF].image->setFrameX(0);	// �̹����� ���� �̹����� ��ü�Ѵ�.

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
	EFFECTMANAGER->update();			//����Ʈ �Ŵ��� ������Ʈ

	// �� ���� true��� ���ӿ������� �� ���ƿ°�
	if (SCENEMANAGER->get_playerDead_Reset())
	{
		_pm->setEricDead();
		_pm->setBaleogDead();
		_pm->setOlafDead();
		SCENEMANAGER->clear_YouDie();
		SCENEMANAGER->playerDead_Reset_END();
	}
	
	// ���� �޽����� ���� ������ �����ִ´�.
	if (!_giveUpStart && !CAMERAMANAGER->get_Camera_Move())
	{
		_wm->update();
		_pm->update();
		_em->update();
		_wf->update();
	
	}
	testStateImage();  // ĳ���� ��ȯ �׽�Ʈ
	setting_InvenSelectPos();

	RECT ttemp;
	_ERC = _pm->getPlayerEric().rc;
	_BRC = _pm->getPlayerBaleog().rc;
	_ORC = _pm->getPlayerOlaf().rc;

	//cout << "======================================================================" << endl;
	//cout << "����" << (_ERC.left + _ERC.right) / 2 << " " << (_ERC.top + _ERC.bottom) / 2 << endl;
	//cout << "���α�" << (_BRC.left + _BRC.right) / 2 << " " << (_BRC.top + _BRC.bottom) / 2 << endl;
	//cout << "�ö���" << (_ORC.left + _ORC.right) / 2 << " " << (_ORC.top + _ORC.bottom) / 2 << endl;
	//cout << "����" << _UI_Ending.rc.left << " " << _UI_Ending.rc.top << endl;
	//cout << "����" << _CC << endl;

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
	// �׽�Ʈ�� ����̴�. ���Ŀ��� �����!
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

	// �������� ���
	_UI_Garbage.image->render(getMemDC(), _UI_Garbage.rc.left, _UI_Garbage.rc.top);

	// �̹��� ���� ���
	for (int i = 0; i < 3; ++i)
	{
		//Rectangle(getMemDC(), _UI_State[i].rc);
		_UI_State[i].image->frameRender(getMemDC(), _UI_State[i].rc.left, _UI_State[i].rc.top,
			_UI_State[i].image->getFrameX(), 0);
	}

	//// �κ��丮 ��Ʈ ���
	//for (int i = 0; i < 3; ++i)
	//{
	//	for (int j = 0; j < 4; ++j)
	//	{
	//		//Rectangle(getMemDC(), _UI_Inventory[i][j].rc);
	//		IMAGEMANAGER->findImage("Select_Image")->render(getMemDC(), _UI_Inventory[i][j].rc.left, _UI_Inventory[i][j].rc.top);
	//	}
	//}

	// �÷��̾��� ������� ���==========================================
	// �÷��̾��� ���� ü���� ������ŭ ����Ѵ�.
	// �÷��̾ ü���� �������� �׸�ŭ �پ���.
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

	// �κ��丮
	for (int i = 0; i < 3; ++i)
	{
		if (_charNum == i && _pm->getItem() && !_pm->getTrade())
		{
			_banjjack++;
			if (_banjjack <= 20)
			{
				// ������ �κ� ��ġ�� ���� �޶�����.
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

	// ������ ���
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
	
	
	// ���� ��� �׽�Ʈ

	//_UI_GiveUpPOS[0].image->render(getMemDC(), _UI_GiveUpPOS[0].rc.left, _UI_GiveUpPOS[0].rc.top);
	//_UI_GiveUpPOS[1].image->render(getMemDC(), _UI_GiveUpPOS[1].rc.left, _UI_GiveUpPOS[1].rc.top);
	//_UI_GiveUpPOS[2].image->render(getMemDC(), _UI_GiveUpPOS[2].rc.left, _UI_GiveUpPOS[2].rc.top);

	if (_giveUpStart)
	{
		_giveCnt++;
		// ���� ���� �̹����� ��� ����� �ǰ� �ִ´�.
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

	EFFECTMANAGER->render();		//����Ʈ �Ŵ��� ����

}

void stageScene::posSetting()
{
	// ���� UI �ʱ�ȭ
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
	_UI_State[PT_ERIC].rc.left += 2;	// �������� ��ġ ����
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

	// ���� �ÿ� �������� ���õǾ� �ִ�
	_UI_State[PT_ERIC].image->setFrameX(1);


	// �κ��丮 UI �ʱ�ȭ (ĳ���͸��� 4���� �ִ�.)
	for (int i = 0; i < 3; ++i)			// ĳ���Ͱ� 3��
	{
		for (int j = 0; j < 4; ++j)		// �κ��丮 4��
		{
			_UI_Inventory[i][j].image = new image;
			if (j < 2)
				_UI_Inventory[i][j].rc = RectMake(238 + (j * 48) + (i * 213), WINSIZEY - 138, 49, 48);			// �κ��丮 ��Ʈ
			else if (j < 4)
				_UI_Inventory[i][j].rc = RectMake(238 + ((j - 2) * 48) + (i * 213), WINSIZEY - 90, 49, 48);		// �κ��丮 ��Ʈ
			_UI_Inventory[i][j].pos.x = (_UI_Inventory[i][j].rc.left + _UI_Inventory[i][j].rc.right) / 2;		// �κ��丮 �߽���ǥ
			_UI_Inventory[i][j].pos.y = (_UI_Inventory[i][j].rc.bottom + _UI_Inventory[i][j].rc.top) / 2;		// �κ��丮 �߽���ǥ
		}
	}


	// �������� UI �ʱ�ȭ
	_UI_Garbage.image = IMAGEMANAGER->findImage("GarbageBox");
	_UI_Garbage.rc = RectMake(764, _UI_State[PT_ERIC].rc.top, _UI_Garbage.image->getWidth(), _UI_Garbage.image->getHeight());
	_UI_Garbage.pos.x = (_UI_Garbage.rc.left + _UI_Garbage.rc.right) / 2;
	_UI_Garbage.pos.y = (_UI_Garbage.rc.top + _UI_Garbage.rc.bottom) / 2;

	// ĳ���� ��ȯ ���� (�� ���ڰ� �ٲ�� �� ĳ���͸� �ٶ󺻴�?)
	_charNum = PT_ERIC;


	// GiveUP �޴��� ���� ����
	// ĳ���Ͱ� ��� �׾��ų�, Esc�� �������� ���� �ɼ��̴�.
	// ��ġ �ʱ�ȭ
	setting_GiveUpPos();

	// ĳ���� ���� ��ġ �ʱ�ȭ
	_UI_HP[PT_ERIC].image = new image;
	_UI_HP[PT_ERIC].image = IMAGEMANAGER->findImage("Life");

	_UI_HP[PT_BALEOG].image = new image;
	_UI_HP[PT_BALEOG].image = IMAGEMANAGER->findImage("Life");

	_UI_HP[PT_OLAF].image = new image;
	_UI_HP[PT_OLAF].image = IMAGEMANAGER->findImage("Life");

	_UI_HP[PT_ERIC].rc = RectMake(_UI_State[PT_ERIC].rc.left , _UI_State[PT_ERIC].rc.bottom+1, 12, 12);
	_UI_HP[PT_BALEOG].rc = RectMake(_UI_State[PT_BALEOG].rc.left , _UI_State[PT_BALEOG].rc.bottom+1, 12, 12);
	_UI_HP[PT_OLAF].rc = RectMake(_UI_State[PT_OLAF].rc.left , _UI_State[PT_OLAF].rc.bottom+1, 12, 12);


	// ���� ���� �ʱ�ȭ
	_UI_Ending.image = new image;
	_UI_Ending.rc = RectMake(CAMERAMANAGER->get_WorldSize_X() - 100, CAMERAMANAGER->get_WorldSize_Y() - 200, 100, 100);
}

void stageScene::addStageImage()
{
	// �������� UI���� ����� �̹����� �����Ѵ�.
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

	// �׽�Ʈ�� ��Ʈ���� �����ٸ�
	if (KEYMANAGER->isOnceKeyDown(VK_CONTROL))
	{
		//_UI_State[_charNum].image->setFrameX(0);
		//_charNum++;
		//if (_charNum > PT_Def) _charNum = PT_Run;
		//_UI_State[_charNum].image->setFrameX(1);

		_UI_State[_charNum].image->setFrameX(0);	// ĳ���Ͱ� ��ü�Ǹ� ������ �ִ� ĳ���ʹ� 0�� �̹����� ����
		_charNum++;									// ���� ĳ���͸� ���� ���ڸ� ���������ش�.
		if (_charNum == 3)	_charNum = 0;			// ���� ĳ������ ������ �Ѿ��
		if (_UI_State[_charNum].dead)
		{
			_charNum++;								// ���� ���� ĳ���Ͱ� �׾��ִٸ�, ���� ĳ���͸� ��ȸ�Ѵ�.
			if (_charNum == 3)	_charNum = 0;			// ���� ĳ������ ������ �Ѿ��

			if (_UI_State[_charNum].dead)
				_charNum++;
			if (_charNum == 3)	_charNum = 0;			// ���� ĳ������ ������ �Ѿ��
		}
		_UI_State[_charNum].image->setFrameX(1);

		// ��Ȳ�� �°� ī�޶� ��ǥ�� �Ѱ�����Ѵ�.
		// ���� ĳ���Ͱ� �ƴ϶�� 0�� �̹����� ������ֵ��� �ٲ��ش�.
		// ĳ���Ͱ� ���õǾ� �ִ� ��Ȳ�̶�� 1�� �̹����� ����Ѵ�.

		// ĳ���Ͱ� ��ȯ�� �ɶ�
		// ī�޶� �̵��� �϶�� true�� ���� ���� �Ѱ��ش�. + �� ĳ������ ��ǥ
		// ī�޶� �Ŵ��������� ���ϴ� ��ġ���� �̵� �� �Ŀ� false�� �ٲ�����
		// �׋����ʹ� ĳ������ ��ǥ �������� ����ٴѴ�.
		// ī�޶� �̵��� ������ �Ұ��� �ٲ�� �׷��� ī�޶� ������ �� �ֵ��� ��ġ�� �������ش�.
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
	// ĳ������ ���� ������ �޾Ƽ� �����Ѵ�.
	// ���� ĳ���Ͱ� �׾��ٸ� ���� �̹����� �׾��ְ� �Ѵ�.
	_UI_State[PT_ERIC].dead = _pm->getPlayerEric().isDead;
	_UI_State[PT_BALEOG].dead = _pm->getPlayerBaleog().isDead;
	_UI_State[PT_OLAF].dead = _pm->getPlayerOlaf().isDead;

	// ���� dead���� true�� ĳ���Ͱ� �׾��ٴ� ��
	if (_UI_State[PT_ERIC].dead)
	{
		_UI_State[PT_ERIC].image->setFrameX(2);		// �̹����� ���� �̹����� ��ü�Ѵ�.
		SCENEMANAGER->set_PlayerLife(PT_ERIC, true);		
	}

	if (_UI_State[PT_BALEOG].dead)
	{
		_UI_State[PT_BALEOG].image->setFrameX(2);	// �̹����� ���� �̹����� ��ü�Ѵ�.
		SCENEMANAGER->set_PlayerLife(PT_BALEOG, true);		
	}

	if (_UI_State[PT_OLAF].dead)
	{
		_UI_State[PT_OLAF].image->setFrameX(2);		// �̹����� ���� �̹����� ��ü�Ѵ�.
		SCENEMANAGER->set_PlayerLife(PT_OLAF, true);		
	}

	// ���� ĳ���Ͱ� �׾��� ��쿡 ���� ĳ���ͷ� �ٲٴ� ���
	if (_UI_State[_charNum].dead)
	{
		while (true)
		{
			// ĳ���Ͱ� ��� ���� ��Ȳ�� ���� ���� ȭ������ �Ѿ��.
			if (_UI_State[PT_ERIC].dead && _UI_State[PT_BALEOG].dead && _UI_State[PT_OLAF].dead)
			{
				_playerAllDeadTimer++;
				// ���͹��� �ְ� �Ѿ�� �׳� �Ѿ�� ���ϰ�
				// ���� ���� ������ �Ѿ��.
				// �ӽ÷� ��Ʈ�� ������ �Ѱ���

				//_wm->init();		// ���� �����ϴϱ� ��� ������ �ʱ�ȭ ���ش�.
				//_pm->init();
				//_em->init();

				// ���͹��� �ְ� GiveUp���� Yes���� ���͸� ������ ���ӿ��������� �Ѿ��.
				// ���� No�� �����ٸ� Ÿ��Ʋ ȭ������ �Ѿ��.
				if (_playerAllDeadTimer >= 100)
				{
					SCENEMANAGER->set_SceneState(SS_GAMEOVER);
				}

				break;
			}

			_charNum++;
			if (_charNum == 3)	_charNum = 0;				// ���� ĳ������ ������ �ʰ��ߴٸ� �ٽ� 0������

			if (_charNum == 3)	_charNum = 0;				// ���� ĳ������ ������ �ʰ��ߴٸ� �ٽ� 0������

			if (!_UI_State[_charNum].dead)
			{
				_UI_State[_charNum].image->setFrameX(1);	// ĳ���Ͱ� ���������ϱ� ĳ���͸� ���ش�.

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
				break;										// ���� ���� ĳ���Ͱ� ���� �ʾҴٸ� ���Ϲ� ������
			}
			//if (_charNum == 3)	_charNum = 0;				// ���� ĳ������ ������ �ʰ��ߴٸ� �ٽ� 0������
		}
	}

	// ���� �÷��̾ ���� ���߿� �ٽ� �ϰ� �;�����
	if (KEYMANAGER->isOnceKeyDown(VK_ESCAPE))
	{
		// ESCŰ�� ������ �״� ���� �� �� �ִ�.
		// _giveUpStart�� true��� ���� �޽����� ��µǰ� �ٸ��͵��� �����.
		_giveUpStart = !_giveUpStart;

		// �ٽ� �õ����� ������ ���� â�� ����ϰ� �����.
		// YES�� ������ ���ӿ��� ȭ������ �̵��Ѵ�.
		// NO�� ������ Ÿ��Ʋ ȭ������ �̵��Ѵ�. 200103 AM 12:27 ������ (�̿�)
		
		//SCENEMANAGER->set_SceneState(SS_GAMEOVER);
	}

	// ���� ESC�� ������ ��쿡�� ���� �޴� ������ ��� �����ϴ�.
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

		// false���̸� YES���� ��Ʈ�� ��ġ�� �Ȱ��ش�.
		if (!_giveUpSelect)
		{
			_UI_GiveSelectPos.pos.x = _UI_GiveUpPOS[GU_YES].pos.x;
			_UI_GiveSelectPos.pos.y = _UI_GiveUpPOS[GU_YES].pos.y;
		}

		// true���̸� NO���� ��Ʈ�� ��ġ�� �Ȱ��ش�.
		if (_giveUpSelect)
		{
			_UI_GiveSelectPos.pos.x = _UI_GiveUpPOS[GU_NO].pos.x;
			_UI_GiveSelectPos.pos.y = _UI_GiveUpPOS[GU_NO].pos.y;
		}

		_UI_GiveSelectPos.rc = RectMake(_UI_GiveSelectPos.pos.x, _UI_GiveSelectPos.pos.y, 10, 10);

		// YES�� NO�� �浹 �Ǿ� ������ ���͸� ������ �ش� ����� ������ �Ѵ�.
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
	// �÷��̾� �Ŵ������� �޾ƿ� �κ� ���� ��ġ ������ �ּҸ� �����Ѵ�.
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
	//// Ʈ���� ���� ��ġ
	//_trySelect.pos.x = _tryPos[T_YES].pos.x;
	//_trySelect.pos.y = _tryPos[T_YES].pos.y;
	//_trySelect.rc = RectMake(_trySelect.pos.x, _trySelect.pos.y, 10, 10);

	// ���� �̹����� �����Ѵ�.
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
	
	//// Ʈ���� ���� ��ġ
	_UI_GiveSelectPos.pos.x = _UI_GiveUpPOS[GU_YES].pos.x;
	_UI_GiveSelectPos.pos.y = _UI_GiveUpPOS[GU_YES].pos.y;
	_UI_GiveSelectPos.rc = RectMake(_UI_GiveSelectPos.pos.x, _UI_GiveSelectPos.pos.y, 10, 10);

}
