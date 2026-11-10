#include "../../framework.h"
#include "../../Framework/environment.h"
#include "inventory_ui.h"

InventoryUI::~InventoryUI()
{
	for (std::size_t i = 0; i < inventoryUI.size(); i++)
	{
		delete inventoryUI[i]->frameSprite;
		delete inventoryUI[i]->itemSprite;
	}
}

void InventoryUI::Render(Inventory& inventory)
{
	for (std::size_t i = 0; i < inventoryUI.size(); i++)
	{
		auto& ui = inventoryUI[i];
		ui->frameSprite->render();
		ui->itemSprite->render();

		//資材の重量
		float weight = inventory.GetInventory()[i]->GetItemData().weight;

		WCHAR buf[64];	//バッファ（文字列の幅・高さを測るため）
		swprintf_s(buf, _countof(buf), L"%.1fkg", weight);	//数値を文字列に変換

		DWRITE_TEXT_METRICS metrics{};	//出力用メトリクス
		if (vnFont::MeasureText(buf, ui->width, ui->height, metrics))
		{
			float textX = ui->x - metrics.width / 2.0f;
			float textY = ui->y - metrics.height / 2.0f;

			vnFont::print(textX, textY, L"%ls", buf);	//WCHAR出力のために%lsが必須
		}
	}
}

void InventoryUI::SetInventorySlot()
{
	float x = (float)400.0f + 100.0f * inventoryUI.size();
	float y = (float)100.0f + 100.0f * inventoryUI.size();
	vnSprite* fSprite = new vnSprite(x, y, WIDTH, HEIGHT, L"data/image/test.png");	//フレーム画像
	vnSprite* iSprite = new vnSprite(x, y, WIDTH, HEIGHT, L"data/image/test.png");	//アイテム画像
	InventorySlot* slot = new InventorySlot{ x, y, WIDTH, HEIGHT, fSprite, iSprite };

	inventoryUI.push_back(slot);
}
