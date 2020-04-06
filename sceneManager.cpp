#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

sceneManager::sceneManager()
{
}


sceneManager::~sceneManager()
{
}

gameNode* sceneManager::_currentScene = NULL;

HRESULT sceneManager::init()
{
	_currentScene = NULL;
	_YouDie[0] = false;
	_YouDie[1] = false;
	_YouDie[2] = false;

	return S_OK;
}

void sceneManager::release()
{
	mapSceneIter miSceneList = _mSceneList.begin();

	for (; miSceneList != _mSceneList.end();)
	{
		if (miSceneList->second != NULL)
		{
			if (miSceneList->second == _currentScene) miSceneList->second->release();
			SAFE_DELETE(miSceneList->second);
			miSceneList = _mSceneList.erase(miSceneList);
		}
		else ++miSceneList;
	}

	_mSceneList.clear();
}

void sceneManager::update()
{
	if (_currentScene) _currentScene->update();
}

void sceneManager::render()
{
	if (_currentScene) _currentScene->render();
}

// 씬 키값, 씬 클래스를 받아와 씬을 추가 시킨다.
gameNode * sceneManager::addScene(string sceneName, gameNode * scene)
{
	if(!scene) return nullptr;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

HRESULT sceneManager::changeScene(string sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);	//씬의 키값을 찾아낸다.

	if (find == _mSceneList.end()) return E_FAIL;		
	if (find->second == _currentScene) return S_OK;		

	if (SUCCEEDED(find->second->init()))
	{
		//어떤 씬의 정보가 처음에 들어있기 때문에 릴리즈 시켜준다.
		if (_currentScene) _currentScene->release();

		//현재 씬에 바꾸려는 씬을 담는다
		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}

void sceneManager::set_SceneState(int state)
{
	if (state >= 0 && state <= 7) _scene.state = (SceneState)state;
}

void sceneManager::SceneChange()
{
	// 플레이어의 상태에 따라 장면을 바꿔준다.
	switch (_scene.state)
	{
	case SS_NONE:
		break;

	case SS_PAUSE:
		break;

	case SS_INTRO:
		// 인트로 씬을 출력한다.
		SCENEMANAGER->changeScene("Intro");
		break;

	case SS_STAGE:
		// 스테이지 씬을 출력한다.
		SCENEMANAGER->changeScene("Stage");
		_scene.state = SS_RESTART;
		break;

	case SS_GAMEOVER:
		SCENEMANAGER->changeScene("GameOver");
		break;

	case SS_STOP:
		break;

	case SS_CLEAR:
		SCENEMANAGER->changeScene("Ending");
		break;
	}
}
