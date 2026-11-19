//--------------------------------------------------------------//
//	"time_manager.cpp"											//
//	時間管理クラス												//
//  1フレームの処理時間の計算担当								//
//									           最終修正日 11/19	//
//--------------------------------------------------------------//

#include "time_manager.h"

double TimeManager::deltaTime = 0.0f;	//１フレームにかかった処理時間

/// <summary>
/// 初期化
/// </summary>
void TimeManager::Initialize()
{
	//配列の初期化
	memset(&freq, 0, sizeof(freq));
	memset(&startCount, 0, sizeof(startCount));
	memset(&endCount, 0, sizeof(endCount));

	QueryPerformanceFrequency(&freq);		//１秒間のカウント数を取得
}

/// <summary>
/// フレームの開始処理
/// </summary>
void TimeManager::BeginFrame()
{
	QueryPerformanceCounter(&startCount);	//開始時間を取得
}

/// <summary>
/// 経過時間更新
/// </summary>
void TimeManager::UpdateDeltaTime()
{
	QueryPerformanceCounter(&endCount);	//終了時間を取得

	//経過時間計算
	deltaTime = (double)(endCount.QuadPart - startCount.QuadPart) / freq.QuadPart;

	startCount = endCount;
}
