#pragma once

#include <vector>
#include "material.h"

class Inventory
{
private:
	//インベントリの情報
	std::vector<ItemData> inventory;	//インベントリのデータ

	std::size_t maxInventoryCount = 10;		//最大数
	float maxWeight = 100;					//最大重量

public:
	Inventory() = default;	//コンストラクタ

	bool CanAdd(const ItemData& data) const;	//追加可能フラグ関数
	float GetTotalWeight() const;				//インベントリの最大重量取得関数
	void SetItemData(ItemData itemData);		//資材情報の追加
};
