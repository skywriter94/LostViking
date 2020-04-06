#pragma once
#include "gameNode.h"
#include "player.h"
#include <vector>
#define PI 3.141592654
#define BALEOG_SPEED 5


enum ARROWSTATE
{
	SHOT,
	FIRE_SHOT
};
struct tagArrow
{
	image* arrowImage;				//ȭ�� �̹��� ����
	RECT rc;						//ȭ�� ��Ʈ
	float x, y;						//ȭ�� ��ǥ
	float angle;
	float radius;
	float speed;
	float fireX, fireY;
	float range;
	bool isFire;

	int count;
	int direction;

	float probeX;
};

class arrow : public gameNode				//ȭ�� Ŭ����
{
private:
	vector<tagArrow> _vArrow;				//ȭ�� ���� ����
	vector<tagArrow>::iterator _viArrow;	//���ͷ�����
	float _range;							//��Ÿ�
	int _arrowMax;							//�ִ� ����
	int _direction;							//����
	ARROWSTATE _arrowState;					//ȭ�� ����

public:
	arrow() {};
	~arrow() {};
	virtual HRESULT init(int arrowMax);
	virtual void release();
	virtual void update();
	virtual void render();

	void fire(float x, float y, float speed, float angle, int direction, float range);
	void blade(float x, float y, float speed, float angle, int direction, float range);


	void removeArrow(int arrNum);
	void arrowMove(bool fire);
	void arrowPixelCollision();


	vector<tagArrow> getVArrow() { return _vArrow; }
	vector<tagArrow>::iterator getViArrow() { return _viArrow; }
	RECT getArrowRect(int num) { return _vArrow[num].rc; }
	//�浹 ���ؼ� ��α�->ȭ��->ȭ�� ��Ʈ�� �Լ� �޾ƿ��°� ��������

};

class playerbaleog : public player	//���α� Ŭ����
{

private:
	tagPlayer _baleog;				//�÷��̾� �±�			
	bool _baleogAttackMotion;		//�Ұ��� ������ Į����2, ������ Į����1�� ���´�.
	bool _pullString;				//Ȱ������ ������ �� �����ϴ� �Ұ�
	bool _notMove;					//������ �� �� �̵��� �����ϰ� �ϴ� �Ұ�			
	bool _isHit;

	float _probeY;
	float _moveSpeed;

	arrow* _ar;						//ȭ�� Ŭ������ �����ͷ� ����Ų��


	bool _stopControl;		// �̰��� true��� ������ �� ����.
	bool _isItem;
	bool _stop;
	bool _baleogUnable;	//�����Ҵ�

	COLORREF getPixel_BC;
	int r_BC;
	int g_BC;
	int b_BC;

	COLORREF getPixel_TC;
	int r_TC;
	int g_TC;
	int b_TC;

	RECT _testRect;

	int _hitCount;

	int _baleogStateTemp;
	int _baleogPosStateTemp;

public:
	playerbaleog();
	~playerbaleog();

	virtual HRESULT init(float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	void key();									//����Ű �Լ�
	void setBaleogState(tagState state) { _baleog.state = state; }						//���¿� �̹��� ã���ִ� �Լ�
	void PixelCollision();						//�ȼ� �浹 �Լ�
	void setBaleogPosState(tagPosState posstate) { _baleog.posState = posstate; }			//ū���� ���¿� �̹��� ã���ִ� �Լ�
	void setBaleogImage();
	void setBaleogPosImage();
	void baleogDie();
	void baleogHit();

	tagPlayer getBaleog() { return _baleog; }

	void hgKey();	// �����̰� ���� ��� �̵�Ű
	void set_stopKey(bool stop) { _stopControl = stop; }
	void player_Kill() { _baleog.isDead = true; }
	void setItemKey() { _isItem ? _isItem = false : _isItem = true; }

	//void PixelCollision();			// Y�� �浹 ó�� 
	void PixelRightCollision();		// X�� ������ �浹 ó�� 
	void PixelLeftCollision();		// X�� ����   �浹 ó�� 

	RECT getBaleogRect() { return _baleog.rc; }
	void setBaleogX(float x) { _baleog.x = x; }
	void setBaleogY(float y) { _baleog.y = y; }
	void setBaleogFrame() { _baleog.currentFrameX = 0; }
	void setBaleogStop() { _stop = true; }
	void setBaleogFrameSpeed(int speed) { _baleog.frameSpeed = speed; }
	void setBaleogUnable() { _baleogUnable = true; }

	void setHit() { _isHit = true; }
	bool getHit() { return _isHit; }

	void setBaleogHit() { _baleog.hp--; }

	arrow* getVArrow() { return _ar; }


	void ResetAnimation1();
	void ResetAnimation2();

	void setBaleogDead(bool dead) { _baleog.isDead = dead; }	// ���� �߰�
	void setBaleogHP() { _baleog.hp++; if (_baleog.hp >= 3) _baleog.hp = 3; }


};



