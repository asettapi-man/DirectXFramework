#include "time_manager.h"

double TimeManager::deltaTime = 0.0f;

void TimeManager::Initialize()
{
	memset(&freq, 0, sizeof(freq));
	memset(&startCount, 0, sizeof(startCount));
	memset(&endCount, 0, sizeof(endCount));

	QueryPerformanceFrequency(&freq);		//１秒間のカウント数を取得
}

void TimeManager::BeginFrame()
{
	QueryPerformanceCounter(&startCount);	//開始時間を取得
}

void TimeManager::UpdateDeltaTime()
{
	QueryPerformanceCounter(&endCount);	//終了時間を取得

	//経過時間計算
	deltaTime = (double)(endCount.QuadPart - startCount.QuadPart) / freq.QuadPart;

	startCount = endCount;
}
