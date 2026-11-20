#pragma once

#include <vector>
#include "model.h"

/// <summary>
/// 資材の状態
/// </summary>
enum class MaterialState
{
	NONE,		//0:通常
	HOLD,		//1:保持
	ROTATE,		//2:回転
	CHARGE,		//3:チャージ
	THROW		//4:投擲
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

class Material
{
private:
	static constexpr int MINDROPCOUNT = 2;			//最小ドロップ数
	static constexpr int MAXDROPCOUNT = 4;			//最大ドロップ数
	static constexpr float MINWEIGHT = 5.0f;		//最小重量
	static constexpr float MAXWEIGHT = 10.0f;		//最大重量
	static constexpr float OFFSET = 0.15f;			//調整用

	vnModel* model;			//モデル情報
	AABBCollider collider;	//AABBコライダー
	ItemData itemData;		//資材情報
	MaterialState state;	//状態

	XMVECTOR velocity;	//速度ベクトル
	XMVECTOR gravity;	//重力ベクトル

	XMFLOAT3 colliderExtents;	//コライダーの範囲

	float phi = 0.0f;		//角度φ 緯度
	float theta = 0.0f;		//角度θ 経度

	float elapsed = 0.0f;	//左クリックの長押し時間
	float force = 1.0f;		//投擲力

	bool isHold;	//保持フラグ
	bool isThrow;	//投擲フラグ
public:
	Material(MaterialType type);		//コンストラクタ
	~Material();						//デストラクタ
	
	void Initialize();											//初期化
	void Execute(double deltaTime, const XMVECTOR holdPos);		//継続処理
	void Render();												//AABBコライダー描画

	inline vnModel* GetModel() const { return model; }				//モデル情報取得
	inline AABBCollider& GetAABBCollider()  { return collider; }	//AABBコライダー取得
	inline ItemData& GetItemData() { return itemData; }				//資材情報取得
	inline MaterialState& GetState() { return state; }				//資材状態取得
	inline float GetForce() const { return force; }					//投擲力取得
	inline bool GetIsHold() const { return isHold; }				//保持フラグ取得
	inline bool GetIsThrow() const { return isThrow; }				//投擲フラグ取得

	inline void SetMaterialState(MaterialState state) { this->state = state; }	//資材状態設定
	inline void SetIsHold(bool flag) { isHold = flag; }							//保持フラグ設定

	//== 生成 ==
	float GetRandomWeight();						//資材の重量決定
	void CreateModel(MaterialType type);			//モデル生成
	ItemData CreateItemData(MaterialType type);		//資材情報の作成

	//== 操作 ==
	void RotateMaterial(double deltaTime);					//回転処理
	void ThrowCharge(double deltaTime, XMVECTOR holdPos);	//投擲溜め処理
	void ThrowMaterial(XMVECTOR lineDir);					//投擲処理
};