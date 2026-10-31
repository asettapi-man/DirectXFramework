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
	return false;
}

/// <summary>
/// インベントリ内の合計重量を取得
/// </summary>
/// <param name="inventory">インベントリ情報</param>
/// <returns>合計重量を返す</returns>
int Inventory::GetTotalWeight(const std::vector<ItemData> inventory) const
{
	int totalWeight = 0;	//結果用

	for (const ItemData& item : inventory)
	{
		int weight = item.weight;	//アイテムの重量
		totalWeight += weight;
	}

	return totalWeight;
}
