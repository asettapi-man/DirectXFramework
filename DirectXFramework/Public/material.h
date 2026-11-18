#pragma once

#include <vector>
#include "model.h"

/// <summary>
/// 資材の状態
/// </summary>
enum class MaterialState
{
	NONE,	//0:通常
	HOLD,	//1:保持
	ROTATE	//2:回転
};

/// <summary>
/// 資材の種類
/// </summary>
enum class MaterialType
{
	WOOD,	//木材
	METAL	//金属
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
	static constexpr float offset = 0.15f;				//調整用

	vnModel* model;			//モデル情報
	AABBCollider collider;	//AABBコライダー
	ItemData itemData;
	MaterialState state;

	XMVECTOR velocity;	//速度ベクトル
	XMVECTOR gravity;	//重力ベクトル

	XMFLOAT3 colliderExtents;	//コライダーの範囲


	float phi = 0.0f;		//角度φ 緯度
	float theta = 0.0f;		//角度θ 経度

	bool isHold;		//保持フラグ

public:
	Material(MaterialType type);		//コンストラクタ
	~Material();						//デストラクタ
	
	void Initialize();											//初期化関数
	void Execute(double deltaTime, const XMVECTOR holdPos);		//処理関数
	void Render();												//AABBコライダー描画関数

	inline vnModel* GetModel() const { return model; }			//モデル情報取得関数
	inline AABBCollider& GetAABBCollider() { return collider; }	//AABBコライダー取得関数
	inline ItemData& GetItemData() { return itemData; }			//資材情報取得関数
	inline MaterialState& GetState() { return state; }			//資材状態取得関数
	inline bool GetIsHold() const { return isHold; }			//保持フラグ取得関数

	inline void SetMaterialState(MaterialState state) { this->state = state; }	//資材状態設定関数
	inline void SetIsHold(bool flag) { isHold = flag; }							//保持フラグ設定関数

	//== 生成 ==
	float GetRandomWeight();						//資材の重量決定関数
	void CreateModel(MaterialType type);			//資材生成関数
	ItemData CreateItemData(MaterialType type);		//資材情報の作成関数

	//== 操作 ==
	void RotateMaterial(double deltaTime);	//資材の回転処理関数
};