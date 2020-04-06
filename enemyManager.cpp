#include "stdafx.h"
#include "EnemyManager.h"
#include"playerManager.h"
#include "enemy.h"


EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

HRESULT EnemyManager::init()
{
	EFFECTMANAGER->addEffect("Enemy_die", "./image./Enemy/Enemy_Die.bmp", 891, 73, 81, 73, 1.0f, 0.1f, 50);
	IMAGEMANAGER->addFrameImage("Enemy_Die", "./image./Enemy/Enemy_Die.bmp", 891, 73, 11, 1, true, RGB(255, 0, 255));

	_enemyBullet = new Enemy_Bullet;
	_enemyBullet->init();
	_worldObjects = new worldObjects;
	_worldObjects->init();

	_enemyEffect = new effect;
	_enemyEffect->init(IMAGEMANAGER->findImage("Enemy_Die"), 81, 73, 1.0f, 0.25f,true);

	EnemyCreate();
	return S_OK;
}

void EnemyManager::release()
{
}

void EnemyManager::update()
{
	for (_viEnemy = _vEnemy.begin();_viEnemy != _vEnemy.end();++_viEnemy)
	{
		(*_viEnemy)->update();
		(*_viEnemy)->setPlayerRect(_playerManager->getPlayerEric().rc, _playerManager->getPlayerBaleog().rc, _playerManager->getPlayerOlaf().rc);
		//EFFECTMANAGER->play("Enemy_die", (*_viEnemy)->getX(), (*_viEnemy)->getY());
	}
	_enemyBullet->update();
	_ericRect = _playerManager->getPlayerEric().rc;
	_baleogRect = _playerManager->getPlayerBaleog().rc;
	_olafRect = _playerManager->getPlayerOlaf().rc;

	if (_playerMirra)
	{
		_playerMirraCount++;
		if (_playerMirraCount > 100)
		{
			_playerMirra = false;
			_playerMirraCount = 0;
		}
	}
	EnemyRemove();
	bulletFire();
	Collision();
	
	_enemyEffect->update();

	for (_viEffect = _vEffect.begin(); _viEffect != _vEffect.end(); ++_viEffect)
	{
		(*_viEffect)->update();
	}

}

void EnemyManager::render()
{
	for (_viEnemy = _vEnemy.begin();_viEnemy != _vEnemy.end();++_viEnemy)
	{
		(*_viEnemy)->render();
	}
	_enemyBullet->render();
	_enemyEffect->render(CAMERAMANAGER->getWorDC());

	for (_viEffect = _vEffect.begin(); _viEffect != _vEffect.end(); ++_viEffect)
	{
		(*_viEffect)->render(CAMERAMANAGER->getWorDC());
	}
}

void EnemyManager::EnemyCreate()
{
	Enemy* Mummy_1;
	Mummy_1 = new Enemy_Mummy;
	Mummy_1->init(EnemyType::MUMMY, 83, 750);
	_vEnemy.push_back(Mummy_1);

	Enemy* Mummy_2;
	Mummy_2 = new Enemy_Mummy;
	Mummy_2->init(EnemyType::MUMMY, 2380, 270);
	_vEnemy.push_back(Mummy_2);

	Enemy* Mummy_3;
	Mummy_3 = new Enemy_Mummy;
	Mummy_3->init(EnemyType::MUMMY, 3180, 185);
	_vEnemy.push_back(Mummy_3);

	Enemy* Mummy_4;
	Mummy_4 = new Enemy_Mummy;
	Mummy_4->init(EnemyType::MUMMY, 3710, 955);
	_vEnemy.push_back(Mummy_4);


	Enemy* Mummy_5;
	Mummy_5 = new Enemy_Mummy;
	Mummy_5->init(EnemyType::MUMMY, 3470, 1300);
	_vEnemy.push_back(Mummy_5);

	//Enemy* Scorpion_1;
	//Scorpion_1 = new Enemy_Scorpion;
	//Scorpion_1->init(EnemyType::SCORPION, 1440, 375);
	//_vEnemy.push_back(Scorpion_1);

	//Enemy* Scorpion_2;
	//Scorpion_2 = new Enemy_Scorpion;
	//Scorpion_2->init(EnemyType::SCORPION, 2063, 950);
	//_vEnemy.push_back(Scorpion_2);

	//Enemy* Scorpion_3;
	//Scorpion_3 = new Enemy_Scorpion;
	//Scorpion_3->init(EnemyType::SCORPION, 3246, 1245);
	//_vEnemy.push_back(Scorpion_3);

	Enemy* Snake_1;
	Snake_1 = new Enemy_Snake;
	Snake_1->init(EnemyType::SNAKE, 1100, 380);
	_vEnemy.push_back(Snake_1);

	Enemy* Snake_2;
	Snake_2 = new Enemy_Snake;
	Snake_2->init(EnemyType::SNAKE, 2515, 615);
	_vEnemy.push_back(Snake_2);

	Enemy* Snake_3;
	Snake_3 = new Enemy_Snake;
	Snake_3->init(EnemyType::SNAKE, 2751, 615);
	_vEnemy.push_back(Snake_3);
}

void EnemyManager::EnemyCreate(float x, float y)
{
	Enemy* PlayerMummy;
	PlayerMummy = new Enemy_PlayerMummy;
	PlayerMummy->init(EnemyType::PLAYERMUMMY, x, y);
	_vEnemy.push_back(PlayerMummy);
}

void EnemyManager::EnemyRemove()
{
	for (_viEnemy = _vEnemy.begin();_viEnemy != _vEnemy.end();)
	{
		if ((*_viEnemy)->getDie())
		{
			//EFFECTMANAGER->play("die", (*_viEnemy)->getRect().left, (*_viEnemy)->getRect().top);
			//EFFECTMANAGER->play("Enemy_Die", (*_viEnemy)->getX(), (*_viEnemy)->getY());
			createEffect((*_viEnemy)->getX(), (*_viEnemy)->getY() + 15);
			_vEnemy.erase(_viEnemy);
			break;
		}
		else
		{
			++_viEnemy;
		}
	}
}


void EnemyManager::Collision()
{
	RECT temp;
	for (_viEnemy = _vEnemy.begin();_viEnemy != _vEnemy.end();++_viEnemy)
	{
		for (int i = 0;i < _worldObjects->get_vTrap().size();++i)
		{
			if (IntersectRect(&temp, &(*_viEnemy)->getRect(), &_worldObjects->get_vTrap()[i].rc))
			{
				//(*_viEnemy)->setTurn();
				(*_viEnemy)->setTrueWall();
				break;
			}
			else
			{
				(*_viEnemy)->setFalseWall();
			}
		}
		// 에릭 미라 생성 
		if (IntersectRect(&temp, &(*_viEnemy)->getAttackRect(), &_ericRect))
		{
			if (_playerManager->getPlayerEric().state != STATE_MIRRA)
			{
				_playerManager->getEric()->setEricState(STATE_MIRRA);
			    _playerManager->getEric()->setEricFrame();
				_playerManager->getEric()->setEricStop();
			}
			if (!_playerMirra && _playerManager->getPlayerEric().currentFrameX > _playerManager->getPlayerEric().image->getMaxFrameX()-1)
			{
				EnemyCreate(_playerManager->getPlayerEric().x + 30, _playerManager->getPlayerEric().y + 45);
				_playerMirra = true;
			}
			break;
		}
		//발레오그 미라 생성
		if (IntersectRect(&temp, &(*_viEnemy)->getAttackRect(), &_baleogRect))
		{
			//발레오그 상대정의
			//
			if (_playerManager->getPlayerBaleog().state != STATE_MIRRA)
			{
				_playerManager->getbaleog()->setBaleogState(STATE_MIRRA);
				_playerManager->getbaleog()->setBaleogFrame();
				_playerManager->getbaleog()->setBaleogStop();
			}
			if (!_playerMirra)
			{
				EnemyCreate(_playerManager->getPlayerBaleog().x + 30, _playerManager->getPlayerBaleog().y);
				_playerMirra = true;
			}
			break;
		}

		//올라프 미라 생성
		if (IntersectRect(&temp, &(*_viEnemy)->getAttackRect(), &_olafRect))
		{
			//올라프 상태정의
			//
			if (_playerManager->getOlaf()->getOlafHP() != 0)
			{
				if (_playerManager->getOlaf()->GetOlafShieldState())
				{
					if (_playerManager->getPlayerOlaf().state != STATE_MIRRA)
					{
						_playerManager->getOlaf()->Set_OlafState(STATE_MIRRA);
					}
					if (!_playerMirra)
					{
						EnemyCreate(_playerManager->getOlaf()->getOlafX() + 30, _playerManager->getOlaf()->getOlafY());
						_playerMirra = true;
					}
				}
				else
				{ //0이 오른쪽
					if (_playerManager->getOlaf()->GetOlafDir()) // 왼쪽
					{
						if (  (_playerManager->getOlaf()->GetOlafRC().right + _playerManager->getOlaf()->GetOlafRC().left) / 2 < 
							((*_viEnemy)->getAttackRect().right + (*_viEnemy)->getAttackRect().left) / 2)
						{
							if (_playerManager->getPlayerOlaf().state != STATE_MIRRA)
							{
								_playerManager->getOlaf()->Set_OlafState(STATE_MIRRA);
							}
							if (!_playerMirra)
							{
								//EnemyCreate(_playerManager->getOlaf()->getOlafX() + 30, _playerManager->getOlaf()->getOlafY());
								_playerMirra = true;
							}
						}
					}
					else // 오른쪽
					{
						if ((_playerManager->getOlaf()->GetOlafRC().right + _playerManager->getOlaf()->GetOlafRC().left) / 2 >
							((*_viEnemy)->getAttackRect().right + (*_viEnemy)->getAttackRect().left) / 2)
						{
							if (_playerManager->getPlayerOlaf().state != STATE_MIRRA)
							{
								_playerManager->getOlaf()->Set_OlafState(STATE_MIRRA);
							}
							if (!_playerMirra)
							{
								//EnemyCreate(_playerManager->getOlaf()->getOlafX() + 30, _playerManager->getOlaf()->getOlafY());
								_playerMirra = true;
							}
						}
					}
				}
			}
			break;
		}


		//총알 충돌부분========================================================================================================================
		for (int i = 0; i < _enemyBullet->getVBullet().size(); i++)
		{
			if (IntersectRect(&temp, &_enemyBullet->getVBullet()[i].rect, &_ericRect))
			{
				if (!_enemyBullet->getVBullet()[i].isFire)continue;
				_enemyBullet->removeBullet(i);
				if (!_playerManager->getEric()->getHit())
				{
					_playerManager->getEric()->setHit();
					_playerManager->getEric()->setEricHit();
				}
				break;
			}

			if ((IntersectRect(&temp, &_enemyBullet->getVBullet()[i].rect, &_baleogRect)))
			{
				if (!_enemyBullet->getVBullet()[i].isFire)continue;
				_enemyBullet->removeBullet(i);

				//_playerManager->getbaleog()->setBaleogHit();

				if (!_playerManager->getbaleog()->getHit())
				{
					_playerManager->getbaleog()->setHit();
					_playerManager->getbaleog()->setBaleogHit();
				}
				/*_playerManager->getbaleog()->setBaleogHit();*/
				break;
			}

			if ((IntersectRect(&temp, &_enemyBullet->getVBullet()[i].rect, &_olafRect)))
			{
				if (!_enemyBullet->getVBullet()[i].isFire)continue;
				_enemyBullet->removeBullet(i);

				if (_playerManager->getOlaf()->getOlafHP() != 0)
				{
					if (_playerManager->getOlaf()->GetOlafShieldState())
					{
						_playerManager->getOlaf()->Set_OlafState(STATE_HIT);
						_playerManager->getOlaf()->OlafHit();
					}
					else
					{
						if (_playerManager->getOlaf()->GetOlafDir()) // 방패방향 왼쪽인 경우
						{
							if ((_playerManager->getOlaf()->GetOlafRC().right + _playerManager->getOlaf()->GetOlafRC().left) / 2 < _enemyBullet->getVBullet()[i].x)
							{
								_playerManager->getOlaf()->Set_OlafState(STATE_HIT);
								_playerManager->getOlaf()->setOlafHit();
								_playerManager->getOlaf()->OlafHit();
							}
						}
						else
						{
							if ((_playerManager->getOlaf()->GetOlafRC().right + _playerManager->getOlaf()->GetOlafRC().left) / 2 > _enemyBullet->getVBullet()[i].x)
							{
								_playerManager->getOlaf()->Set_OlafState(STATE_HIT);
								_playerManager->getOlaf()->setOlafHit();
								_playerManager->getOlaf()->OlafHit();
							}
						}
					}
				}
			}
		}
	}
}

void EnemyManager::bulletFire()
{

	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{

		if ((*_viEnemy)->getisFire())
		{
			
			if(DISCOVERYPlayer::BALEOG==(*_viEnemy)->getDiscoveryPlayer())_enemyBullet->bulletFire((*_viEnemy)->getX(), (*_viEnemy)->getY() - (*_viEnemy)->getImage()->getFrameHeight() / 2, getAngle((*_viEnemy)->getX(), (*_viEnemy)->getY() - (*_viEnemy)->getImage()->getFrameHeight() / 2, _playerManager->getPlayerBaleog().x, _playerManager->getPlayerBaleog().y+50));
			if(DISCOVERYPlayer::ERIC== (*_viEnemy)->getDiscoveryPlayer())_enemyBullet->bulletFire((*_viEnemy)->getX(), (*_viEnemy)->getY() - (*_viEnemy)->getImage()->getFrameHeight() / 2, getAngle((*_viEnemy)->getX(), (*_viEnemy)->getY() - (*_viEnemy)->getImage()->getFrameHeight() / 2, _playerManager->getPlayerEric().x, _playerManager->getPlayerEric().y+50));
			if (DISCOVERYPlayer::OLAF == (*_viEnemy)->getDiscoveryPlayer())_enemyBullet->bulletFire((*_viEnemy)->getX(), (*_viEnemy)->getY() - (*_viEnemy)->getImage()->getFrameHeight() / 2, getAngle((*_viEnemy)->getX(), (*_viEnemy)->getY() - (*_viEnemy)->getImage()->getFrameHeight() / 2, _playerManager->getPlayerOlaf().x, _playerManager->getPlayerOlaf().y+50));
			
			(*_viEnemy)->setisFire(false);
			break;
		}
	}
}

void EnemyManager::createEffect(float x, float y)
{
	effect* Enemyeffect;
	Enemyeffect = new effect;
	Enemyeffect->init(IMAGEMANAGER->findImage("Enemy_Die"), 81, 73, 1.0f, 0.25f, true, x, y);
	_vEffect.push_back(Enemyeffect);

}
