#pragma once

#include <vector>
#include "material.h"

class Inventory
{
private:
	static constexpr float MAXWEIGHT = 100.0f;		//最大重量
	std::vector<Material*> inventory;				//インベントリ情報

public:
	Inventory() = default;	//コンストラクタ

	inline float GetMaxWeight() const { return MAXWEIGHT; }						//インベントリ内の最大重量取得
	inline std::vector<Material*> GetInventory() const { return inventory; }	//インベントリ情報取得

	float GetTotalWeight() const;					//インベントリの最大重量取得

	bool CanAdd(Material* material) const;			//追加可能か確認

	void SetMaterial(Material* material);		//資材の追加
	void DeleteMaterial(Material* material);	//資材の削除
};