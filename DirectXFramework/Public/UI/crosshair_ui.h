#pragma once

#include "../sprite.h"

class CrosshairUI
{
private:
	static constexpr float WIDTH = 200.0f;	//画像の横幅
	static constexpr float HEIGHT = 200.0f;	//画像の縦幅

	vnSprite* normalCrosshair;	//通常クロスヘア
	vnSprite* actionCrosshair;	//アクション時のクロスヘア

public:
	CrosshairUI();	//コンストラクタ

	inline vnSprite* GetNormalCrosshair() const { return normalCrosshair; }	//通常クロスヘア画像取得関数
	inline vnSprite* GetActionCrosshair() const { return actionCrosshair; }	//アクションクロスヘア画像取得関数

	void UpdateCrosshairUI(bool flag);	//フラグごとに画像を変更
};