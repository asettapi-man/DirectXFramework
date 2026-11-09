#include "../framework.h"
#include "../Framework/environment.h"
#include "crosshair_ui.h"

CrosshairUI::CrosshairUI()
{
	crosshair = new vnSprite(
		(float)vnMainFrame::screenCenterX,
		(float)vnMainFrame::screenCenterY,
		WIDTH,
		HEIGHT,
		L"data/image/centerDot.png"
	);
}

void CrosshairUI::UpdateCrosshairUI(bool flag)
{
	if (flag)
	{
		
	}
}
