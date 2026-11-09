#include "../../framework.h"
#include "../../Framework/environment.h"
#include "inventory_ui.h"

InventoryUI::~InventoryUI()
{
	for (std::size_t i = 0; i < inventoryUI.size(); i++)
	{
		delete inventoryUI.at(i)->sprite;
	}
}

void InventoryUI::Render(Inventory& inventory)
{
	for (std::size_t i = 0; i < inventoryUI.size(); i++)
	{
		auto& ui = inventoryUI.at(i);
		ui->sprite->render();
		vnFont::print(ui->x, ui->y, L"%.1fkg", inventory.GetInventory().at(i)->GetItemData().weight);
	}
}

void InventoryUI::SetInventorySlot()
{
	float x = (float)10.0f + 30.0f * inventoryUI.size();
	float y = (float)10.0f + 30.0f * inventoryUI.size();
	vnSprite* sprite = new vnSprite(x, y, WIDTH, HEIGHT, L"data/image/test.png");
	InventorySlot* slot = new InventorySlot{ x, y, WIDTH, HEIGHT, sprite };

	inventoryUI.push_back(slot);
}
