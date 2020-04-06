#pragma once
#include "gameNode.h"
#include "player.h"

class playerEric : public player
{
private:
	tagPlayer _eric;

	int _hitCount;				// �¾��� �� ���͹� 
	int _breathCount;			// �� ī���� 
	int _breathFrameCount;	    // �� ������ ī���� 
	int _slideCount;			// �����̵� ī��Ʈ 

	float _jumpStart;		    // ���� ������ 
	float _gravity;				// �߷°�
	float _slidePower;			// �����̵� �Ŀ� 

	bool _ericAttack;		    // ���� ������ 
	bool _ericJump;				// ���� ������
	bool _ericUnable;			// ���� �Ҵ�
	bool _isHit;				// ������ 
	bool _isBreath;             // �� ����� ��
	bool _isSlide;              // �����̵� BOOL
	bool _isSlideOn;			// BOOL-> �����̵��϶� 
	bool _stopControl;			// �̰��� true��� ������ �� ����.
	bool _stop;					// 
	bool _isItem;				// �κ� ����Ѵٸ�  


public:
	playerEric();
	~playerEric();

	virtual HRESULT init(float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void move();
	virtual void key();

	void ericFrameCount();
	void ericJump();
	void ericAttackMove();
	void ericHit();
	void setEricImage();
	//�ȼ��浹ó�� 
	void PixelCollision();			// Y�� �浹 ó�� 
	void PixelRightCollision();		// X�� ������ �浹 ó�� 
	void PixelLeftCollision();		// X�� ����   �浹 ó�� 
	void isJumpPixelCollision();	// ������ �浹 ó�� 

	void ericDie();					// ü���� 0�̸� �״� ����
	tagPlayer getEric()			 { return _eric;	}	 // ERIC ��ȯ 
	RECT getEricRect()			 { return _eric.rc; }    // ERIC RECT ��ȯ
	void setEricX(float x)		 { _eric.x = x; }		 // ERIC X
	void setEricY(float y)		 { _eric.y = y; }        // ERIC Y
	void setHit()			     { _isHit = true; }      // ERIC ���� �� BOOL��
	bool getHit()		         { return _isHit; }
	void setEricHP()			 { if(_eric.hp<3) _eric.hp++; }		 // ERIC HP++
	void setEricHit()			 { _eric.hp--; }		 //		 HP--
	

	void setEricPosState(tagPosState posstate) {  _eric.posState = posstate; }
	void player_Kill()		 { _eric.isDead = true; } // ���� ���� 
	void setEricUnable()	 { _ericUnable = true;	} // ��ġ�� ���� �� ������
	void setEricStop()		 { _stop = true; }		  // STOP 
	void setEricFrame()		 { _eric.currentFrameX = 0;}
	void setEricJump()		 { _ericJump = false; }
	void setEricJumpPower()  { _eric.jumpPower = 17; }
	bool getItem()			 { return _isItem; }
	void setItemKey()		 { _isItem ? _isItem = false : _isItem = true; }

	void setEricFrameSpeed(int speed) {	_eric.frameSpeed = speed; }
	void setEricState(tagState state) { _eric.state = state; }	// ���¸� ������ �ִ� ���� 

	void set_stopKey(bool stop)		  { _stopControl = stop;}
	void setEricDead(bool dead)		  { _eric.isDead = dead; }	// ERIC ���� 
};

