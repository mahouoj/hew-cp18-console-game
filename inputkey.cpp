//=============================================================================
//＜使い方＞
//		inputkey.cpp と inputkey.h をプロジェクト内に入れる
//		キー入力するプログラムに「#include "inputkey.h"」を入れる
//
//		GetKeyPress(KEYNAME::～～～～) → ～～～～のキーが押されている間
//		GetKeyTrigger(KEYNAME::～～～～) → ～～～～のキーが押された瞬間
//		GetKeyRelease(KEYNAME::～～～～) → ～～～～のキーが解放された瞬間
//		
//=============================================================================
#define CONIOEX
#include "conioex.h"
#include "inputkey.h"
#include <iostream>

// キーコンフィグ設定
#define KEY_CONFIG_MAX		(4)	//キーコンフィグの登録数

// キーコンフィグ構造体配列と初期化
KEYCONFIG nKey[KEY_CONFIG_MAX] =
{
	//	キーの種類				conioexのキー
	{KEYNAME::KEY_RIGHT		,	PK_RIGHT	},
	{KEYNAME::KEY_LEFT		,	PK_LEFT		},
	{KEYNAME::KEY_JUMP		,	PK_UP		},
	{KEYNAME::KEY_SHOT		,	PK_SP		},

	// 必要なだけ追加
};

// キー情報が格納される構造体配列
KEYSTATE tKeyData[KEYNAME::KEY_NUM];//GAMEKEYNAMEの登録数分作る

//=============================================================================
// 初期化処理
//=============================================================================
void InitKey()
{
	//キー情報を０クリアする
	memset(&tKeyData[0], 0, sizeof(tKeyData));

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitKey()
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateKey()
{
	//KEYCONFIGに登録されたキーの情報を取得
	for (int nCnt = 0; nCnt < KEY_CONFIG_MAX; nCnt++)
	{
		// 前回のキー情報を保存
		tKeyData[nKey[nCnt].nName].nPressOld = tKeyData[nKey[nCnt].nName].nPress;

		// 現在のキー情報を取得
		tKeyData[nKey[nCnt].nName].nPress = inport(nKey[nCnt].nCode);

		// トリガー情報
		tKeyData[nKey[nCnt].nName].nTrigger =
			(tKeyData[nKey[nCnt].nName].nPressOld ^ tKeyData[nKey[nCnt].nName].nPress) & // 状態変化発生
			tKeyData[nKey[nCnt].nName].nPress;                                           // キー押下

		// リリース情報
		tKeyData[nKey[nCnt].nName].nRelease =
			(tKeyData[nKey[nCnt].nName].nPressOld ^ tKeyData[nKey[nCnt].nName].nPress) & // 状態変化発生
			(~tKeyData[nKey[nCnt].nName].nPress);                                        // キー解放

	}

}

//=============================================================================
// 描画処理 (デバッグ表示用)
//=============================================================================
void DrawKey()
{
#ifdef _DEBUG
	int nX = 1, nY = 2;			// 座標設定
	gotoxy(nX, nY);
	textcolor(LIGHTCYAN);
	std::cout << "KEY: ";
	PrintButton(tKeyData[KEYNAME::KEY_JUMP]);
	std::cout << "   ";
	PrintButton(tKeyData[KEYNAME::KEY_SHOT]);
	gotoxy(nX, nY + 1);
	std::cout << "    ";
	PrintButton(tKeyData[KEYNAME::KEY_LEFT]);
	std::cout << " ";
	PrintButton(tKeyData[KEYNAME::KEY_RIGHT]);
#endif
}

//=============================================================================
// トリガーデータ取得 (押した瞬間)
//=============================================================================
int GetKeyTrigger(KEYNAME nKey)
{
	return tKeyData[nKey].nTrigger;
}

//=============================================================================
// リリースデータ取得 (押した瞬間)
//=============================================================================
int GetKeyRelease(KEYNAME nKey)
{
	return tKeyData[nKey].nRelease;
}

//=============================================================================
// プレスデータ取得 (押し続けている状態)
//=============================================================================
int GetKeyPress(KEYNAME nKey)
{
	return tKeyData[nKey].nPress;
}

//=============================================================================
// ボタン描画 (デバッグ表示用)
//=============================================================================
void PrintButton(KEYSTATE tKeyState)
{
	// 色設定
	if (tKeyState.nTrigger)				// キーを押したとき
	{
		textcolor(RED);
	}
	else if (tKeyState.nRelease)	// キーを離したとき
	{
		textcolor(YELLOW);
	}
	else
	{
		textcolor(LIGHTCYAN);
	}

	// 表示設定
	if (tKeyState.nPress)				// キーを押している間
	{
		std::cout << "@";
	}
	else
	{
		std::cout << "0";
	}
}