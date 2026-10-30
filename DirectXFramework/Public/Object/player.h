#pragma once

#include "../model.h"
#include "../../Public/Collider/AABBCollider.h"

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

	vnModel* model;	//プレイヤーモデル
	AABBCollider aabbCollider;	//AABBの当たり判定

	XMVECTOR vMove;		//移動ベクトル
	XMVECTOR velocity;	//速度ベクトル
	XMVECTOR gravity;	//重力ベクトル

	XMFLOAT3 playerPos;	//プレイヤーの座標

	float jumpForce;	//ジャンプ力

	bool isJump;	//ジャンプ可能フラグ

	//極座標の情報
	float radius;	//半径
	float theta;	//角度θ(平面角,経度)
	float phi;		//角度φ(仰角,緯度)

	bool reverseH;	//水平反転
	bool reverseV;	//垂直反転

public:

	// == ゲッター関数 == 
	inline vnModel* GetModel() { return model; }	//モデルの取得
	inline AABBCollider& GetAABBCollider() { return aabbCollider; }	//AABBの当たり判定の取得
	inline XMVECTOR GetvMove() { return vMove; }	//移動ベクトルの取得
	inline XMVECTOR Getvelocity() { return velocity; }	//速度ベクトルの取得
	inline bool GetIsJump() { return isJump; }	//ジャンプフラグの取得

	Player(const WCHAR* folder, const WCHAR* file, const XMFLOAT3& boxColliderCenter, const XMFLOAT3& boxColliderExtents);	//コンストラクタ
	~Player();			//デストラクタ

	void Initialize();	//初期化
	void Execute();		//処理
	void Render();		//描画

	//== AABBの当たり判定 ==
	bool CheckPlayerCollision(AABBCollider& c);
};
