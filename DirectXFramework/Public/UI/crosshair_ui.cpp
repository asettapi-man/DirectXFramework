#include "../../framework.h"
#include "../../Framework/environment.h"
#include "crosshair_ui.h"

/// <summary>
/// コンストラクタ
/// </summary>
CrosshairUI::CrosshairUI()
{
	normalCrosshair = new vnSprite(
		(float)vnMainFrame::screenCenterX,
		(float)vnMainFrame::screenCenterY,
		WIDTH,
		HEIGHT,
		L"data/image/normalCrosshair.png"
	);

	actionCrosshair = new vnSprite(
		(float)vnMainFrame::screenCenterX,
		(float)vnMainFrame::screenCenterY,
		WIDTH,
		HEIGHT,
		L"data/image/actionCrosshair.png"
	);
}

/// <summary>
/// クロスヘア表示変更関数
/// </summary>
/// <param name="flag">アクションフラグ</param>
void CrosshairUI::UpdateCrosshairUI(bool flag)
{
	//クロスヘアの切り替え
	normalCrosshair->setRenderEnable(!flag);
	actionCrosshair->setRenderEnable(flag);
}
