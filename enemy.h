#pragma once
#include"gameNode.h"
//�߰��� ���� ������ Ȯ���ϴ� Enum��

enum class DISCOVERYPlayer
{
	ERIC, BALEOG, OLAF
};

enum class EnemyState
{
	IDLE,		//ī�޶� �ۿ� ������ ������ �ִ� ����
	SCOUT,		//ī�޶� �ȿ� ���ͼ� �÷��̾ ã�� �����ϴ� ����
	DISCOVERY,	//�÷��̾ ���� ���� �ȿ� ���� �÷��̾ �����ϴ� ����
	ATTACK,		//�÷��̾ ���� ���� �ȿ� ���ͼ� �����ϴ� ����
	DIE			//HP�� 0���ϰ� �Ǿ� ����� ����
};

enum class EnemyLR
{
	LEFT,		//���� �̹��� ������ �����ϱ� ���� ENUM 
	RIGHT		//				"
};

enum class EnemyType
{
	MUMMY,		//���� Ÿ�� ����(�̶�)
	SCORPION,	//		"	   (����)
	SNAKE,		//		"	   (��)
	PLAYERMUMMY,//      "      (�÷��̾� �̶�)
};

class Enemy : public gameNode
{
protected:
	EnemyState _enemyState;					//���� ���� �� �̹��� ������ ������ ENUM��
	EnemyLR _enemyLR;						//					"
	EnemyType _enemyType;					//					"
	DISCOVERYPlayer _discoveryPlayer;		//�߰��� �÷��̾ � �÷��̾����� �˾Ƴ��� ���� ����

	float _x, _y;							//���� ��ġ�� ������ ����
	RECT _enemyRect;						//���� ��Ʈ�� ������ ����	
	RECT _enemyAttackRangeRect;				//���� ���ݹ���
	RECT _enemyAttackRect;					//���� ���� ��Ʈ
	RECT _enemy_DISCOVERY_Rect;				//���� Ž�� ����
	RECT _cameraRect;						//ī�޶��� ���� ī�޶� �ȿ� ���� ���Դ��� �Ǵ��ϱ� ����

	image* _image;							//���� �̹����� ������ ����
	image* _Attack_image;					//���� ���� �̹����� ������ ����
	image* _die_Image;
	int _probeX, _probeY;					//���� ��,�� �浹 ó���� ���� Ž�� ����
	int _turn_Num;							//���� ������ ��ȯ�ϱ� ���� ����

	int _frameX, _frameY;					//���� ������ ����
	int _frameCount;						//���� ������ ī��Ʈ ����

	int _enemyHP;							//���� HP

	bool _turn;								//���� ������ �ٲ��ֱ� ���� bool����
	bool _die;								//���� ��� ���¸� ��ȯ�ϱ� ���� �Լ�

	bool _isFire;
	bool _wall;

	bool _RIP;
public:
	RECT _ericRect;							//������ ��Ʈ
	RECT _olafRect;							//�ö����� ��Ʈ
	RECT _baleogRect;						//���α��� ��Ʈ
	RECT _playerRect;						//�÷����� ��Ʈ

	POINT _player;							//�÷��̾��� ��ġ	

	virtual HRESULT init(EnemyType enemyType, float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void EnemyAction();					//���� ������ ���� �ൿ�� �����ϴ� �Լ�
	virtual void Frame();						//���� �������� �����ϴ� �Լ�

	virtual void Move();						//���� �������� ����ϴ� �Լ�
	virtual void Scout();						//���� Ž���� ����ϴ� �Լ�
	virtual void Discovery();					//���� �߰��س��� �Լ�
	virtual void UnDiscovery();					
	virtual void Tracking();					//���� �Ѵ� �Լ�
	virtual void Attack(EnemyType enemyType);	//���� �����Ÿ� �ȿ� ���� ���� ���·� ��ȯ�ϴ� �Լ�
	virtual void Attack();						//���������� ���� �����ϴ� �Լ�
	virtual void AttackDirection();				//���ݽ� ���� ������ ���ϴ� �Լ�
	virtual void UnAttack();					//���ݻ��¿��� ����� �ϴ� �Լ�
	virtual void die();
	virtual	void platformColision();			//enemy�� �ٴڿ� �ٿ����� �Լ�
	//����������������������������������������/�����ڡ������������������������������������������������
	virtual EnemyType getEnemyType() { return _enemyType; }		//enemyType ��ȯ
	virtual EnemyState getEnemyState() { return _enemyState; }	//���� ���¸� ��ȯ�ϴ� �Լ�(_enemyState)
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



	//�̹��� �߰� �Լ�
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