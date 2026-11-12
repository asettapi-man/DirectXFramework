#include "../../framework.h"
#include "../../Framework/environment.h"
#include <algorithm>
#include "inventory_ui.h"

int InventoryUI::currentID = 0;

/// <summary>
/// コンストラクタ
/// </summary>
InventoryUI::InventoryUI()
{
	isInventoryUIRender = false;
}

/// <summary>
/// デストラクタ
/// </summary>
InventoryUI::~InventoryUI()
{
	for (auto& ui : inventoryUI)
	{
		delete ui->frameSprite;
		delete ui->itemSprite;
	}
}

/// <summary>
/// 初期化関数
/// </summary>
void InventoryUI::Initialize()
{
	isInventoryUIRender = false;
}

/// <summary>
/// 処理関数
/// </summary>
void InventoryUI::Execute()
{
	for (auto& ui : inventoryUI)
	{
		if (ui->itemSprite->CheckImageMouseOver())
		{
			//画像の色を灰色に変更
			ui->frameSprite->SetImageColor(0.5f, 0.5f, 0.5f);
			ui->itemSprite->SetImageColor(0.5f, 0.5f, 0.5f);

			if (!vnMouse::trgL()) return;
			DeleteInventorySlot(*ui);

		}
		else
		{
			//画像の色を通常時に戻す
			ui->frameSprite->SetImageColor(1.0f, 1.0f, 1.0f);
			ui->itemSprite->SetImageColor(1.0f, 1.0f, 1.0f);
		}
	}
}

/// <summary>
/// 描画関数
/// </summary>
/// <param name="inventory">インベントリ情報</param>
void InventoryUI::Render(Inventory& inventory)
{
	const std::size_t inventorySize = inventory.GetInventory().size();
	const std::size_t inventorySlotSize = inventoryUI.size();

	if (inventorySize > inventorySlotSize)
	{
		std::size_t sub = inventorySize - inventorySlotSize;
		SetInventorySlot(sub, inventory);
	}

	for (std::size_t i = 0; i < inventorySize; i++)
	{
		auto& ui = inventoryUI[i];

		//要素順に配置位置を修正
		ui->frameSprite->posX = (float)100.0f + 100.0f * inventoryUI.size();
		ui->frameSprite->posY = (float)100.0f + 100.0f * inventoryUI.size();
		ui->itemSprite->posX = (float)100.0f + 100.0f * inventoryUI.size();
		ui->itemSprite->posY = (float)100.0f + 100.0f * inventoryUI.size();

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
/// インベントリスロット追加関数
/// </summary>
/// <param name="loopNum">ループ回数</param>
/// <param name="inventory">インベントリ情報</param>
void InventoryUI::SetInventorySlot(std::size_t loopNum, Inventory& inventory)
{
	if (loopNum <= 0) return;	//０以下なら処理を行わない

	//足りていない分追加する
	for (std::size_t i = 0; i < loopNum; i++)
	{
		//inventory配列の追加対象の要素番号を確認
		std::size_t targetNum = inventory.GetInventory().size() - loopNum - i;

		currentID++;

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
		InventorySlot* slot = new InventorySlot{ currentID,fSprite, iSprite };
	
		inventoryUI.push_back(slot);
	}
}

int InventoryUI::GetSelectedIndex(InventorySlot& target)
{
	//インベントリ画面にあるスロットすべてを確認

	for (std::size_t i = 0; i < inventoryUI.size(); i++)
	{
		if (*inventoryUI[i] != target)
		{
			return i;
		}
	}
}

void InventoryUI::DeleteInventorySlot(InventorySlot& target)
{
	//インベントリ画面にあるスロットすべてを確認
	for (auto itr = inventoryUI.begin(); itr != inventoryUI.end(); itr++)
	{
		if (**itr != target) continue;

		inventoryUI.erase(itr);
		break;
	}

	currentID--;
}
