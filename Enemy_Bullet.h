#pragma once
#include"gameNode.h"
#include<vector>
struct tagBullet
{
	float x, y, angle;
	float probeX, probeY;
	RECT rect;
	image* image;
	bool isFire;
};
class Enemy_Bullet:public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	int _bulletMax;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void bulletFire(float x,float y,float angle);
	void bulletMove();
	void probe();

	void removeBullet(int Num);

	vector<tagBullet>			getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() {return _viBullet;}
};

