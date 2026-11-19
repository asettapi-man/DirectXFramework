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

	inline vnSprite* GetNormalCrosshair() const { return normalCrosshair; }	//通常クロスヘア画像取得
	inline vnSprite* GetActionCrosshair() const { return actionCrosshair; }	//アクションクロスヘア画像取得

	void UpdateCrosshairUI(bool flag);	//クロスヘア表示変更
};