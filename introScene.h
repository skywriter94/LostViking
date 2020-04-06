#pragma once
#include "gameNode.h"

#define INTROIMAGESIZE 5		// ��Ʈ�� �̹��� ����
#define NEXTSCENECOUNT 50		// ���������� �Ѿ����� �����ִ� �ð�
#define FADEINSPEED 5

enum Scene_Select_RC
{
	SS_GAME_START,
	SS_GAME_PASSWORD
};

struct tagIntroInfo
{
	image*			scene_Image;			// �̹��� ����
	int				scene_Number;			// �� �ѹ�
	bool			scene_State;			// �� ����
	int				fade_In;				// ���ķ����� ����
	char			scene_Str[20];			// ���ڿ� ���� ����
	int				scene_Cnt;				// �̹����� ��µǼ� �����ִ� �ð�

	RECT			scene_Select_rc[2];		// �� ���� ��Ʈ
	image*			scene_Select_Image;		// ���� �̹���
	bool			scene_Change_pos;		// �Է¿� ���� ���� �̹����� ��ǥ�� �ٲ۴�.
	bool			input_Pass;				// �н����� �Է� ����
	int				pass_Select_Pos;		// �н����� ��ġ
};

struct tagPasswordInfo
{
	RECT			pass_Num_rc;			// ��й�ȣ ��Ʈ�� ��ġ
	image*			pass_Num_Image;			// ��й�ȣ �̹���
	int				save_Num_Pos;			// � �̹����� ����ؾ��ϴ��� �̹��� ��ġ����
};

// ��Ʈ�� ���� ó���� Ŭ�����̴�.
class introScene : public gameNode
{
private:
	tagIntroInfo		_introScene;
	tagPasswordInfo		_password[4];	
	int					_passwordMove;
	int					_passCnt;			// �н����� ī��Ʈ
	bool				_passMiss;			// �н����尡 Ʋ�ȴ���
	bool				_changingScene;		// �� ü����

	HDC					_sceneDC;
	image*				_sceneImage;

public:
	introScene();
	~introScene();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void addIntroImage();		// ��Ʈ�ο� ���� �̹����� �����Ѵ�.
	void setting_AlphaBlend();	// ���ĺ��忡 ����� ���� �����Ѵ�.
	void Select_Key();			// ���� Ű�� �����Ѵ�.
	void Input_Key();			// Ű���� �Է¿� ���� ����

};

