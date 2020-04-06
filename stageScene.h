#pragma once
#include "gameNode.h"
#include "worldMap.h"			// 스테이지에서만 필요하기 때문에 이곳으로 이동
#include "playerManager.h"		// 스테이지에서만 필요하기 때문에 이곳으로 이동
#include "enemyManager.h"		// 스테이지에서만 필요하기 때문에 이곳으로 이동
#include "waterFall.h"



// 플레이어의 타입
enum PlayerType
{
	PT_ERIC,
	PT_BALEOG,
	PT_OLAF
};


enum tagGiveUp
{
	GU_CENTER,
	GU_YES,
	GU_NO
};


// 상태 UI의 정보를 담는 구조체
struct _tagStateUI
{
	POINTFLOAT		pos;						// 중점 
	RECT			rc;							// 렉트 좌표
	image*			image;						// 이미지 저장하는곳
	bool			dead;						// 캐릭터가 죽었는지 확인
};

class stageScene : public gameNode
{
private:
	_tagStateUI		_UI_State[3];				// 캐릭터마다 상태 이미지가 있다. 3칸
	_tagStateUI		_UI_Inventory[3][4];		// 캐릭터마다 인벤토리가 있다. 각각 4칸씩
	_tagStateUI		_UI_Garbage;				// 휴지통 이미지 위치
	_tagStateUI		_UI_HP[3];					// HP 정보
	_tagStateUI		_UI_Ending;					// 엔딩 정보
	RECT			_ERC, _BRC, _ORC;
	bool			_CC;	

	int				_charNum;					// 캐릭터를 가리키는 번호
	int				_banjjack;					// 반짝반짝

	int				_playerAllDeadTimer;		// 플레이어가 모두 죽었을때 일정 시간 뒤에 게임오버 창으로 넘어간다.

	int*			_player_InvenPos;			// 현재 가리키는 인벤 위치
	bool			_player_UsingInven;			// 현재 플레이어는 인벤을 이용하고 있다.

	_tagStateUI		_UI_GiveUpPOS[3];			// 기브업 이미지 좌표
	_tagStateUI		_UI_GiveSelectPos;			// 기브업 셀렉 좌표
	bool			_giveUpStart;				// 기브업을 호출
	bool			_giveUpSelect;				// 기브업 선택
	bool			_gBanZZank;					// 기브업 반짝반짝
	int				_giveCnt;
	bool			_iBanZZack;					// 아이템이 반짝반짝
	int				_iBanCnt;					// 아이템 반짝반짝 카운터

	worldMap* _wm;
	playerManager* _pm;
	EnemyManager* _em;
	waterFall* _wf;

private:
	//폭파 했을때
	int _boomCount;
	int _fadeIn;
	bool _isChange;
public:
	stageScene();
	~stageScene();

	virtual HRESULT init();
	void release();
	void update();
	void render();

	void posSetting();
	void addStageImage();		// 스테이지 UI에 필요한 이미지를 추가하는 함수

	void testStateImage();		// 이미지 테스트용 함수

	void set_PlayerDead();

	void setting_InvenSelectPos();	// 인벤토리가 현재 가리키는 위치를 저장해둔다.
	void setting_GiveUpPos();		// 기브업 메시지 좌표 셋팅

};

