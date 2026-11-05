#pragma once

#include "../framework.h"

class TimeManager
{
private:
	//今回と前回の開始時間
	LARGE_INTEGER freq, startCount, endCount;
	static double deltaTime;	//経過時間

public:
	// == ゲッター ==
	static double GetDeltaTime() { return deltaTime; }

	void Initialize();	//初期化関数
	void BeginFrame();	//フレームの開始処理関数
	void UpdateDeltaTime();	//経過時間更新関数
};