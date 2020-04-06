#pragma once
#include "gameNode.h"
#include "worldMap.h"			// �������������� �ʿ��ϱ� ������ �̰����� �̵�
#include "playerManager.h"		// �������������� �ʿ��ϱ� ������ �̰����� �̵�
#include "enemyManager.h"		// �������������� �ʿ��ϱ� ������ �̰����� �̵�
#include "waterFall.h"



// �÷��̾��� Ÿ��
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


// ���� UI�� ������ ��� ����ü
struct _tagStateUI
{
	POINTFLOAT		pos;						// ���� 
	RECT			rc;							// ��Ʈ ��ǥ
	image*			image;						// �̹��� �����ϴ°�
	bool			dead;						// ĳ���Ͱ� �׾����� Ȯ��
};

class stageScene : public gameNode
{
private:
	_tagStateUI		_UI_State[3];				// ĳ���͸��� ���� �̹����� �ִ�. 3ĭ
	_tagStateUI		_UI_Inventory[3][4];		// ĳ���͸��� �κ��丮�� �ִ�. ���� 4ĭ��
	_tagStateUI		_UI_Garbage;				// ������ �̹��� ��ġ
	_tagStateUI		_UI_HP[3];					// HP ����
	_tagStateUI		_UI_Ending;					// ���� ����
	RECT			_ERC, _BRC, _ORC;
	bool			_CC;	

	int				_charNum;					// ĳ���͸� ����Ű�� ��ȣ
	int				_banjjack;					// ��¦��¦

	int				_playerAllDeadTimer;		// �÷��̾ ��� �׾����� ���� �ð� �ڿ� ���ӿ��� â���� �Ѿ��.

	int*			_player_InvenPos;			// ���� ����Ű�� �κ� ��ġ
	bool			_player_UsingInven;			// ���� �÷��̾�� �κ��� �̿��ϰ� �ִ�.

	_tagStateUI		_UI_GiveUpPOS[3];			// ���� �̹��� ��ǥ
	_tagStateUI		_UI_GiveSelectPos;			// ���� ���� ��ǥ
	bool			_giveUpStart;				// ������ ȣ��
	bool			_giveUpSelect;				// ���� ����
	bool			_gBanZZank;					// ���� ��¦��¦
	int				_giveCnt;
	bool			_iBanZZack;					// �������� ��¦��¦
	int				_iBanCnt;					// ������ ��¦��¦ ī����

	worldMap* _wm;
	playerManager* _pm;
	EnemyManager* _em;
	waterFall* _wf;

private:
	//���� ������
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
	void addStageImage();		// �������� UI�� �ʿ��� �̹����� �߰��ϴ� �Լ�

	void testStateImage();		// �̹��� �׽�Ʈ�� �Լ�

	void set_PlayerDead();

	void setting_InvenSelectPos();	// �κ��丮�� ���� ����Ű�� ��ġ�� �����صд�.
	void setting_GiveUpPos();		// ���� �޽��� ��ǥ ����

};

