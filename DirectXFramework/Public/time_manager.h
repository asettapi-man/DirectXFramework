#pragma once

#include "../framework.h"

class TimeManager
{
private:
	//１フレームにかかった処理時間
	static double deltaTime;

	//今回と前回の開始時間
	LARGE_INTEGER freq, startCount, endCount;
public:
	static double GetDeltaTime() { return deltaTime; }	//経過時間取得

	void Initialize();	//初期化
	void BeginFrame();	//フレームの開始処理
	void UpdateDeltaTime();	//経過時間更新
};