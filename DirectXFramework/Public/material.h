#pragma once

#include <vector>
#include "model.h"

/// <summary>
/// 資材の種類
/// </summary>
enum class MaterialType
{
	Wood,	//木材
	Metal	//金属
};

/// <summary>
/// 資材情報
/// </summary>
struct ItemData
{
	MaterialType type;
	float weight;		// 資源の重さ
};

//資材共通クラス
class Material
{
private:
	static constexpr int minDropCount = 2;				//最小ドロップ数
	static constexpr int maxDropCount = 4;				//最大ドロップ数
	static constexpr float minMaterialWeight = 5.0f;	//最小重量
	static constexpr float maxMaterialWeight = 10.0f;	//最大重量

	vnModel* model;			//モデル情報
	AABBCollider collider;	//AABBコライダー
	ItemData itemData;

	XMVECTOR velocity;	//速度ベクトル
	XMVECTOR gravity;	//重力ベクトル

public:
	Material(MaterialType type);		//コンストラクタ
	~Material();						//デストラクタ
	
	void Initialize();					//初期化関数
	void Execute(double deltaTime);		//処理関数
	void Render();						//AABBコライダー描画関数

	inline vnModel* GetModel() const { return model; }			//モデル情報取得関数
	inline AABBCollider& GetAABBCollider() { return collider; }	//AABBコライダー取得関数
	inline ItemData& GetItemData() { return itemData; }			//資材情報取得関数

	float GetRandomWeight();						//資材の重量決定関数
	void CreateModel(MaterialType type);			//資材生成関数
	ItemData CreateItemData(MaterialType type);		//資材情報の作成関数
};