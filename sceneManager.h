#pragma once
#include "singletonBase.h"
#include <map>

class gameNode;

// 진행중인 씬의 상태
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

	bool	  _YouDie[3];		// 캐릭터의 죽음을 관리한다.
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

	void set_SceneState(int state);							// 씬의 상태를 셋팅해준다.
	SceneState get_SceneState() { return _scene.state; }	// 씬의 상태를 불러온다.

	void SceneChange();										// 상태에 따라 씬을 바꿔준다.

	void set_PlayerLife(int type, bool dead) { _YouDie[type] = dead; }		// 플레이어 타입, 죽음 여부 (0 = 에릭, 1 = 벨로그, 2 = 올라프)
																			// 죽었다면 true의 값을 넣어준다.
	bool* get_PlayerLife() { return _YouDie; }
	bool get_PlayerLife_Eric() { return _YouDie[0]; }
	bool get_PlayerLife_Baleog() { return _YouDie[1]; }
	bool get_PlayerLife_Olaf() { return _YouDie[2]; }
	void playerDead_Reset() { _playerDead_Reset = true; }		// 이값이 true라면 플레이어들의 죽음 상태를 true로 바꿔준다.
	void playerDead_Reset_END() { _playerDead_Reset = false; }	// 초기화가 끝났다면 이 값을 false로
	bool get_playerDead_Reset() { return _playerDead_Reset; }
	void clear_YouDie() { _YouDie[0] = false, _YouDie[1] = false, _YouDie[2] = false; }
	

};

