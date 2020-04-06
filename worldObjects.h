#pragma once
#include "gameNode.h"
//오브젝트와 플레이어와 충돌을 위한 playerManager 전방 선언
class playerManager;
enum tagTypeItem
{
	ITEM_BOMB,
	ITEM_BOOM,
	ITEM_TOMATO,
	ITEM_REDKEY,
	ITEM_REDLOCKER,
	ITEM_BLUEKEY,
	ITEM_BLUELOCKER,
};
enum tagTypeTrap
{
	TRAP_POISION,
	TRAP_NIDDLE,
	TRAP_WALL,
	TRAP_RED_UNBREAKABLE_WALL,
	TRAP_BLUE_UNBREAKABLE_WALL,
	TRAP_BORAD,
	TRAP_EXIT,
};
struct tagObjects
{
	tagTypeItem item;
	tagTypeTrap trap;
	RECT rc;				//오브젝트(함정/맵 장식 등)
	image* image;
	bool isCollision;		//함정과 플레이어가 만났는지 아닌지
	bool isUse;				//아이템을 사용했다면
	float x, y;				//오브젝트 좌표
	//프레임 x,y 값
	int frameX;
	int frameY;
	// 페이드 인
	int fadeIn;
};
class worldObjects : public gameNode
{
private:
	playerManager* _pm;
private:
	//■■■■■■■■■■■함정 벡터■■■■■■■■■■■■■■■■■■■■■■
	vector<tagObjects>				 _vTrap;   // 가시 , 독, 발판 
	vector<tagObjects>::iterator    _viTrap;
	//■■■■■■■■■■■아이템 벡터■■■■■■■■■■■■■■■■■■■■■■
	vector<tagObjects>				  _vItem;
	vector<tagObjects>::iterator	 _viItem;
private:
	//■■■■■■■■■■■가시 함정■■■■■■■■■■■■■■■■■■■■■■
	tagObjects _niddle[3];
private:
	//■■■■■■■■■■■독 함정■■■■■■■■■■■■■■■■■■■■■■
	tagObjects _poision[4];
	bool _isntColliToNiddle;
private:
	//■■■■■■■■■■■벽 및 발판■■■■■■■■■■■■■■■■■■■■■■
	tagObjects _flyingBoard; // 발판 
	bool _isUpDown;
	tagObjects _breakableWallOne;
	tagObjects _breakableWallTwo;
	tagObjects _unbreakableWallOne;
	tagObjects _unbreakableWallTwo;
private:
	//■■■■■■■■■■■폭포■■■■■■■■■■■■■■■■■■■■■■
	tagObjects _waterFall[3];
private:
	//■■■■■■■■■■■아이템■■■■■■■■■■■■■■■■■■■■■■
	tagObjects _Items[9];
	tagObjects _isBoomEffects;
	bool _isChange;
private:
	//■■■■■■■■■■■탈출■■■■■■■■■■■■■■■■■■■■■■
	tagObjects _EXIT;
private:
	int _frameCount;			//프레임 카운터
	int _boomCount;				//폭탄사용후 폭탄이 터지는카운터
	bool _isBoomShow;
public:
	worldObjects();
	~worldObjects();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();

	vector<tagObjects> get_vTrap() { return _vTrap; }
	vector<tagObjects>::iterator get_viTrap() { return _viTrap; }

	vector<tagObjects> get_vItem() { return _vItem; }
	vector<tagObjects>::iterator get_viItem() { return _viItem; }

	void setTrapCollision(int arrNum){_vTrap[arrNum].isCollision = true;}
	void setItemCollision(int arrNum) { _vItem[arrNum].isCollision = true; }

	virtual void frameWork();
	virtual void move();
	bool getUpDown() { return _isUpDown; }
	void MakeBoom(float x, float y);
	bool getIsBoomShow() { return _isBoomShow; }
	void setIsBoomShow() { _isBoomShow = true; }
	void setPlayerManagerAddressLink(playerManager* pm) { _pm = pm; }

	RECT getBombRect() { return _Items[8].rc; }
	int getBombFrameCount() { return _Items[8].frameX; }

};

