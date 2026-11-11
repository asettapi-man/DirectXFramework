#pragma once

#include <vector>
#include "../sprite.h"
#include "../inventory.h"

struct InventorySlot
{
	float x, y;
	float width, height;
	vnSprite* frameSprite;
	vnSprite* itemSprite;
};

class InventoryUI
{
private:
	static constexpr float WIDTH = 150.0f;	//画像の幅
	static constexpr float HEIGHT = 150.0f;	//画像の高さ

	std::vector<InventorySlot*> inventoryUI;	//インベントリスロット

	bool isInventoryUIRender;	//インベントリUI表示フラグ
public:
	InventoryUI();	//コンストラクタ
	~InventoryUI();				//デストラクタ

	void Initialize();					//初期化関数
	void Execute();						//処理関数
	void Render(Inventory& inventory);	//描画関数

	inline bool GetIsInventoryUIRender() const { return isInventoryUIRender; }
	inline void SetIsInventoryUIRender(bool flag) { isInventoryUIRender = flag; }

	void SetInventorySlot();		//インベントリスロット追加関数
	void DeleteInventorySlot();		//インベントリスロット削除関数
};
