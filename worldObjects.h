#pragma once
#include "gameNode.h"
//������Ʈ�� �÷��̾�� �浹�� ���� playerManager ���� ����
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
	RECT rc;				//������Ʈ(����/�� ��� ��)
	image* image;
	bool isCollision;		//������ �÷��̾ �������� �ƴ���
	bool isUse;				//�������� ����ߴٸ�
	float x, y;				//������Ʈ ��ǥ
	//������ x,y ��
	int frameX;
	int frameY;
	// ���̵� ��
	int fadeIn;
};
class worldObjects : public gameNode
{
private:
	playerManager* _pm;
private:
	//���������������� ���͡����������������������
	vector<tagObjects>				 _vTrap;   // ���� , ��, ���� 
	vector<tagObjects>::iterator    _viTrap;
	//����������������� ���͡����������������������
	vector<tagObjects>				  _vItem;
	vector<tagObjects>::iterator	 _viItem;
private:
	//�����������ᰡ�� ���������������������������
	tagObjects _niddle[3];
private:
	//�����������ᵶ ���������������������������
	tagObjects _poision[4];
	bool _isntColliToNiddle;
private:
	//�����������Ắ �� ���ǡ����������������������
	tagObjects _flyingBoard; // ���� 
	bool _isUpDown;
	tagObjects _breakableWallOne;
	tagObjects _breakableWallTwo;
	tagObjects _unbreakableWallOne;
	tagObjects _unbreakableWallTwo;
private:
	//���������������������������������������
	tagObjects _waterFall[3];
private:
	//����������������ۡ����������������������
	tagObjects _Items[9];
	tagObjects _isBoomEffects;
	bool _isChange;
private:
	//������������Ż������������������������
	tagObjects _EXIT;
private:
	int _frameCount;			//������ ī����
	int _boomCount;				//��ź����� ��ź�� ������ī����
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

