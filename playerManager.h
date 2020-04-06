#pragma once
#include "gameNode.h"
#include "playerEric.h"
#include "playerbaleog.h"
#include "PlayerOlaf.h"
#include "worldObjects.h"
#include <list>

class EnemyManager;
class worldObjects;
// 플레이어의 타입
enum PLAYER_TYPE
{
	PT_Eric,
	PT_Baleog,
	PT_Olaf
};

struct tagInven
{
	image*		   image;	 // ITEM IMAGE
	tagTypeItem typeItem;	 // ITEM TYPE
	int			  player;	 // 0 = ERIC, 1 = BALEOG, 2 = OLAF 
	int		 invenNumber;	 // 인벤토리 위치 : 0~3, (4 = 쓰레기통)
	bool		  choice;    // 선택한건지 안한건지를 판단하는 변수 
};
struct arrowBreakTag
{
	image* breakImage;
	int frameX;
	int timeCount;
};
class playerManager : public gameNode
{
private:
	playerEric* _eric;
	playerbaleog* _baleog;
	PlayerOlaf* _olaf;
	arrow* _arrow;

	worldObjects* _wo;
	EnemyManager* _em;

	vector<tagInven>		    _vInven;		// 인벤토리 백터 
	vector<tagInven>::iterator _viInven;		// 인벤토리 이터레이터 

	vector<arrowBreakTag*> _vArrowBreakTag;

	//인벤토리 관련 변수  // 0 = ERIC, 1 = BALEOG, 2 = OLAF
	int _direction[3];  // player, invenNumber 
	int _itemCount[3];	// player가 가지고 있는 아이템의 갯수 
	int	_playing;		// player 
	int invenNum;		// 플레이어의 인벤이 몇 번째인지를 가리키는 수 

	bool _trade;		// 거래 중인지 아닌지 판단

public:
	playerManager() {}
	~playerManager() {}

	HRESULT init();
	void update();
	void release();
	void render();

	//인벤토리 관련 
	vector<tagInven>		   get_vInven()		{ return _vInven; }
	vector<tagInven>::iterator get_viInven()	{ return _viInven;}
	int*  getItemNum()						 { return _direction; }

	//플레이어 및 화살 
	playerEric* getEric()		{ return _eric; }
	playerbaleog* getbaleog()   { return _baleog;}
	PlayerOlaf* getOlaf()		{ return _olaf; }

	tagPlayer getPlayerEric()       { return _eric->getEric(); }
	tagPlayer getPlayerBaleog()     { return _baleog->getBaleog(); }
	tagPlayer getPlayerOlaf()       { return _olaf->getOlaf(); }

	arrow* arrowOne() {	return _arrow;}

	void KILLPlayer();
	void trapColision();
	void itemColision();
	void enemyColision();
	//인벤토리 관련 함수 
	void itemKey();
	void itemUse();
	int itemConnect(int playing);
	bool getItem()	{ return _eric->getItem(); }
	bool getTrade() { return _trade; }

	int getPlaying() { return _playing; }

	void set_Playing(int playChar)						{ _playing = playChar; }
	void setWorldObjectAddressLink(worldObjects* wo)	{ _wo = wo; }
	void setMemoryAddressLink(EnemyManager* em)			{ _em = em; }

	void removeInven(int Num) { _vInven.erase(_vInven.begin() + Num); }		// 인벤토리 삭제 

	void setEricDead() { _eric->setEricDead(false); }
	void setBaleogDead() { _baleog->setBaleogDead(false); }
	void setOlafDead() { _olaf->setOlafDead(false); }

	//훈재 추가
	void StandOnOlafShield();
};