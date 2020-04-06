#pragma once
#include "gameNode.h"
#include "player.h"

class playerEric : public player
{
private:
	tagPlayer _eric;

	int _hitCount;				// 맞았을 때 인터벌 
	int _breathCount;			// 숨 카운터 
	int _breathFrameCount;	    // 숨 프레임 카운터 
	int _slideCount;			// 슬라이딩 카운트 

	float _jumpStart;		    // 점프 시작점 
	float _gravity;				// 중력값
	float _slidePower;			// 슬라이딩 파워 

	bool _ericAttack;		    // 에릭 공격중 
	bool _ericJump;				// 에릭 점프중
	bool _ericUnable;			// 전투 불능
	bool _isHit;				// 맞을때 
	bool _isBreath;             // 숨 허덕일 때
	bool _isSlide;              // 슬라이딩 BOOL
	bool _isSlideOn;			// BOOL-> 슬라이딩하라 
	bool _stopControl;			// 이값이 true라면 움직일 수 없다.
	bool _stop;					// 
	bool _isItem;				// 인벤 사용한다면  


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
	//픽셀충돌처리 
	void PixelCollision();			// Y축 충돌 처리 
	void PixelRightCollision();		// X축 오른쪽 충돌 처리 
	void PixelLeftCollision();		// X축 왼쪽   충돌 처리 
	void isJumpPixelCollision();	// 점프시 충돌 처리 

	void ericDie();					// 체력이 0이면 죽는 상태
	tagPlayer getEric()			 { return _eric;	}	 // ERIC 반환 
	RECT getEricRect()			 { return _eric.rc; }    // ERIC RECT 반환
	void setEricX(float x)		 { _eric.x = x; }		 // ERIC X
	void setEricY(float y)		 { _eric.y = y; }        // ERIC Y
	void setHit()			     { _isHit = true; }      // ERIC 맞을 때 BOOL값
	bool getHit()		         { return _isHit; }
	void setEricHP()			 { if(_eric.hp<3) _eric.hp++; }		 // ERIC HP++
	void setEricHit()			 { _eric.hp--; }		 //		 HP--
	

	void setEricPosState(tagPosState posstate) {  _eric.posState = posstate; }
	void player_Kill()		 { _eric.isDead = true; } // 지울 예정 
	void setEricUnable()	 { _ericUnable = true;	} // 박치기 했을 때 정지값
	void setEricStop()		 { _stop = true; }		  // STOP 
	void setEricFrame()		 { _eric.currentFrameX = 0;}
	void setEricJump()		 { _ericJump = false; }
	void setEricJumpPower()  { _eric.jumpPower = 17; }
	bool getItem()			 { return _isItem; }
	void setItemKey()		 { _isItem ? _isItem = false : _isItem = true; }

	void setEricFrameSpeed(int speed) {	_eric.frameSpeed = speed; }
	void setEricState(tagState state) { _eric.state = state; }	// 상태를 정의해 주는 세터 

	void set_stopKey(bool stop)		  { _stopControl = stop;}
	void setEricDead(bool dead)		  { _eric.isDead = dead; }	// ERIC 죽음 
};

