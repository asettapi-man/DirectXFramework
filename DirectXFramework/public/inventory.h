#pragma once

#include <vector>
#include "material.h"

class Inventory
{
private:
	//インベントリの情報
	std::vector<Material*> inventory;	//インベントリのデータ

	std::size_t maxInventoryCount = 10;		//最大数
	float maxWeight = 100.0f;				//最大重量

public:
	Inventory() = default;	//コンストラクタ

	inline std::size_t GetInventoryCount() const { return inventory.size(); }		//インベントリ内の数取得関数	

	bool CanAdd(const Material* material) const;	//追加可能フラグ関数
	float GetTotalWeight() const;					//インベントリの最大重量取得関数

	void SetMaterial(Material* material);		//資材の追加
	void DeleteMaterial(Material* material);	//資材の削除
};