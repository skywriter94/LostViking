#pragma once
#include"gameNode.h"
//발견한 적의 종류를 확인하는 Enum문

enum class DISCOVERYPlayer
{
	ERIC, BALEOG, OLAF
};

enum class EnemyState
{
	IDLE,		//카메라 밖에 있을때 가만히 있는 상태
	SCOUT,		//카메라 안에 들어와서 플레이어를 찾아 정찰하는 상태
	DISCOVERY,	//플레이어가 정찰 범위 안에 들어와 플레이어를 추적하는 상태
	ATTACK,		//플레이어가 공격 범위 안에 들어와서 공격하는 상태
	DIE			//HP가 0이하가 되어 사망한 상태
};

enum class EnemyLR
{
	LEFT,		//적의 이미지 방향을 결정하기 위한 ENUM 
	RIGHT		//				"
};

enum class EnemyType
{
	MUMMY,		//적의 타입 지정(미라)
	SCORPION,	//		"	   (전갈)
	SNAKE,		//		"	   (뱀)
	PLAYERMUMMY,//      "      (플레이어 미라)
};

class Enemy : public gameNode
{
protected:
	EnemyState _enemyState;					//적의 상태 및 이미지 방향을 결정할 ENUM문
	EnemyLR _enemyLR;						//					"
	EnemyType _enemyType;					//					"
	DISCOVERYPlayer _discoveryPlayer;		//발견한 플레이어가 어떤 플레이어인지 알아내기 위한 변수

	float _x, _y;							//적의 위치를 지정할 변수
	RECT _enemyRect;						//적의 렉트를 지정할 변수	
	RECT _enemyAttackRangeRect;				//적의 공격범위
	RECT _enemyAttackRect;					//적의 공격 렉트
	RECT _enemy_DISCOVERY_Rect;				//적의 탐지 범위
	RECT _cameraRect;						//카메라의 범위 카메라 안에 적이 들어왔는지 판단하기 위함

	image* _image;							//적의 이미지를 저정할 변수
	image* _Attack_image;					//적의 공격 이미지를 저장할 변수
	image* _die_Image;
	int _probeX, _probeY;					//적의 벽,땅 충돌 처리를 위한 탐색 변수
	int _turn_Num;							//적의 방향을 전환하기 위한 변수

	int _frameX, _frameY;					//적의 프레임 변수
	int _frameCount;						//적의 프레임 카운트 변수

	int _enemyHP;							//적의 HP

	bool _turn;								//적의 방향을 바꿔주기 위한 bool변수
	bool _die;								//적의 사망 상태를 반환하기 위한 함수

	bool _isFire;
	bool _wall;

	bool _RIP;
public:
	RECT _ericRect;							//에릭의 렉트
	RECT _olafRect;							//올라프의 렉트
	RECT _baleogRect;						//벨로그의 렉트
	RECT _playerRect;						//플레이의 렉트

	POINT _player;							//플레이어의 위치	

	virtual HRESULT init(EnemyType enemyType, float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void EnemyAction();					//적의 상태의 따른 행동을 지정하는 함수
	virtual void Frame();						//적의 프레임을 관리하는 함수

	virtual void Move();						//적의 움직임을 담당하는 함수
	virtual void Scout();						//적의 탐색을 담당하는 함수
	virtual void Discovery();					//적을 발견해내는 함수
	virtual void UnDiscovery();					
	virtual void Tracking();					//적을 쫓는 함수
	virtual void Attack(EnemyType enemyType);	//적이 사정거리 안에 들어와 공격 상태로 변환하는 함수
	virtual void Attack();						//실질적으로 적을 공격하는 함수
	virtual void AttackDirection();				//공격시 공격 방향을 정하는 함수
	virtual void UnAttack();					//공격상태에서 벗어나게 하는 함수
	virtual void die();
	virtual	void platformColision();			//enemy를 바닥에 붙여놓는 함수
	//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■전근자/설정자■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	virtual EnemyType getEnemyType() { return _enemyType; }		//enemyType 반환
	virtual EnemyState getEnemyState() { return _enemyState; }	//적의 상태를 반환하는 함수(_enemyState)
	virtual RECT getRect() { return _enemyRect; }
	virtual RECT getAttackRect() { return _enemyAttackRect; }
	virtual bool getDie() { if (_die)return true;else return false; }
	virtual bool getisFire() { if (_isFire)return true;else return false; }
	virtual void setisFire(bool isFire) { _isFire = isFire; }
	virtual void Hit() { _enemyHP--; }
	virtual void setTurn() { _turn_Num++; }
	virtual void setTrueWall() { _wall = true; }
	virtual void setFalseWall() { _wall = false; }
	virtual float getX() { return _x; }
	virtual float getY() { return _y; }
	virtual image* getImage() { return _Attack_image; }
	virtual DISCOVERYPlayer getDiscoveryPlayer() { return _discoveryPlayer; }
	virtual void setPlayerRect(RECT eric, RECT baleog, RECT olaf);



	//이미지 추가 함수
	virtual void imageReset()
	{
		IMAGEMANAGER->addFrameImage("Enemy_Mummy", "./image./Enemy/Enemy_Mummy.bmp", 558, 180, 6, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("Enemy_Mummy_Attack", "./image./Enemy/Enemy_Mummy_Attack.bmp", 540, 206, 6, 2, true, RGB(255, 0, 255));

		IMAGEMANAGER->addFrameImage("Enemy_PlayerMummy", "./image./Enemy/Enemy_PlayerMummy.bmp", 558, 180, 6, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("Enemy_PlayerMummy_Attack", "./image./Enemy/Enemy_PlayerMummy_Attack.bmp", 540, 206, 6, 2, true, RGB(255, 0, 255));

		IMAGEMANAGER->addFrameImage("Enemy_Scorpion", "./image./Enemy/Enemy_Scorpion.bmp", 384, 192, 4, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("Enemy_Scorpion_Attack", "./image./Enemy/Enemy_Scorpion_Attack.bmp", 784, 198, 7, 2, true, RGB(255, 0, 255));

		IMAGEMANAGER->addFrameImage("Enemy_Snake", "./image./Enemy/Enemy_Snake.bmp", 88, 20, 2, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("Enemy_Snake_Attack", "./image./Enemy/Enemy_Snake_Attack.bmp", 618, 146, 6, 2, true, RGB(255, 0, 255));

	}
};