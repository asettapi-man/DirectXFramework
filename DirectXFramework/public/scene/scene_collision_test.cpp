#include "../../framework.h"
#include "../../Framework/environment.h"

//初期化関数
bool SceneCollisionTest::initialize()
{
	pPoint = new vnModel(L"data/model/primitive/", L"sphere.vnm");
	pPoint->setScale(0.1f, 0.1f, 0.1f);

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
	points[1] = XMVectorSet(3.0f, 4.0f, 3.0f, 0.0f);

	//三角形の頂点座標
	points[2] = XMVectorSet(-1.0f, 0.0f, 1.0f, 0.0f);
	points[3] = XMVectorSet(-1.0f, 0.0f, -1.0f, 0.0f);
	points[4] = XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);

	//極座標の初期値
	radius = 7.0f;	//半径
	theta = 0.0f;	//角度θ
	phi = 0.0f;		//角度φ

	return true;
}

//終了関数
void SceneCollisionTest::terminate()
{

}

//処理関数
void SceneCollisionTest::execute()
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

	//平面の情報を作成
	XMVECTOR v1, v2;	//三角形の辺
	v1 = XMVectorSubtract(points[3], points[2]);	//辺1
	v2 = XMVectorSubtract(points[4], points[2]);	//辺2

	planeNormal = XMVector3Cross(v1, v2);	//法線ベクトル（外積）
	planeNormal = XMVector3Normalize(planeNormal);	//正規化

	XMVECTOR vDot = XMVector3Dot(planeNormal, points[2]);	//平面と原点の距離
	PlaneDistance = -XMVectorGetX(vDot);

	//直線と平面の衝突点を求める
	float d, s;
	XMVECTOR vS = XMVector3Dot(planeNormal, linePos);
	s = XMVectorGetX(vS);
	s += PlaneDistance;
	XMVECTOR vD = XMVector3Dot(planeNormal, linePos + lineDir);
	d = XMVectorGetX(vD);
	d += PlaneDistance;

	float t = s / (s - d);	//衝突点のパラメータt
	float dif = s - d;	//floatの差

	if (fabsf(dif) > 0.000001f && t >= 0)
	{
		XMVECTOR hit = linePos + lineDir * t;	//衝突点の座標

		pPoint->setPosition(&hit);	//衝突点の座標をポイントに設定

		//衝突判定
		//三角形の辺のベクトル作成
		XMVECTOR edge1 = XMVectorSubtract(points[3], points[2]);
		XMVECTOR edge2 = XMVectorSubtract(points[4], points[3]);
		XMVECTOR edge3 = XMVectorSubtract(points[2], points[4]);

		//衝突点と三角形の各頂点のベクトル作成
		XMVECTOR hitEdge1 = XMVectorSubtract(hit, points[2]);
		XMVECTOR hitEdge2 = XMVectorSubtract(hit, points[3]);
		XMVECTOR hitEdge3 = XMVectorSubtract(hit, points[4]);

		//三角形の辺と衝突点のベクトルの外積を計算（法線ベクトルと同じ方向かどうかを判定するため）
		XMVECTOR cross1 = XMVector3Cross(edge1, hitEdge1);
		XMVECTOR cross2 = XMVector3Cross(edge2, hitEdge2);
		XMVECTOR cross3 = XMVector3Cross(edge3, hitEdge3);

		//外積と法線ベクトルの内積を計算
		cross1 = XMVector3Dot(cross1, planeNormal);
		cross2 = XMVector3Dot(cross2, planeNormal);
		cross3 = XMVector3Dot(cross3, planeNormal);

		if (XMVectorGetY(cross1) > 0.0f && XMVectorGetY(cross2) > 0.0f && XMVectorGetY(cross3) > 0.0f)
		{
			//衝突している場合の処理
			pPoint->setDiffuse(1.0f, 0.0f, 0.0f, 1.0f);	//赤色に変更
		}
		else
		{
			//衝突していない場合の処理
			pPoint->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);	//白色に変更
		}
	}
	//線の描画
	vnDebugDraw::Line(&points[0], &points[1], 0xffffff00);

	//三角形の描画
	vnDebugDraw::Line(&points[2], &points[3], 0xffff00ff);
	vnDebugDraw::Line(&points[3], &points[4], 0xffff00ff);
	vnDebugDraw::Line(&points[4], &points[2], 0xffff00ff);

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
	vnFont::print(50.0f, 50.0f + (float)(line * 16), e[3][row] == eOperation::On ? 0xff00ffff : 0xffffffff, L"Tri[1]");
	vnFont::print(150.0f, 50.0f + (float)(line * 16), e[3][0] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetX(points[3]));
	vnFont::print(250.0f, 50.0f + (float)(line * 16), e[3][1] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetY(points[3]));
	vnFont::print(350.0f, 50.0f + (float)(line++ * 16), e[3][2] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetZ(points[3]));
	vnFont::print(50.0f, 50.0f + (float)(line * 16), e[4][row] == eOperation::On ? 0xff00ffff : 0xffffffff, L"Tri[2]");
	vnFont::print(150.0f, 50.0f + (float)(line * 16), e[4][0] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetX(points[4]));
	vnFont::print(250.0f, 50.0f + (float)(line * 16), e[4][1] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetY(points[4]));
	vnFont::print(350.0f, 50.0f + (float)(line++ * 16), e[4][2] == eOperation::On ? 0xff00ffff : 0xffffffff, L"%.3f", XMVectorGetZ(points[4]));
	line++;
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), 0xffffffff, L"操作方法");
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"↑↓ : 頂点の選択");
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"PageUp : 増加");
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"PageDown : 減少");
	line++;
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"t : %.3f", t);
	//vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"hitPoint : %.3f, %.3f, %.3f", XMVectorGetX(hit), XMVectorGetY(hit), XMVectorGetZ(hit));
	//vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"planeNormal : %.3f, %.3f, %.3f", XMVectorGetX(planeNormal), XMVectorGetY(planeNormal), XMVectorGetZ(planeNormal));
	//vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"cross1 : %.3f, %.3f, %.3f", XMVectorGetX(cross1), XMVectorGetY(cross1), XMVectorGetZ(cross1));
	//vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"cross2 : %.3f, %.3f, %.3f", XMVectorGetX(cross2), XMVectorGetY(cross2), XMVectorGetZ(cross2));
	//vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"cross3 : %.3f, %.3f, %.3f", XMVectorGetX(cross3), XMVectorGetY(cross3), XMVectorGetZ(cross3));

	vnDebugDraw::Grid();	//グリッドの描画
	vnDebugDraw::Axis();	//軸の描画

	vnScene::execute();
}

//描画関数
void SceneCollisionTest::render()
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

	pPoint->render();

	vnScene::render();
}
