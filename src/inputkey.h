#ifndef _INPUTKEY_H_
#define _INPUTKEY_H_

enum	KEYNAME	//ゲーム内で使うキーの種類
{
	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_JUMP,
	KEY_SHOT,

	KEY_NUM,	// キーの種類数
};

struct	KEYSTATE		//キー状態(各キー設定1つに対して構造体１つ)
{
	int		nTrigger;		//押した瞬間
	int		nRelease;		//離した瞬間
	int		nPress;			//現在のキー状態
	int		nPressOld;	//前回のPressの値
};

struct	KEYCONFIG			//ゲーム内のキーとキーボードキーの割り当て
{
	KEYNAME		nName;		//ゲーム内のキー種類
	int				nCode;		//conioex.hのキーボードのキーコード
};

//プロトタイプ宣言
void	InitKey();		//初期化
void	UninitKey();	//終了処理
void	UpdateKey();	//キー情報の更新
void	DrawKey();		//描画処理（デバッグ等で使う）

int		GetKeyTrigger(KEYNAME nKey);	//キーのトリガー情報を取得
int		GetKeyPress(KEYNAME nKey);		//キーのプレス情報を取得
int		GetKeyRelease(KEYNAME nKey);	//キーのリリース情報を取得

void	PrintButton(KEYSTATE tKeyState);		//ボタン描画（デバッグ用）

#endif //_INPUTKEY_H_
