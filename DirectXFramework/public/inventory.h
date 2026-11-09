#pragma once

#include <vector>
#include "material.h"

class Inventory
{
private:
	//インベントリの情報
	static constexpr float maxWeight = 100.0f;		//最大重量
	std::vector<Material*> inventory;				//インベントリのデータ

public:
	Inventory() = default;	//コンストラクタ

	inline float GetMaxWeight() const { return maxWeight; }		//インベントリ内の最大重量

	bool CanAdd(Material* material) const;			//追加可能フラグ関数
	float GetTotalWeight() const;					//インベントリの最大重量取得関数

	void SetMaterial(Material* material);		//資材の追加
	void DeleteMaterial(Material* material);	//資材の削除
};