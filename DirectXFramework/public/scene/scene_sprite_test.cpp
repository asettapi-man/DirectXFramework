//--------------------------------------------------------------//
//	"scene_sprite_test.cpp"										//
//		スプライトテスト										//
//													2026/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../Framework/environment.h"

//初期化関数
bool SceneSpriteTest::initialize()
{
	//スプライトの作成 (中心座標X, Y, サイズX, Y, 画像のパス, UV(省略可))
	pSprite = new vnSprite(						//画像サイズ（1280*720）
		(float)vnMainFrame::screenCenterX,		//中心座標X　640
		(float)vnMainFrame::screenCenterY,		//中心座標Y　360
		512.0f, 512.0f,							//幅・高さ
		L"data/image/sample_texture.png");		//画像の相対パス（先頭に`L`を付けるときはWCHAR型（日本語を扱えるchar型）の時だけ）

	//4頂点にUV座標の設定
	pSprite->vtx[0].u = 0.0f;
	pSprite->vtx[0].v = 0.0f;

	pSprite->vtx[1].u = 0.5f;
	pSprite->vtx[1].v = 0.0f;

	pSprite->vtx[2].u = 0.0f;
	pSprite->vtx[2].v = 0.5f;

	pSprite->vtx[3].u = 0.5f;
	pSprite->vtx[3].v = 0.5f;

	counter = 0;

	return true;
}

//終了関数
void SceneSpriteTest::terminate()
{
	//スプライトの削除
	delete pSprite;
}

//処理関数（60fps）
void SceneSpriteTest::execute()
{
	if (vnKeyboard::trg(DIK_SPACE))
	{
		counter++;
		int current = counter % ImageCount;

		//スペースキーが押された瞬間
		//4頂点にUV座標の再設定
		switch (current)
		{
		case 0:
			pSprite->vtx[0].u = 0.0f;
			pSprite->vtx[0].v = 0.0f;

			pSprite->vtx[1].u = 0.5f;
			pSprite->vtx[1].v = 0.0f;

			pSprite->vtx[2].u = 0.0f;
			pSprite->vtx[2].v = 0.5f;

			pSprite->vtx[3].u = 0.5f;
			pSprite->vtx[3].v = 0.5f;
			break;

		case 1:
			pSprite->vtx[0].u = 0.5f;
			pSprite->vtx[0].v = 0.0f;

			pSprite->vtx[1].u = 1.0f;
			pSprite->vtx[1].v = 0.0f;

			pSprite->vtx[2].u = 0.5f;
			pSprite->vtx[2].v = 0.5f;

			pSprite->vtx[3].u = 1.0f;
			pSprite->vtx[3].v = 0.5f;
			break;

		case 2:
			pSprite->vtx[0].u = 0.0f;
			pSprite->vtx[0].v = 0.5f;

			pSprite->vtx[1].u = 0.5f;
			pSprite->vtx[1].v = 0.5f;

			pSprite->vtx[2].u = 0.0f;
			pSprite->vtx[2].v = 1.0f;

			pSprite->vtx[3].u = 0.5f;
			pSprite->vtx[3].v = 1.0f;
			break;

		case 3:
			pSprite->vtx[0].u = 0.5f;
			pSprite->vtx[0].v = 0.5f;

			pSprite->vtx[1].u = 1.0f;
			pSprite->vtx[1].v = 0.5f;

			pSprite->vtx[2].u = 0.5f;
			pSprite->vtx[2].v = 1.0f;

			pSprite->vtx[3].u = 1.0f;
			pSprite->vtx[3].v = 1.0f;
			break;
		}
	}

	float value_p = 1.0f;
	float value_s = 0.01f;
	float value_r = 1.0f * 3.141592f / 180.0f;

	if (vnKeyboard::trg(DIK_UP) && --Cursor < 0)
	{
		Cursor = OperationMax - 1;
	}
	else if (vnKeyboard::trg(DIK_DOWN) && ++Cursor >= OperationMax)
	{
		Cursor = 0;
	}
	else if (vnKeyboard::on(DIK_RIGHT))
	{
		switch (Cursor)
		{
		case PositionX:	pSprite->posX += value_p;	break;
		case PositionY:	pSprite->posY += value_p;	break;
		case ScaleX:	pSprite->scaleX += value_s;	break;
		case ScaleY:	pSprite->scaleY += value_s;	break;
		case Rotate:	pSprite->rot += value_r;	break;
		}
	}
	else if (vnKeyboard::on(DIK_LEFT))
	{
		switch (Cursor)
		{
		case PositionX:	pSprite->posX -= value_p;	break;
		case PositionY:	pSprite->posY -= value_p;	break;
		case ScaleX:	pSprite->scaleX -= value_s;	break;
		case ScaleY:	pSprite->scaleY -= value_s;	break;
		case Rotate:	pSprite->rot -= value_r;	break;
		}
	}
	else if (vnKeyboard::on(DIK_SPACE))
	{
		switch (Cursor)
		{
		case PositionX:	pSprite->posX = (float)vnMainFrame::screenCenterX;	break;
		case PositionY:	pSprite->posY = (float)vnMainFrame::screenCenterY;	break;
		case ScaleX:	pSprite->scaleX = 1.0f;	break;
		case ScaleY:	pSprite->scaleY = 1.0f;	break;
		case Rotate:	pSprite->rot = 0.0f;	break;
		}
	}

	float x = 30.0f;
	float y = 10.0f;
	int line = 0;
	int interval = 16;
	vnFont::print(x - 20.0f, (float)(y + Cursor * interval), 16.0f, L"→");
	vnFont::print(x, y + (float)(line++ * interval), 16.0f, L"Position X : %.3f", pSprite->posX);
	vnFont::print(x, y + (float)(line++ * interval), 16.0f, L"Position Y : %.3f", pSprite->posY);
	vnFont::print(x, y + (float)(line++ * interval), 16.0f, L"Scale X    : %.3f", pSprite->scaleX);
	vnFont::print(x, y + (float)(line++ * interval), 16.0f, L"Scale Y    : %.3f", pSprite->scaleY);
	vnFont::print(x, y + (float)(line++ * interval), 16.0f, L"Rotation   : %.3f", pSprite->rot / 3.1415192f * 180.0f);

	vnScene::execute();
}

//描画関数
void SceneSpriteTest::render()
{
	pSprite->render();
}

