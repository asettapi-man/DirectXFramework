#include "../framework.h"
#include "../Framework/environment.h"
#include "inventory.h"

/// <summary>
/// 追加可能フラグ関数
/// </summary>
/// <param name="material">資材クラス</param>
/// <returns>追加可能かどうかを返す</returns>
bool Inventory::CanAdd(const Material* material) const
{
	//データを追加可能かどうかと重さが最大重量を越えていないか確認
	return GetTotalWeight() + material->GetItemData().weight < maxWeight;
}

/// <summary>
/// インベントリ内の合計重量を取得する関数
/// </summary>
/// <returns>合計重量を返す</returns>
float Inventory::GetTotalWeight() const
{
	float totalWeight = 0;	//トータル用

	for (const Material* mat : inventory)
	{
		int weight = mat->GetItemData().weight;	//アイテムの重量
		totalWeight += weight;
	}

	return totalWeight;
}

/// <summary>
/// インベントリに資材を追加する関数
/// </summary>
/// <param name="material">資材クラス</param>
void Inventory::SetMaterial(Material* material)
{
	//追加可能？
	if (CanAdd(material))
	{
		inventory.push_back(material);	//データを配列に追加
	}
}

void Inventory::DeleteMaterial(Material* material)
{
	if (inventory.empty()) return;	//要素が無ければ終了

	for (auto itr = inventory.begin(); itr != inventory.end(); itr++)
	{
		if (*itr != material) continue;

		//一致する資材情報があれば削除
		inventory.erase(itr);
	}
}
