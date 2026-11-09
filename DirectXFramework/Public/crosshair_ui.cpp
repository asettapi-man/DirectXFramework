#include "../framework.h"
#include "../Framework/environment.h"
#include "crosshair_ui.h"

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

void CrosshairUI::UpdateCrosshairUI(bool flag)
{
	//ƒNƒƒXƒwƒA‚ÌØ‚è‘Ö‚¦
	normalCrosshair->setRenderEnable(!flag);
	actionCrosshair->setRenderEnable(flag);
}
