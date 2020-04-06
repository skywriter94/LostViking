#pragma once
#include "gameNode.h"

#define INTROIMAGESIZE 5		// 인트로 이미지 개수
#define NEXTSCENECOUNT 50		// 다음씬으로 넘어가기까지 멈춰있는 시간
#define FADEINSPEED 5

enum Scene_Select_RC
{
	SS_GAME_START,
	SS_GAME_PASSWORD
};

struct tagIntroInfo
{
	image*			scene_Image;			// 이미지 저장
	int				scene_Number;			// 씬 넘버
	bool			scene_State;			// 씬 상태
	int				fade_In;				// 알파랜더값 저장
	char			scene_Str[20];			// 문자열 저장 공간
	int				scene_Cnt;				// 이미지가 출력되서 멈춰있는 시간

	RECT			scene_Select_rc[2];		// 씬 선택 렉트
	image*			scene_Select_Image;		// 셀렉 이미지
	bool			scene_Change_pos;		// 입력에 따라 셀렉 이미지의 좌표를 바꾼다.
	bool			input_Pass;				// 패스워드 입력 상태
	int				pass_Select_Pos;		// 패스워드 위치
};

struct tagPasswordInfo
{
	RECT			pass_Num_rc;			// 비밀번호 렉트의 위치
	image*			pass_Num_Image;			// 비밀번호 이미지
	int				save_Num_Pos;			// 어떤 이미지를 출력해야하는지 이미지 위치정보
};

// 인트로 씬을 처리할 클래스이다.
class introScene : public gameNode
{
private:
	tagIntroInfo		_introScene;
	tagPasswordInfo		_password[4];	
	int					_passwordMove;
	int					_passCnt;			// 패스워드 카운트
	bool				_passMiss;			// 패스워드가 틀렸는지
	bool				_changingScene;		// 씬 체인지

	HDC					_sceneDC;
	image*				_sceneImage;

public:
	introScene();
	~introScene();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void addIntroImage();		// 인트로에 쓰일 이미지를 셋팅한다.
	void setting_AlphaBlend();	// 알파블렌드에 사용할 값을 연산한다.
	void Select_Key();			// 선택 키를 설정한다.
	void Input_Key();			// 키보드 입력에 따른 저장

};

