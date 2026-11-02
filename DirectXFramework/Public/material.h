#pragma once

#include <vector>
#include "model.h"

/// <summary>
/// 資材情報
/// </summary>
struct ItemData
{
	vnModel* model;
	float weight;	// 資源の重さ
};

//資材共通クラス
class Material
{
private:
	const int minDropCount = 2;	//最小ドロップ数
	const int maxDropCount = 4;	//最大ドロップ数
	const float minMaterialWeight = 5.0f;	//最小重量
	const float maxMaterialWeight = 10.0f;	//最大重量

	ItemData itemData;

public:
	~Material();
	float GetRandomWeight();						//資材の重量をランダム化
	std::vector<vnModel*> RandomDropMaterials();	//幾つかの資材を生成
};