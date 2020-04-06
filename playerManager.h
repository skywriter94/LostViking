#pragma once
#include "gameNode.h"
#include "playerEric.h"
#include "playerbaleog.h"
#include "PlayerOlaf.h"
#include "worldObjects.h"
#include <list>

class EnemyManager;
class worldObjects;
// �÷��̾��� Ÿ��
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
	int		 invenNumber;	 // �κ��丮 ��ġ : 0~3, (4 = ��������)
	bool		  choice;    // �����Ѱ��� ���Ѱ����� �Ǵ��ϴ� ���� 
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

	vector<tagInven>		    _vInven;		// �κ��丮 ���� 
	vector<tagInven>::iterator _viInven;		// �κ��丮 ���ͷ����� 

	vector<arrowBreakTag*> _vArrowBreakTag;

	//�κ��丮 ���� ����  // 0 = ERIC, 1 = BALEOG, 2 = OLAF
	int _direction[3];  // player, invenNumber 
	int _itemCount[3];	// player�� ������ �ִ� �������� ���� 
	int	_playing;		// player 
	int invenNum;		// �÷��̾��� �κ��� �� ��°������ ����Ű�� �� 

	bool _trade;		// �ŷ� ������ �ƴ��� �Ǵ�

public:
	playerManager() {}
	~playerManager() {}

	HRESULT init();
	void update();
	void release();
	void render();

	//�κ��丮 ���� 
	vector<tagInven>		   get_vInven()		{ return _vInven; }
	vector<tagInven>::iterator get_viInven()	{ return _viInven;}
	int*  getItemNum()						 { return _direction; }

	//�÷��̾� �� ȭ�� 
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
	//�κ��丮 ���� �Լ� 
	void itemKey();
	void itemUse();
	int itemConnect(int playing);
	bool getItem()	{ return _eric->getItem(); }
	bool getTrade() { return _trade; }

	int getPlaying() { return _playing; }

	void set_Playing(int playChar)						{ _playing = playChar; }
	void setWorldObjectAddressLink(worldObjects* wo)	{ _wo = wo; }
	void setMemoryAddressLink(EnemyManager* em)			{ _em = em; }

	void removeInven(int Num) { _vInven.erase(_vInven.begin() + Num); }		// �κ��丮 ���� 

	void setEricDead() { _eric->setEricDead(false); }
	void setBaleogDead() { _baleog->setBaleogDead(false); }
	void setOlafDead() { _olaf->setOlafDead(false); }

	//���� �߰�
	void StandOnOlafShield();
};