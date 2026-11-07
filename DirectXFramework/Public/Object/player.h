#pragma once

#include "../model.h"
#include "../../Public/Collider/AABBCollider.h"
#include "../../Public/inventory.h"

class Player
{
private:
	//プレイヤーの衝突方向
	enum class eDirection
	{
		NONE,
		X_RIGHT,
		X_LEFT,
		Y_TOP,
		Y_DOWN,
		Z_FRONT,
		Z_BACK,
	};

	eDirection direction;

	vnModel* model;				//プレイヤーモデル
	AABBCollider aabbCollider;	//AABBの当たり判定
	Inventory inventory;		//インベントリ情報
	
	XMVECTOR vMove;		//移動ベクトル
	XMVECTOR velocity;	//速度ベクトル
	XMVECTOR gravity;	//重力ベクトル

	XMFLOAT3 playerPos;	//プレイヤーの座標

	//Rayの情報（カメラ）
	XMVECTOR linePos;	//直線上の一点
	XMVECTOR lineDir;	//直線の方向ベクトル

	float jumpForce = 0.3f;	//ジャンプ力
	float speedDown;		//重量による移動速度低下量

	bool isJump;			//ジャンプ可能フラグ
	bool isInventoryOpen;	//インベントリ開閉フラグ

	//極座標の情報
	float radius;	//半径
	float theta;	//角度θ(平面角,経度)
	float phi;		//角度φ(仰角,緯度)

	bool reverseH;	//水平反転
	bool reverseV;	//垂直反転

public:

	// == ゲッター関数 == 
	inline vnModel* GetModel() const { return model; }						//モデルの取得
	inline AABBCollider& GetAABBCollider() { return aabbCollider; }			//AABBの当たり判定の取得
	inline Inventory& GetInventory() { return inventory; }					//インベントリ情報の取得
	inline XMVECTOR GetvMove() const { return vMove; }						//移動ベクトルの取得
	inline XMVECTOR Getvelocity() const { return velocity; }				//速度ベクトルの取得
	inline bool GetIsJump() const { return isJump; }						//ジャンプフラグの取得
	inline bool GetIsInventoryOpen() const { return isInventoryOpen; }		//インベントリ開閉フラグの取得
	inline float GetSpeedDown() const { return speedDown; }					//移動速度低下量の取得

	Player(const WCHAR* folder, const WCHAR* file, const XMFLOAT3& boxColliderCenter, const XMFLOAT3& boxColliderExtents);	//コンストラクタ
	~Player();

	void Initialize();					//初期化関数
	void Execute(double deltaTime);		//処理関数
	void Render();						//描画関数

	//== AABBの当たり判定 ==
	bool CheckPlayerCollision(AABBCollider& c);
};
