#pragma once

#include "sprite.h"

class CrosshairUI
{
private:
	static constexpr float WIDTH = 200.0f;	//画像の横幅
	static constexpr float HEIGHT = 200.0f;	//画像の縦幅

	vnSprite* crosshair;	//クロスヘア

public:
	CrosshairUI();	//コンストラクタ

	inline vnSprite* GetSprite() const { return crosshair; }	//クロスヘア画像取得関数

	void UpdateCrosshairUI(bool flag);	//フラグごとに画像を変更
};