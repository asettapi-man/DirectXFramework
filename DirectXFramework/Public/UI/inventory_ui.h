

#pragma once

#include <vector>
#include "../sprite.h"
#include "../inventory.h"

/// <summary>
/// インベントリスロット
/// </summary>
struct InventorySlot
{
	int id;						//番号
	vnSprite* frameSprite;		//フレーム画像
	vnSprite* itemSprite;		//アイテム画像

	//等号オペレーター
	bool operator==(const InventorySlot& slot)
	{
		return id == slot.id;
	}

	//不等号オペレーター
	bool operator!=(const InventorySlot& slot)
	{
		return !(*this == slot);
	}
};

class InventoryUI
{
private:
	static constexpr float WIDTH = 150.0f;	//画像の幅
	static constexpr float HEIGHT = 150.0f;	//画像の高さ

	std::vector<InventorySlot*> inventorySlots;	//インベントリスロット

	bool isVisible;		//表示フラグ
	bool isSelected;	//選択フラグ
public:
	InventoryUI();		//コンストラクタ
	~InventoryUI();		//デストラクタ

	void Initialize();					//初期化
	void Execute(int& index);			//継続処理
	void Render(Inventory& inventory);	//描画処理

	inline std::vector<InventorySlot*> GetInventoryUI() const { return inventorySlots; }	//インベントリスロット配列取得
	inline bool GetIsVisible() const { return isVisible; }									//表示フラグ取得
	inline bool GetIsSelected() const { return isSelected; }								//選択フラグ取得
	int GetSelectedIndex(InventorySlot& inventorySlot);										//選択したインベントリスロットの番号取得

	inline void SetIsVisible(bool flag) { isVisible = flag; }		//表示フラグ設定
	inline void SetIsSelected(bool flag) { isSelected = flag; }		//選択フラグ設定

	
	void SetInventorySlot(std::size_t loopNum, Inventory& inventory);	//インベントリスロット追加
	void DeleteInventorySlot(InventorySlot* inventorySlot);				//インベントリスロット削除

};
