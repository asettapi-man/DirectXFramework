#pragma once

#include <vector>
#include "../sprite.h"
#include "../inventory.h"

struct InventorySlot
{
	int id;
	vnSprite* frameSprite;
	vnSprite* itemSprite;

	//等しい時に使用するオペレーター
	bool operator==(const InventorySlot& slot)
	{
		return id == slot.id;
	}

	//等しくない時に使用するオペレーター
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

	static int currentID;			//インベントリスロットの番号

	std::vector<InventorySlot*> inventoryUI;	//インベントリスロット

	bool isInventoryUIRender;	//インベントリUI表示フラグ
public:
	InventoryUI();		//コンストラクタ
	~InventoryUI();		//デストラクタ

	void Initialize();					//初期化関数
	void Execute();						//処理関数
	void Render(Inventory& inventory);	//描画関数

	inline std::vector<InventorySlot*> GetInventoryUI() const { return inventoryUI; }	//インベントリスロット配列取得関数
	inline bool GetIsInventoryUIRender() const { return isInventoryUIRender; }			//インベントリUI描画フラグ取得関数

	inline void SetIsInventoryUIRender(bool flag) { isInventoryUIRender = flag; }		//インベントリUI描画フラグ設定関数

	
	void SetInventorySlot(std::size_t loopNum, Inventory& inventory);	//インベントリスロット追加関数

	int GetSelectedIndex(InventorySlot& inventorySlot);				//選択したインベントリスロットの番号取得関数
	void DeleteInventorySlot(InventorySlot& inventorySlot);				//インベントリスロット削除関数
};
