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

/// <summary>
/// 資材の種類
/// </summary>
enum class MaterialType
{
	Wood,	//木材
	Metal	//金属
};

//資材共通クラス
class Material
{
private:
	const int minDropCount = 2;				//最小ドロップ数
	const int maxDropCount = 4;				//最大ドロップ数
	const float minMaterialWeight = 5.0f;	//最小重量
	const float maxMaterialWeight = 10.0f;	//最大重量

	ItemData itemData;

public:
	Material(MaterialType type);		//コンストラクタ
	~Material();						//デストラクタ

	float GetRandomWeight();						//資材の重量決定関数
	vnModel* CreateModel(MaterialType type);		//資材生成関数
	ItemData CreateItemData(MaterialType type);		//資材情報の作成関数
};