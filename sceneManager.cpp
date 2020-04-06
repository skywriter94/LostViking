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

// �� Ű��, �� Ŭ������ �޾ƿ� ���� �߰� ��Ų��.
gameNode * sceneManager::addScene(string sceneName, gameNode * scene)
{
	if(!scene) return nullptr;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

HRESULT sceneManager::changeScene(string sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);	//���� Ű���� ã�Ƴ���.

	if (find == _mSceneList.end()) return E_FAIL;		
	if (find->second == _currentScene) return S_OK;		

	if (SUCCEEDED(find->second->init()))
	{
		//� ���� ������ ó���� ����ֱ� ������ ������ �����ش�.
		if (_currentScene) _currentScene->release();

		//���� ���� �ٲٷ��� ���� ��´�
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
	// �÷��̾��� ���¿� ���� ����� �ٲ��ش�.
	switch (_scene.state)
	{
	case SS_NONE:
		break;

	case SS_PAUSE:
		break;

	case SS_INTRO:
		// ��Ʈ�� ���� ����Ѵ�.
		SCENEMANAGER->changeScene("Intro");
		break;

	case SS_STAGE:
		// �������� ���� ����Ѵ�.
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
