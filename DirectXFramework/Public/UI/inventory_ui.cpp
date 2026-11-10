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
		vnFont::print(ui->x, ui->y, L"%.1fkg", inventory.GetInventory()[i]->GetItemData().weight);
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
