//--------------------------------------------------------------//
//	"inventory_ui.cpp"											//
//	インベントリUIクラス										//
//  インベントリの中身の表示を担当								//
//									           最終修正日 11/19	//
//--------------------------------------------------------------//

#include "../../framework.h"
#include "../../Framework/environment.h"
#include <algorithm>
#include "inventory_ui.h"

/// <summary>
/// コンストラクタ
/// </summary>
InventoryUI::InventoryUI()
{
	isVisible = false;
}

/// <summary>
/// デストラクタ
/// </summary>
InventoryUI::~InventoryUI()
{
	for (auto& ui : inventorySlots)
	{
		delete ui->frameSprite;
		delete ui->itemSprite;
		delete ui;
	}
}

/// <summary>
/// 初期化
/// </summary>
void InventoryUI::Initialize()
{
	isVisible = false;
}

/// <summary>
/// 継続処理
/// </summary>
/// <param name="index">選択したUI番号（出力用）</param>
void InventoryUI::Execute(int& index)
{
	if (!isVisible) return;

	for (auto& ui : inventorySlots)
	{
		if (ui->itemSprite->CheckImageMouseOver())
		{
			//画像の色を灰色に変更
			float gray = 0.5f;	//灰色
			ui->frameSprite->SetImageColor(gray, gray, gray);
			ui->itemSprite->SetImageColor(gray, gray, gray);

			if (!vnMouse::trgL()) continue;
			index = GetSelectedIndex(*ui);
			isSelected = true;
		}
		else
		{
			//画像の色を通常時に戻す
			float white = 1.0f;	//白色
			ui->frameSprite->SetImageColor(white, white, white);
			ui->itemSprite->SetImageColor(white, white, white);
		}
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="inventory">インベントリ情報</param>
void InventoryUI::Render(Inventory& inventory)
{
	const std::size_t inventorySize = inventory.GetInventory().size();	//インベントリのサイズ
	const std::size_t inventorySlotSize = inventorySlots.size();		//インベントリUIのサイズ

	if (inventorySize > inventorySlotSize)
	{
		//足りていない分追加する
		std::size_t sub = inventorySize - inventorySlotSize;
		SetInventorySlot(sub, inventory);
	}

	for (std::size_t i = 0; i < inventorySize; i++)
	{
		auto& ui = inventorySlots[i];	//対象

		ui->id = (int)i;	//番号の修正

		//要素順に配置位置を修正
		ui->frameSprite->posX = (float)100.0f + 100.0f * i;
		ui->frameSprite->posY = (float)100.0f + 100.0f * i;
		ui->itemSprite->posX = (float)100.0f + 100.0f * i;
		ui->itemSprite->posY = (float)100.0f + 100.0f * i;

		//画像の描画
		ui->frameSprite->render();
		ui->itemSprite->render();

		//資材の重量
		float weight = inventory.GetInventory()[i]->GetItemData().weight;

		WCHAR buf[64];	//バッファ（文字列の幅・高さを測るため）
		swprintf_s(buf, _countof(buf), L"%.1fkg", weight);	//数値を文字列に変換

		DWRITE_TEXT_METRICS metrics{};	//出力用メトリクス
		if (vnFont::MeasureText(buf, WIDTH, HEIGHT, metrics))
		{
			float textX = ui->frameSprite->posX - metrics.width / 2.0f;
			float textY = ui->frameSprite->posY - metrics.height / 2.0f;

			vnFont::print(textX, textY, L"%ls", buf);	//WCHAR出力のために%lsが必須
		}
	}
}

/// <summary>
/// インベントリスロット追加
/// </summary>
/// <param name="loopNum">追加回数</param>
/// <param name="inventory">インベントリ情報</param>
void InventoryUI::SetInventorySlot(std::size_t addNum, Inventory& inventory)
{
	if (addNum <= 0) return;	//０以下なら処理を行わない

	//足りていない分追加する
	for (std::size_t i = 0; i < addNum; i++)
	{
		//inventory配列の追加対象の要素番号を確認
		std::size_t targetNum = inventory.GetInventory().size() - addNum + i;

		//inventory側にある資材の素材情報を取得
		switch (inventory.GetInventory()[targetNum]->GetItemData().type)
		{
		case MaterialType::WOOD:
			break;

		case MaterialType::METAL:
			break;
		}

		//対応した画像を作成
		vnSprite* fSprite = new vnSprite(0.0f, 0.0f, WIDTH, HEIGHT, L"data/image/test.png");	//フレーム画像
		vnSprite* iSprite = new vnSprite(0.0f, 0.0f, WIDTH, HEIGHT, L"data/image/test.png");	//アイテム画像
		InventorySlot* slot = new InventorySlot{ 0, fSprite, iSprite };
	
		inventorySlots.push_back(slot);
	}
}

/// <summary>
/// 選択したインベントリスロットの番号取得
/// </summary>
/// <param name="target">選択したインベントリスロット</param>
/// <returns>一致した要素がある番号を返す</returns>
int InventoryUI::GetSelectedIndex(InventorySlot& target)
{
	//インベントリ画面にあるスロットすべてを確認
	for (std::size_t i = 0; i < inventorySlots.size(); i++)
	{
		if (*inventorySlots[i] == target)
		{
			return i;
		}
	}
}

/// <summary>
/// インベントリスロット削除
/// </summary>
/// <param name="target">削除するインベントリスロット</param>
void InventoryUI::DeleteInventorySlot(InventorySlot* target)
{
	//インベントリ画面にあるスロットすべてを確認
	for (auto itr = inventorySlots.begin(); itr != inventorySlots.end(); itr++)
	{
		if (*itr != target) continue;

		inventorySlots.erase(itr);	//削除

		//メモリ破棄
		delete target->frameSprite;
		delete target->itemSprite;
		delete target;

		break;
	}
}
