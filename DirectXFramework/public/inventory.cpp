//--------------------------------------------------------------//
//	"inventory.cpp"												//
//	インベントリクラス											//
//  プレイヤーの持ち物（資材）の管理を担当						//
//									           最終修正日 11/19	//
//--------------------------------------------------------------//

#include "../framework.h"
#include "../Framework/environment.h"
#include "inventory.h"

/// <summary>
/// 追加可能か確認
/// </summary>
/// <param name="material">資材クラス</param>
/// <returns>追加可能であればtrueを返す</returns>
bool Inventory::CanAdd(Material* material) const
{
	//重さが最大重量を越えていないか確認
	return GetTotalWeight() + material->GetItemData().weight < MAXWEIGHT;
}

/// <summary>
/// インベントリ内の合計重量を取得
/// </summary>
/// <returns>合計重量を返す</returns>
float Inventory::GetTotalWeight() const
{
	float totalWeight = 0;	//トータル用

	for (Material* mat : inventory)
	{
		float weight = mat->GetItemData().weight;	//アイテムの重量
		totalWeight += weight;
	}

	return totalWeight;
}

/// <summary>
/// 資材を追加
/// </summary>
/// <param name="material">追加する資材クラス</param>
void Inventory::SetMaterial(Material* material)
{
	//追加可能？
	if (CanAdd(material))
	{
		inventory.push_back(material);	//データを配列に追加
	}
}

/// <summary>
/// 資材の削除
/// </summary>
/// <param name="material">削除する資材クラス</param>
void Inventory::DeleteMaterial(Material* material)
{
	if (inventory.empty()) return;	//要素が無ければ終了

	for (auto itr = inventory.begin(); itr != inventory.end(); itr++)
	{
		if (*itr != material) continue;

		//一致する資材情報があれば削除
		inventory.erase(itr);
		break;
	}
}
