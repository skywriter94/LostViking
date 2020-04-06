#pragma once
#include "singletonBase.h"
#include <map>

class gameNode;

// �������� ���� ����
enum SceneState
{
	SS_NONE,
	SS_PAUSE,
	SS_INTRO,
	SS_STAGE,
	SS_GAMEOVER,
	SS_STOP,
	SS_RESTART,
	SS_CLEAR
};

enum PlayerName
{
	PN_Eric,
	PN_Baleog,
	PN_Olaf
};

struct SceneInfo
{
	SceneState	state;
};


class sceneManager : public singletonBase<sceneManager>
{
public:
	typedef map<string, gameNode*>				mapSceneList;
	typedef map<string, gameNode*>::iterator	mapSceneIter;

private:
	static gameNode* _currentScene;
	mapSceneList _mSceneList;
	SceneInfo _scene;

	bool	  _YouDie[3];		// ĳ������ ������ �����Ѵ�.
	bool	  _playerDead_Reset;

public:
	sceneManager();
	~sceneManager();

	HRESULT init();
	void release();
	void update();
	void render();

	gameNode* addScene(string sceneName, gameNode* scene);

	HRESULT changeScene(string sceneName);

	void set_SceneState(int state);							// ���� ���¸� �������ش�.
	SceneState get_SceneState() { return _scene.state; }	// ���� ���¸� �ҷ��´�.

	void SceneChange();										// ���¿� ���� ���� �ٲ��ش�.

	void set_PlayerLife(int type, bool dead) { _YouDie[type] = dead; }		// �÷��̾� Ÿ��, ���� ���� (0 = ����, 1 = ���α�, 2 = �ö���)
																			// �׾��ٸ� true�� ���� �־��ش�.
	bool* get_PlayerLife() { return _YouDie; }
	bool get_PlayerLife_Eric() { return _YouDie[0]; }
	bool get_PlayerLife_Baleog() { return _YouDie[1]; }
	bool get_PlayerLife_Olaf() { return _YouDie[2]; }
	void playerDead_Reset() { _playerDead_Reset = true; }		// �̰��� true��� �÷��̾���� ���� ���¸� true�� �ٲ��ش�.
	void playerDead_Reset_END() { _playerDead_Reset = false; }	// �ʱ�ȭ�� �����ٸ� �� ���� false��
	bool get_playerDead_Reset() { return _playerDead_Reset; }
	void clear_YouDie() { _YouDie[0] = false, _YouDie[1] = false, _YouDie[2] = false; }
	

};

