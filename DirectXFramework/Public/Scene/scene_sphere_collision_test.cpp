#include "../../framework.h"
#include "../../Framework/environment.h"

//初期化関数
bool SceneSphereCollisionTest::initialize()
{
	pSphere = new vnModel(L"data/model/primitive/", L"sphere.vnm");
	sphereRadius = 1.0f;
	pSphere->setScale(sphereRadius, sphereRadius, sphereRadius);

	pPoint[0] = new vnModel(L"data/model/primitive/", L"sphere.vnm");
	pPoint[0]->setScale(0.1f, 0.1f, 0.1f);
	pPoint[1] = new vnModel(L"data/model/primitive/", L"sphere.vnm");
	pPoint[1]->setScale(0.1f, 0.1f, 0.1f);

	//開始は全てOFF
	for (int i = 0; i < PointNum; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == 0 && j == 0) continue;
			e[i][j] = eOperation::Off;
		}
	}

	//線の始点と終点の頂点
	points[0] = XMVectorSet(-2.0f, 4.0f, -2.0f, 0.0f);
	points[1] = XMVectorSet(3.0f, 0.0f, 3.0f, 0.0f);
	points[2] = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

	//極座標の初期値
	radius = 7.0f;	//半径
	theta = 0.0f;	//角度θ
	phi = 0.0f;		//角度φ

	return true;
}

//終了関数
void SceneSphereCollisionTest::terminate()
{

}

//処理関数
void SceneSphereCollisionTest::execute()
{
	float value_t = 0.01f;
	float value_r = 1.0f * 3.141592f / 180.0f;
	float value_s = 0.01f;
	float value_l = 0.01f;

	if (vnKeyboard::trg(DIK_UP))
	{
		e[column][row] = eOperation::Off;
		if (--column < 0)
		{
			column = PointNum - 1;
		}
		e[column][row] = eOperation::On;
	}
	else if (vnKeyboard::trg(DIK_DOWN))
	{
		e[column][row] = eOperation::Off;
		if (++column >= PointNum)
		{
			column = 0;
		}
		e[column][row] = eOperation::On;
	}
	else if (vnKeyboard::trg(DIK_RIGHT))
	{
		e[column][row] = eOperation::Off;
		if (++row >= 3)
		{
			row = 0;
		}
		e[column][row] = eOperation::On;
	}
	else if (vnKeyboard::trg(DIK_LEFT))
	{
		e[column][row] = eOperation::Off;
		if (--row < 0)
		{
			row = 2;
		}
		e[column][row] = eOperation::On;
	}
	else if (vnKeyboard::on(DIK_PGUP))
	{
		if (e[column][row] == eOperation::On)
		{
			XMVECTOR v = points[column];
			switch (row)
			{
			case 0:
				v = XMVectorAdd(v, XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f));
				break;
			case 1:
				v = XMVectorAdd(v, XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f));
				break;
			case 2:
				v = XMVectorAdd(v, XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f));
				break;
			}

			points[column] = v;
		}
	}
	else if (vnKeyboard::on(DIK_PGDN))
	{
		if (e[column][row] == eOperation::On)
		{
			XMVECTOR v = points[column];
			switch (row)
			{
			case 0:
				v = XMVectorAdd(v, XMVectorSet(-0.1f, 0.0f, 0.0f, 0.0f));
				break;
			case 1:
				v = XMVectorAdd(v, XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f));
				break;
			case 2:
				v = XMVectorAdd(v, XMVectorSet(0.0f, 0.0f, -0.1f, 0.0f));
				break;
			}
			points[column] = v;
		}
	}

	pSphere->setPosition(&points[2]);

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

	//== 3D（平面角, 仰角）の計算 ==
	float px = radius * cosf(theta) * cosf(phi);
	float py = radius * sinf(phi);
	float pz = radius * sinf(theta) * cosf(phi);

	//カメラの座標の更新
	vnCamera::setPosition(px, py, pz);

	//直線の情報を作成
	linePos = points[0];	//直線上の一点
	lineDir = XMVectorSubtract(points[1], points[0]);	//直線の方向ベクトル
	lineDir = XMVector3Normalize(lineDir);	//正規化

	spherePos = points[2];

	//直線と球の衝突
	XMVECTOR hit[2];
	XMVECTOR v = XMVectorSubtract(linePos, spherePos);
	float vx = XMVectorGetX(v);
	float vy = XMVectorGetY(v);
	float vz = XMVectorGetZ(v);
	if (vx == 0.0f && vy == 0.0f && vz == 0.0f)
	{
		hit[0] = linePos + lineDir * sphereRadius;
		hit[0] = linePos + lineDir * (-sphereRadius);
	}
	else
	{
		XMVECTOR nv = XMVector3Normalize(v);

		XMVECTOR vdt = XMVector3Dot(nv, lineDir);
		XMVECTOR vdl = XMVector3Length(v);
		float dt = XMVectorGetX(vdt);
		//範囲指定
		if (dt > 1.0f) dt = 1.0f;
		else if (dt < -1.0f) dt = -1.0f;

		float dl = XMVectorGetX(vdl);
		float dr = dl * sqrt(1.0f - dt * dt);	//直線と球中心の最近傍距離

		//球の半径より直線が近ければ衝突
		float dq = sqrtf(sphereRadius * sphereRadius - dr * dr);
		float ds = dl * dt;
		float dm = ds - dq;	//直線の視点からの各衝突位置（マイナス側）
		float dp = ds + dq;	//直線の視点からの各衝突位置（プラス側）
		hit[0] = linePos + lineDir * (-dm);
		hit[1] = linePos + lineDir * (-dp);
		pPoint[0]->setPosition(&hit[0]);
		pPoint[1]->setPosition(&hit[1]);


		if (dr <= sphereRadius)
		{
			pPoint[0]->setRenderEnable(true);
			pPoint[1]->setRenderEnable(true);
			pSphere->setDiffuse(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			pPoint[0]->setRenderEnable(false);
			pPoint[1]->setRenderEnable(false);
			pSphere->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	//線の描画
	vnDebugDraw::Line(&points[0], &points[1], 0xffffff00);
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
	//vnFont::print(80.0f, (float)(100 + Cursor * 16), L"→");
	vnFont::print(150.0f, 50.0f + (float)(line * 16), e[column][0] == eOperation::On ? 0xff00ffff : 0xffffffff, L"PositionX");
	vnFont::print(250.0f, 50.0f + (float)(line * 16), e[column][1] == eOperation::On ? 0xff00ffff : 0xffffffff, L"PositionY");
	vnFont::print(350.0f, 50.0f + (float)(line++ * 16), e[column][2] == eOperation::On ? 0xff00ffff : 0xffffffff, L"PositionZ");
	vnFont::print(50.0f, 50.0f + (float)(line * 16), e[0][row] == eOperation::On ? 0xff00ffff : 0xffffffff, L"Line[0]");
	vnFont::print(150.0f, 50.0f + (float)(line * 16), e[0][0] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetX(points[0]));
	vnFont::print(250.0f, 50.0f + (float)(line * 16), e[0][1] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetY(points[0]));
	vnFont::print(350.0f, 50.0f + (float)(line++ * 16), e[0][2] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetZ(points[0]));
	vnFont::print(50.0f, 50.0f + (float)(line * 16), e[1][row] == eOperation::On ? 0xff00ffff : 0xffffffff, L"Line[1]");
	vnFont::print(150.0f, 50.0f + (float)(line * 16), e[1][0] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetX(points[1]));
	vnFont::print(250.0f, 50.0f + (float)(line * 16), e[1][1] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetY(points[1]));
	vnFont::print(350.0f, 50.0f + (float)(line++ * 16), e[1][2] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetZ(points[1]));
	vnFont::print(50.0f, 50.0f + (float)(line * 16), e[2][row] == eOperation::On ? 0xff00ffff : 0xffffffff, L"Tri[0]");
	vnFont::print(150.0f, 50.0f + (float)(line * 16), e[2][0] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetX(points[2]));
	vnFont::print(250.0f, 50.0f + (float)(line * 16), e[2][1] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetY(points[2]));
	vnFont::print(350.0f, 50.0f + (float)(line++ * 16), e[2][2] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetZ(points[2]));
	line++;
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), 0xffffffff, L"操作方法");
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"↑↓ : 頂点の選択");
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"PageUp : 増加");
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"PageDown : 減少");
	line++;
	vnDebugDraw::Grid();	//グリッドの描画
	vnDebugDraw::Axis();	//軸の描画

	vnScene::execute();
}

//描画関数
void SceneSphereCollisionTest::render()
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

	//ビューポートの設定(フルスクリーンに戻す)
	//Viewport.TopLeftX = 0;
	//Viewport.Width = (float)SCREEN_WIDTH;
	//vnDirect3D::getCommandList()->RSSetViewports(1, &Viewport);
	pSphere->render();
	pPoint[0]->render();
	pPoint[1]->render();

	vnScene::render();
}
