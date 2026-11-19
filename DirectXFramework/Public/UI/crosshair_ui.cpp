//--------------------------------------------------------------//
//	"crosshair_ui.cpp"											//
//	クロスヘア表示クラス										//
//  クロスヘアの変更や表示を担当								//
//									           最終修正日 11/19	//
//--------------------------------------------------------------//

#include "../../framework.h"
#include "../../Framework/environment.h"
#include "crosshair_ui.h"

/// <summary>
/// コンストラクタ
/// </summary>
CrosshairUI::CrosshairUI()
{
	//クロスヘア画像の初期化
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
/// クロスヘア表示変更
/// </summary>
/// <param name="flag">trueでアクションクロスヘアに変更</param>
void CrosshairUI::UpdateCrosshairUI(bool flag)
{
	//クロスヘアの切り替え
	normalCrosshair->setRenderEnable(!flag);
	actionCrosshair->setRenderEnable(flag);
}
