#pragma once

#include "../model.h"
#include "../../Public/Collider/AABBCollider.h"

class Player
{
private:
	vnModel* model;	//プレイヤーモデル

	AABBCollider aabbCollider;	//AABBの当たり判定

public:
	inline vnModel* getModel() { return model; }	//モデルの取得

	void initialize();	//初期化
	void execute();		//処理
	void render();		//描画

	//AABBの当たり判定の取得
	AABBCollider& getAABBCollider();
};
