#pragma once
#include "gameNode.h"

enum tagGOPlayer
{
	GOP_Eric,
	GOP_Baleog,
	GOP_Olaf
};

enum tagTry
{
	T_CENTER,
	T_YES,
	T_NO
};

// 이미지 위치 정보
struct tagImagePos
{
	image*				image;
	animation*			ani;
	RECT				rc;
	POINTFLOAT			pos;
	int					timer;
	bool				Collision;
};

struct tagReStart
{
	bool Re_Eric;
	bool Re_Baleog;
	bool Re_Olaf;
};

class gameOverScene : public gameNode
{
private:
	tagImagePos			_bg;				// 배경
	tagImagePos			_ship;				// 배
	tagImagePos			_lightNing[3];		// 라이트닝
	tagImagePos			_goPlayer;			// 플레이어 정보
	tagImagePos			_tryPos[3];			// 트라이 메시지 위치
	tagImagePos			_trySelect;			// 트라이 선택 

	RECT				_goPlayerRC[3];
	bool*				_goPLife;			// 플레이어가 죽었는지 살았는지를 담아둔다.
	int					_goPCnt;			// 번개가 다 치면 플레이어가 살아난다.
	tagReStart			_ReStartGame;
	bool				_PlayerRE[3];		// 엔터를 누르면 true의 값으로 변하고 그 플레이어를 살린다.
	int					_ResetTimer;
	bool				_changeLight;
	bool				_SelectMove;
	int					_SelectCnt;

public:
	gameOverScene();
	~gameOverScene();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setting_Image();		// 필요한 이미지를 세팅한다.
	void setting_Pos();			// 위치 세팅을 한다.
	void setting_init();		// 변수 초기화
	void letsGo_Ship();			// 배는 이동한다.
	void restart_Select();		// 새로 시작할지 연산

};


