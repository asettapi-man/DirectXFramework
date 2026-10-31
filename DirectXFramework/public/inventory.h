#pragma once

#include <vector>

class Inventory
{
private:

	// インベントリの情報
	struct ItemData
	{
		int weight;	// 資源の重さ
	};

	std::vector<ItemData> inventory;	// インベントリのデータ

	std::size_t maxInventoryCount = 10;		// 最大数
	int maxWeight = 100;					// 最大重量

public:
	Inventory() = default;	//コンストラクタ

	bool CanAdd(const ItemData& data) const;	//追加可能フラグ関数
	int GetTotalWeight(const std::vector<ItemData> inventory) const;	//インベントリの最大重量取得関数
};
