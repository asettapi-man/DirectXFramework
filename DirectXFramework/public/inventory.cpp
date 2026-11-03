#include "../framework.h"
#include "../Framework/environment.h"
#include "inventory.h"

/// <summary>
/// 追加可能フラグ関数
/// </summary>
/// <param name="data">追加するデータ</param>
/// <returns>追加可能かどうかを返す</returns>
bool Inventory::CanAdd(const ItemData& data) const
{
	//データを追加可能かどうかと重さが最大重量を越えていないか確認
	return inventory.size() < maxInventoryCount
		&& GetTotalWeight() + data.weight < maxWeight;
}

/// <summary>
/// インベントリ内の合計重量を取得する関数
/// </summary>
/// <returns>合計重量を返す</returns>
float Inventory::GetTotalWeight() const
{
	float totalWeight = 0;	//トータル用

	for (const ItemData& item : inventory)
	{
		int weight = item.weight;	//アイテムの重量
		totalWeight += weight;
	}

	return totalWeight;
}

/// <summary>
/// インベントリにデータを追加する関数
/// </summary>
/// <param name="model">モデル情報</param>
/// <param name="weight">重量</param>
void Inventory::SetItemData(ItemData itemData)
{
	//追加可能？
	if (CanAdd(itemData))
	{
		inventory.push_back(itemData);	//データを配列に追加
	}
}
