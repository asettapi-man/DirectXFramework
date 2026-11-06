#include "../../framework.h"
#include "../../Framework/environment.h"

//初期化関数
bool ScenePickTest::initialize()
{
	vT.resize(pickObjectCount);

	for (int i = 0; i < pickObjectCount; i++)
	{
		vnModel* model = new vnModel(L"data/model/primitive/", L"sphere.vnm");
		model->setPosition(5.0f * (i + 1), 0.0f, 0.0f);
		pPickObj.push_back(model);

		XMVECTOR pos = *model->getPosition();
	}

	//極座標の初期値
	radius = 7.0f;	//半径
	theta = 0.0f;	//角度θ
	phi = 0.0f;		//角度φ

	for (int i = 0; i < pickObjectCount; i++)
	{
		registerObject(pPickObj[i]);
	}

	return true;
}

//終了関数
void ScenePickTest::terminate()
{
	delete pLight;

	for (int i = 0; i < pickObjectCount; i++)
	{
		deleteObject(pPickObj[i]);
	}
}

//処理関数
void ScenePickTest::execute()
{
	/*
	ローカル座標
	↓ワールドマトリクス
	ワールド座標
	↓ビューマトリクス
	カメラ座標
	↓プロジェクションマトリクス
	ビューポート座標
	↓ビューポート
	スクリーン座標（マウスカーソル　/　2D）
	*/

	float mx = (float)vnMouse::getX();
	float my = (float)vnMouse::getY();
	vnFont::print(10.0f, 10.0f, L"マウス座標 : %.3f, %.3f",
		mx,
		my
	);

	//マウス座標（スクリーン座標）-> ビューポート座標
	//X : 0(左）~ 1280（右） -> X : -1（左）~ +1（右）
	//X : 0(上）~ 1280（下） -> X : +1（上）~ -1（下）
	float viewportX = mx / (float)vnMainFrame::screenWidth * 2.0f - 1.0f;	//mx / (float)vnMainFrame::screenWidth = 0 ~ 2だから -1.0fを加えることで -1 ~ 1の範囲に変更できる
	float viewportY = -my / (float)vnMainFrame::screenHeight * 2.0f + 1.0f;
	vnFont::print(10.0f, 30.0f, L"ビューポート座標 : %.3f, %.3f",
		viewportX,
		viewportY
	);

	//マウス座標（ビューポート座標）をカメラのFarPlaneに置く（z = 1, w = 1として扱う）※同次座標
	XMVECTOR vMouse = XMVectorSet(viewportX, viewportY, 1.0f, 1.0f);

	//ビューポート座標 -> ワールド座標
	XMMATRIX vp = *vnCamera::getScreen();	//ビューポート座標にプロジェクションマトリクスを掛けたもの

	//逆行列
	//XMVECTOR* pDeterminant : 行列式（スカラー） 必要ないのであればNULLを入れる
	//0 ; 逆行列の計算に失敗した
	//行列式 : 変換前から変換後の体積比率
	XMMATRIX ivp = XMMatrixInverse(NULL, vp);

	//4x4成分の乗算
	XMVECTOR vWorld = XMVector3Transform(vMouse, ivp);

	vnFont::print(10.0f, 50.0f, L"ワールド座標（歪みあり） : %.3f, %.3f, %.3f, %.3f",
		XMVectorGetX(vWorld),
		XMVectorGetY(vWorld),
		XMVectorGetZ(vWorld),
		XMVectorGetW(vWorld)
	);

	//w除算(成分を1にする = ワールド空間の歪み / 拡大率が無い状態にする)
	float w = XMVectorGetW(vWorld);
	float x = XMVectorGetX(vWorld) / w;
	float y = XMVectorGetY(vWorld) / w;
	float z = XMVectorGetZ(vWorld) / w;
	w = w / w;

	XMVECTOR world = XMVectorSet(x, y, z, w);

	vnFont::print(10.0f, 70.0f, L"ワールド座標 : %.3f, %.3f, %.3f, %.3f",
		XMVectorGetX(world),
		XMVectorGetY(world),
		XMVectorGetZ(world),
		XMVectorGetW(world)
	);

	//直線の情報（カメラの座標から、FarPlaneにあるマウスカーソルに向かう直線）
	linePos = *vnCamera::getPosition();
	lineDir = world - linePos;
	lineDir = XMVector3Normalize(lineDir);

	for (int i = 0; i < pickObjectCount; i++)
	{
		//球の情報
		spherePos = *pPickObj[i]->getPosition();
		sphereRadius = 1.0f;

		//球の中心から最近傍距離を示す直線上の一点
		XMVECTOR v = XMVectorSubtract(spherePos, linePos);
		float t = XMVectorGetX(XMVector3Dot(v, lineDir));
		vT[i] = t;

		float result = vT[0];
		for (int i = 0; i < pickObjectCount; i++)
		{
			if (i == 0) continue;
			if (vT[i] > result && result < 0.0f) continue;
			result = vT[i];
		}

		XMVECTOR closestPoint = linePos + lineDir * result;	//最近傍点

		//最近傍点と球中心の距離
		XMVECTOR distance = XMVectorSubtract(closestPoint, spherePos);
		float dist = XMVectorGetX(XMVector3Length(distance));

		if (dist < sphereRadius)
		{
			pPickObj[i]->setDiffuse(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			pPickObj[i]->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	float value_t = 0.01f;
	float value_r = 1.0f * 3.141592f / 180.0f;
	float value_s = 0.01f;
	float value_l = 0.01f;

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
	//基本
	//vnFont::print(x, y, L"文字列");
	//						AARRGGBB <-- 色の指定(アルファ, 赤, 緑, 青)
	//vnFont::print(x, y, 0xffffffff, L"文字列");
	
	//書式設定
	//vnFont::print(x, y, L"%d", 10);

	//×改行コード
	//vnFont::print(x, y, L"文字列\n文字列", 10);
	//△フォント、文字サイズ -> scene_font_test

	//int line = 0;
	//vnFont::print(80.0f, (float)(100 + Cursor * 16), L"→");
	/*vnFont::print(150.0f, 50.0f + (float)(line * 16), e[column][0] == eOperation::On ? 0xff00ffff : 0xffffffff, L"PositionX");
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
	line++;
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), 0xffffffff, L"操作方法");
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"↑↓ : 頂点の選択");
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"PageUp : 増加");
	vnFont::print(50.0f, 50.0f + (float)(line++ * 16), L"PageDown : 減少");
	line++;
	*/
	vnDebugDraw::Grid();	//グリッドの描画
	vnDebugDraw::Axis();	//軸の描画

	vnScene::execute();
}

//描画関数
void ScenePickTest::render()
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

	vnScene::render();
}
