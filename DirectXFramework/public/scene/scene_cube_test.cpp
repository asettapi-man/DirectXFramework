#include "../../framework.h"
#include "../../Framework/environment.h"

//初期化関数
bool SceneCubeTest::initialize()
{
	pCube = new cCube();
	pTheta = new cCube();
	pPhi = new cCube();

	resetPos = XMVectorSet(pCube->getPositionX(), pCube->getPositionY(), pCube->getPositionZ(), 0.0f);
	resetRot = XMVectorSet(pCube->getRotationX(), pCube->getRotationY(), pCube->getRotationZ(), 0.0f);
	resetScl = XMVectorSet(pCube->getScaleX(), pCube->getScaleY(), pCube->getScaleZ(), 0.0f);
	resetLightDir = XMVectorSet(pLight->getLightDirX(), pLight->getLightDirY(), pLight->getLightDirZ(), 0.0f);
	resetLightCol = XMVectorSet(pLight->getLightColR(), pLight->getLightColB(), pLight->getLightColG(), 0.0f);
	resetLightAmb = XMVectorSet(pLight->getAmbientR(), pLight->getAmbientG(), pLight->getAmbientB(), 0.0f);

	Cursor = 0;

	//極座標の初期値
	radius = 5.0f;	//半径
	theta = 0.0f;	//角度θ
	phi = 0.0f;		//角度φ

	return true;
}

//終了関数
void SceneCubeTest::terminate()
{
	//オブジェクトの削除
	delete pCube;
	delete pTheta;
	delete pPhi;
}

//処理関数
void SceneCubeTest::execute()
{
	float value_t = 0.01f;
	float value_r = 1.0f * 3.141592f / 180.0f;
	float value_s = 0.01f;
	float value_l = 0.01f;

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
		case PositionX:	pCube->addPositionX(value_t);	break;
		case PositionY:	pCube->addPositionY(value_t);	break;
		case PositionZ:	pCube->addPositionZ(value_t);	break;
		case RotateX:	pCube->addRotationX(value_r);	break;
		case RotateY:	pCube->addRotationY(value_r);	break;
		case RotateZ:	pCube->addRotationZ(value_r);	break;
		case ScaleX:	pCube->addScaleX(value_s);		break;
		case ScaleY:	pCube->addScaleY(value_s);		break;
		case ScaleZ:	pCube->addScaleZ(value_s);		break;
		case LightDirX: pLight->addLightDirX(value_l);	break;
		case LightDirY: pLight->addLightDirY(value_l);	break;
		case LightDirZ: pLight->addLightDirZ(value_l);	break;
		case LightColR: pLight->addLightColR(value_l);	break;
		case LightColG: pLight->addLightColG(value_l);	break;
		case LightColB: pLight->addLightColB(value_l);	break;
		case LightAmbR: pLight->addAmbientR(value_l);	break;
		case Radius:	radius += value_t;				break;
		case Theta:		theta += value_r;				break;
		case Phi:		phi += value_r;					break;
		}
	}
	else if (vnKeyboard::on(DIK_LEFT))
	{
		switch (Cursor)
		{
		case PositionX:	pCube->addPositionX(-value_t);	break;
		case PositionY:	pCube->addPositionY(-value_t);	break;
		case PositionZ:	pCube->addPositionZ(-value_t);	break;
		case RotateX:	pCube->addRotationX(-value_r);	break;
		case RotateY:	pCube->addRotationY(-value_r);	break;
		case RotateZ:	pCube->addRotationZ(-value_r);	break;
		case ScaleX:	pCube->addScaleX(-value_s);		break;
		case ScaleY:	pCube->addScaleY(-value_s);		break;
		case ScaleZ:	pCube->addScaleZ(-value_s);		break;
		case LightDirX: pLight->addLightDirX(-value_l);	break;
		case LightDirY: pLight->addLightDirY(-value_l);	break;
		case LightDirZ: pLight->addLightDirZ(-value_l);	break;
		case LightColR: pLight->addLightColR(-value_l);	break;
		case LightColG: pLight->addLightColG(-value_l);	break;
		case LightColB: pLight->addLightColB(-value_l);	break;
		case LightAmbR: pLight->addAmbientR(-value_l);	break;
		case Radius:	radius -= value_t;				break;
		case Theta:		theta -= value_r;				break;
		case Phi:		phi -= value_r;					break;
		}
	}
	else if (vnKeyboard::on(DIK_SPACE))
	{
		switch (Cursor)
		{
		case PositionX:	pCube->setPositionX(XMVectorGetX(resetPos));	break;
		case PositionY:	pCube->setPositionY(XMVectorGetY(resetPos));	break;
		case PositionZ:	pCube->setPositionZ(XMVectorGetZ(resetPos));	break;
		case RotateX:	pCube->setRotationX(XMVectorGetX(resetRot));	break;
		case RotateY:	pCube->setRotationY(XMVectorGetY(resetRot));	break;
		case RotateZ:	pCube->setRotationZ(XMVectorGetZ(resetRot));	break;
		case ScaleX:	pCube->setScaleX(XMVectorGetX(resetScl));		break;
		case ScaleY:	pCube->setScaleY(XMVectorGetY(resetScl));		break;
		case ScaleZ:	pCube->setScaleZ(XMVectorGetZ(resetScl));		break;
		case LightDirX: pLight->setLightDirX(XMVectorGetX(resetLightDir));	break;
		case LightDirY: pLight->setLightDirY(XMVectorGetY(resetLightDir));	break;
		case LightDirZ: pLight->setLightDirZ(XMVectorGetZ(resetLightDir));	break;
		case LightColR: pLight->setLightColR(XMVectorGetX(resetLightCol));	break;
		case LightColG: pLight->setLightColG(XMVectorGetX(resetLightCol));	break;
		case LightColB: pLight->setLightColB(XMVectorGetX(resetLightCol));	break;
		case LightAmbR: pLight->setAmbientR(XMVectorGetX(resetLightAmb));	break;
		case Radius:	radius = 5.0f;										break;
		case Theta:		theta = 0.0f;										break;
		case Phi:		phi = 0.0f;											break;
		}
	}

	//マウスによるカメラ操作
	/*
	キー入力	-> vnKeyboard::
	マウス入力	-> vnMouse::
	*/

	//右クリック時
	if (vnMouse::onR())
	{	
		//マウスの移動量を取得
		int dx = vnMouse::getDX();
		int dy = vnMouse::getDY();

		theta += dx * value_t;
		phi += dy * value_t;
	}

	//マウスホイールの回転量を取得
	LONG wheelDelta = vnDirectInput::getMouseState()->lZ;
	if (wheelDelta < 0)	//ホイールを手前に回転
	{
		radius += value_t * 10;
	}
	else if (wheelDelta > 0)	//ホイールを奥に回転
	{
		radius -= value_t * 10;
	}

	//== 2D（平面角）の計算 ==
	//極座標 -> 直交座標の変換
	float tx = radius * cosf(theta);
	float tz = radius * sinf(theta);

	//カメラの位置を更新
	pTheta->setPositionX(tx);
	pTheta->setPositionZ(tz);

	//== 3D（平面角, 仰角）の計算 ==
	float px = radius * cosf(theta) * cosf(phi);
	float py = radius * sinf(phi);
	float pz = radius * sinf(theta) * cosf(phi);

	pPhi->setPositionX(px);
	pPhi->setPositionY(py);
	pPhi->setPositionZ(pz);

	//カメラの座標の更新
	vnCamera::setPosition(px, py, pz);

	//基本
	//vnFont::print(x, y, L"文字列");
	//						AARRGGBB <-- 色の指定(アルファ, 赤, 緑, 青)
	//vnFont::print(x, y, 0xffffffff, L"文字列");
	
	//書式設定
	//vnFont::print(x, y, L"%d", 10);

	//×改行コード
	//vnFont::print(x, y, L"文字列\n文字列", 10);
	//△フォント、文字サイズ -> scene_font_test

	int line = 0;
	vnFont::print(80.0f, (float)(100 + Cursor * 16), 16.0f, L"→");
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"PositionX : %.3f", pCube->getPositionX());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"PositionY : %.3f", pCube->getPositionY());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"PositionZ : %.3f", pCube->getPositionZ());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"RotationX : %.3f", pCube->getRotationX() / 3.141592f * 180.0f);
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"RotationY : %.3f", pCube->getRotationY() / 3.141592f * 180.0f);
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"RotationZ : %.3f", pCube->getRotationZ() / 3.141592f * 180.0f);
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"ScaleX : %.3f", pCube->getScaleX());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"ScaleY : %.3f", pCube->getScaleY());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"ScaleZ : %.3f", pCube->getScaleZ());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"LightDirX : %.3f", pLight->getLightDirX());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"LightDirY : %.3f", pLight->getLightDirY());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"LightDirZ : %.3f", pLight->getLightDirZ());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"LightColR : %.3f", pLight->getLightColR());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"LightColG : %.3f", pLight->getLightColG());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"LightColB : %.3f", pLight->getLightColB());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"Radius : %.3f", radius);
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"Theta : %.3f", theta / 3.141592f * 180.0f);
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), 16.0f, L"Phi : %.3f", phi / 3.141592f * 180.0f);

	vnDebugDraw::Grid();	//グリッドの描画
	vnDebugDraw::Axis();	//軸の描画

	vnScene::execute();
}

//描画関数
void SceneCubeTest::render()
{
	//ビューポートの設定(画面中央正方形)
	//D3D12_VIEWPORT	Viewport;
	//Viewport.TopLeftX = (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;
	//Viewport.TopLeftY = 0;
	//Viewport.Width = (float)SCREEN_HEIGHT;
	//Viewport.Height = (float)SCREEN_HEIGHT;
	//Viewport.MinDepth = 0.0f;
	//Viewport.MaxDepth = 1.0f;
	//vnDirect3D::getCommandList()->RSSetViewports(1, &Viewport);

	//キューブの描画
	pCube->render();
	pTheta->render();
	pPhi->render();
	//ビューポートの設定(フルスクリーンに戻す)
	//Viewport.TopLeftX = 0;
	//Viewport.Width = (float)SCREEN_WIDTH;
	//vnDirect3D::getCommandList()->RSSetViewports(1, &Viewport);

	vnScene::render();
}
