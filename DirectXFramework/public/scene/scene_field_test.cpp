#include "../../framework.h"
#include "../../Framework/environment.h"

//初期化関数
bool SceneFieldTest::initialize()
{	
	//== 生成 ==
	pPlayer = new vnModel(L"data/model/bear/", L"bear.vnm");	//プレイヤー

	pShadow = new vnModel(L"data/model/", L"shadow.vnm");		//影
	pShadow->setTransparent(true);	//影の半透明を有効
	pShadow->setPositionY(0.01f);	//影の位置を少し浮かせる	Z-Fighting対策

	pCrown = new vnModel(L"data/model/", L"crown.vnm");			//王冠
	pCrown->setScale(0.3f, 0.3f, 0.3f);
	pCrown->setPositionY(2.8f);	//王冠の位置を少し浮かせる
	pCrown->setParent(pPlayer);	//王冠の親をプレイヤーに設定

	pNPC = new vnModel(L"data/model/", L"Megaspikan.vnm");		//NPC
	//pNPC = new vnModel(L"data/model/bear/", L"bear.vnm");		//NPC
	pNPC->setLighting(false);
	pNPC->setPosition(3.0f, 0.0f, 3.0f);
	//pNPC->setDiffuse(1.0f, 0.0f, 0.0f, 0.5f);
	//pNPC->setAmbient(1.0f, 0.0f, 0.0f, 1.0f);
	//pNPC->setSpecular(1.0f, 0.0f, 0.0f, 10.0f);

	//DIffuse : デフォルト(1, 1, 1, 1)	※wが半透明度
	//Ambient : デフォルト(0, 0, 0, 0)
	//Specular: デフォルト(0, 0, 0, 1)	※w（強度）は0以下にしない

	pGround = new vnModel(L"data/model/", L"ground.vnm");		//地面

	pSky = new vnModel(L"data/model/", L"skydome.vnm");			//背景
	pSky->setLighting(false);		//背景のライティングを無効

	pFan = new vnModel(L"data/model/", L"debug_fan.vnm");	//扇形オブジェクト
	pFan->setTransparent(true);	//半透明を有効
	pFan->setParent(pNPC);	//扇形の親をNPCに設定

	pBillboard = new vnBillboard(1.0f, 1.0f, L"data/image/icon_exc.png");
	pBillboard->setPositionY(5.0f);	//NPCの頭上に配置
	pBillboard->setParent(pNPC);	//ビルボードの親をNPCに設定

	for (int i = 0; i < FENCE_NUM; i++)
	{
		pFence[i] = new vnModel(L"data/model/", L"fence.vnm");	//フェンス

		float radian = 360.0f / (float)FENCE_NUM * i * 3.141592f / 180.0f;	//Y座標の角度を計算（ラジアン）

		//極座標 -> 直交座標
		float x = cos(radian) * FENCE_RADIUS;	//cosの計算
		float z = sin(radian) * FENCE_RADIUS;	//sinの計算

		pFence[i]->setPosition(x, 0.0f, z);				//位置を設定
		pFence[i]->setRotationY(-radian + XM_PIDIV2);		//角度を設定（XM_PIDIV2 -> π / 2 -> 90度のラジアン）
	}

	//シーンに登録
	registerObject(pPlayer);
	registerObject(pCrown);
	registerObject(pNPC);
	registerObject(pGround);
	registerObject(pSky);
	for (int i = 0; i < FENCE_NUM; i++)
	{
		registerObject(pFence[i]);
	}

	//== 半透明のオブジェクトは不透明の後に描画する ==
	registerObject(pFan);
	registerObject(pShadow);
	registerObject(pBillboard);

	//== 初期化 ==

	radius = 10.0f;	//半径
	theta = 0.0f;	//角度θ
	phi = 0.0f;		//角度φ

	reverseH = false;	//水平反転
	reverseV = false;	//垂直反転
	isInFan = false;

	return true;
}

//終了関数
void SceneFieldTest::terminate()
{
	//== 破棄 ==
	deleteObject(pPlayer);
	deleteObject(pCrown);
	deleteObject(pNPC);
	deleteObject(pGround);
	deleteObject(pSky);

	for (int i = 0; i < FENCE_NUM; i++)
	{
		deleteObject(pFence[i]);
	}
	deleteObject(pFan);
	deleteObject(pShadow);
	deleteObject(pBillboard);
}

//処理関数
void SceneFieldTest::execute()
{
#ifdef _DEBUG
	if (vnKeyboard::on(DIK_RETURN))
	{
		if (pCrown->getParent() == NULL)
		{
			pCrown->setParent(pPlayer);	//ビルボードの親をNPCに設定
		}
		else
		{
			XMVECTOR v;
			pCrown->getWorldPosition(&v);	//ワールド座標を取得
			pCrown->setParent(NULL);	//ビルボードの親を解除
			pCrown->setPosition(&v);	//ワールド座標をビルボードのローカル座標に設定
		}
	}
#endif

	//移動方向を示す方向ベクトル
	XMVECTOR vPMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR vNMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	bool isMovingForPlayer = false;	//プレイヤーの移動フラグ
	bool isMovingForNPC = false;	//NPCの移動フラグ

	if (vnKeyboard::on(DIK_W))
	{
		//pPlayer->addPositionZ(0.05f);
		vPMove = XMVectorSetZ(vPMove, 0.1f);
		isMovingForPlayer = true;	//移動中
	}
	if (vnKeyboard::on(DIK_S))
	{
		//pPlayer->addPositionZ(-0.05f);
		vPMove = XMVectorSetZ(vPMove, -0.1f);
		isMovingForPlayer = true;	//移動中
	}
	if (vnKeyboard::on(DIK_D))
	{
		//pPlayer->addPositionX(0.05f);
		vPMove = XMVectorSetX(vPMove, 0.1f);
		isMovingForPlayer = true;	//移動中
	}
	if (vnKeyboard::on(DIK_A))
	{
		//pPlayer->addPositionX(-0.05f);
		vPMove = XMVectorSetX(vPMove, -0.1f);
		isMovingForPlayer = true;	//移動中
	}

	if (vnKeyboard::on(DIK_UPARROW))
	{
		//pPlayer->addPositionZ(0.05f);
		vNMove = XMVectorSetZ(vNMove, 0.05f);
		isMovingForNPC = true;	//移動中
	}
	if (vnKeyboard::on(DIK_DOWNARROW))
	{
		//pPlayer->addPositionZ(-0.05f);
		vNMove = XMVectorSetZ(vNMove, -0.05f);
		isMovingForNPC = true;	//移動中
	}
	if (vnKeyboard::on(DIK_RIGHTARROW))
	{
		//pPlayer->addPositionX(0.05f);
		vNMove = XMVectorSetX(vNMove, 0.05f);
		isMovingForNPC = true;	//移動中
	}
	if (vnKeyboard::on(DIK_LEFTARROW))
	{
		//pPlayer->addPositionX(-0.05f);
		vNMove = XMVectorSetX(vNMove, -0.05f);
		isMovingForNPC = true;	//移動中
	}

	//プレイヤーの移動ベクトル（vMove）をカメラの角度（theta）に応じて回転
	XMMATRIX mrotate = XMMatrixRotationY(theta);

	//移動ベクトルに回転マトリクスを適用
	vPMove = XMVector3TransformNormal(vPMove, mrotate);
	vNMove = XMVector3TransformNormal(vNMove, mrotate);

	//プレイヤーの座標に移動ベクトルを加算して更新
	pPlayer->addPosition(&vPMove);
	pNPC->addPosition(&vNMove);

	pShadow->setPosition(pPlayer->getPosition());	//影の座標をプレイヤーの座標に合わせる
	pShadow->setPositionY(0.01f);	//影の座標を少し浮かせる	Z-Fighting対策

	//pFan->setPosition(pNPC->getPosition());	//扇形の座標をNPCの座標に合わせる
	//pFan->setRotation(pNPC->getRotation());	//扇形の角度をNPCの角度に合わせる

	//プレイヤーの角度を進行方向に向ける
	if (isMovingForPlayer)
	{
		float movePX = XMVectorGetX(vPMove);	//進行方向ベクトルのX成分
		float movePZ = XMVectorGetZ(vPMove);	//進行方向ベクトルのZ成分
		float rotPY = atan2f(movePX, movePZ);	//進行方向ベクトルのXとZからY軸の回転角度を計算

		//現在の角度(source) ; pPlayer->getRotation()
		//目的の角度(destination) ; rotPY
		XMMATRIX mSrc, mDst, mAns;
		XMVECTOR qSrc, qDst, qAns;

		//オイラー角からクオータニオンに変換（Y軸回転のみ）
		//qSrc = XMQuaternionRotationRollPitchYaw(0.0f, pPlayer->getPositionY(), 0.0f);
		//qDst = XMQuaternionRotationRollPitchYaw(0.0f, rotPY, 0.0f);

		//オイラー角をマトリクスに変換（Y軸回転のみ）
		mSrc = XMMatrixRotationY(pPlayer->getRotationY());
		mDst = XMMatrixRotationY(rotPY);

		//マトリクスをクオータニオンに変換
		qSrc = XMQuaternionRotationMatrix(mSrc);
		qDst = XMQuaternionRotationMatrix(mDst);

		//球面線形補間での中間の姿勢（角度）を計算
		qAns = XMQuaternionSlerp(qSrc, qDst, 0.1f);	//第三引数の数値が大きいほど速く回転する

		//クオータニオンをマトリクスに変換
		mAns = XMMatrixRotationQuaternion(qAns);

		//マトリクスかオイラー角(Y軸回転値)を計算
		float ry = atan2f(-XMVectorGetZ(mAns.r[0]), XMVectorGetZ(mAns.r[2]));

		//計算された中間角度を設定
		pPlayer->setRotationY(ry);	//プレイヤーのY軸の回転角度を更新
	}

	//NPCの角度を進行方向に向ける
	if (isMovingForNPC)
	{
		float moveX = XMVectorGetX(vNMove);	//進行方向ベクトルのX成分
		float moveZ = XMVectorGetZ(vNMove);	//進行方向ベクトルのZ成分
		float rotY = atan2f(moveX, moveZ);	//進行方向ベクトルのXとZからY軸の回転角度を計算
		pNPC->setRotationY(rotY);	//プレイヤーのY軸の回転角度を更新
	}

	//移動範囲のチェック

	//プレイヤーの原点からの距離取得
	XMVECTOR vDistance = XMVector3Length(*pPlayer->getPosition());
	float distance = XMVectorGetX(vDistance);

	if (distance > FIELD_RADIUS)
	{
		//はみ出している -> フェンスの内側に戻す
		XMVECTOR v = *pPlayer->getPosition();
		XMVECTOR normalize = XMVector3Normalize(v);
		//normalize *= FIELD_RADIUS;	//正規化したベクトルにフェンスの半径を掛ける
		v = XMVectorScale(normalize, FIELD_RADIUS);	//正規化したベクトルにフェンスの半径を掛ける
		pPlayer->setPosition(&v);	//プレイヤーの座標を更新
	}

	XMVECTOR sub = XMVectorSubtract(*pPlayer->getPosition(), *pNPC->getPosition());	//プレイヤーの座標とNPCの座標の差分ベクトルを計算
	XMVECTOR vLength = XMVector3Length(sub);	//差分ベクトルの長さを計算（距離）
	float len = XMVectorGetX(vLength);

	if (len < 2.0f)
	{
		//めり込んでいる -> プレイヤーの座標を戻す
		float rest = 2.0f - len;
		float half = rest / 2;	//めり込んでいる距離の半分を計算
		XMVECTOR normalize = XMVector3Normalize(sub);
		XMVECTOR newPlayerPos = XMVectorAdd(*pPlayer->getPosition(), XMVectorScale(normalize, half));	//Playerの座標から正規化した差分ベクトルに距離halfを掛けてプレイヤーの座標を計算
		XMVECTOR newNPCPos = XMVectorAdd(*pNPC->getPosition(), XMVectorScale(normalize, -half));	//NPCの座標から正規化した差分ベクトルに距離halfを掛けてNPCの座標を計算
		pPlayer->setPosition(&newPlayerPos);	//プレイヤーの座標を更新
		pNPC->setPosition(&newNPCPos);	//NCPの座標を更新
	}

	XMVECTOR vNPCtoPlayer = XMVectorSubtract(*pPlayer->getPosition(), *pNPC->getPosition());	//プレイヤーの座標と扇形の座標の差分ベクトルを計算
	XMVECTOR vDist = XMVector3Length(vNPCtoPlayer);	//差分ベクトルの長さを計算（距離）
	float dist = XMVectorGetX(vDist);
	float fanDistance = 10.0f;	//扇形の半径

	XMVECTOR vNPCFront = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);	//NPCの正面方向のベクトル
	XMMATRIX mNPCRotate = XMMatrixRotationY(pNPC->getRotationY());	//NPCの回転マトリクスを作成
	vNPCFront = XMVector3TransformNormal(vNPCFront, mNPCRotate);

	vNPCtoPlayer = XMVector3Normalize(vNPCtoPlayer);
	//vNPCFront = XMVector3Normalize(vNPCFront);

	XMVECTOR vDot = XMVector3Dot(vNPCtoPlayer, vNPCFront);	//内積を計算
	float dot = XMVectorGetX(vDot);
	if (dot > 1.0f)	//float型の精度によるノイズのカット
	{
		dot = 1.0f;
	}
	float radian = acosf(dot);	//内積から角度を計算（ラジアン）
	float degree = radian / 3.141592f * 180.0f;	//ラジアンを度に変換

	if (dist <= fanDistance && degree <= 45.0f)
	{
		isInFan = true;
		vNPCtoPlayer = XMVectorScale(vNPCtoPlayer, 0.05f);
		pNPC->addPosition(&vNPCtoPlayer);

		//プレイヤーの方を向く
		float x = XMVectorGetX(vNPCtoPlayer);
		float z = XMVectorGetZ(vNPCtoPlayer);
		float npcY = atan2f(x, z);
		pNPC->setRotationY(npcY);
		//pFan->setRotationY(pNPC->getRotationY());
		pBillboard->setRenderEnable(true);
		//pBillboard->setPosition(pNPC->getPosition());
		//pBillboard->setPositionY(5.0f);	//NPCの頭上に配置
	}
	else
	{
		isInFan = false;
		pBillboard->setRenderEnable(false);
	}


	//マウスの移動量を取得
	float dx = vnMouse::getDX();
	float dy = vnMouse::getDY();

	const float value_r = 0.5f * 3.141592f / 180.0f;	//ラジアン角度

	//角度の更新
	if (reverseH)
	{
		dx = -dx;	//水平反転
	}
	if (reverseV)
	{
		dy = -dy;	//垂直反転
	}

	theta += dx * value_r;
	phi += dy * value_r;

	//角度の制限
	if (phi > 1.5f)
	{
		phi = 1.5f;
	}
	else if (phi < -1.5f)
	{
		phi = -1.5f;
	}

	//== カメラ座標の計算 ==
	XMVECTOR camBias = XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f);	//カメラの位置のプレイヤーに対するオフセット
	XMVECTOR camTarget;	//カメラの注視点（プレイヤーの座標 + オフセット）
	XMVECTOR camPos;	//カメラの位置

	//回転していない基準となるベクトル
	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -radius, 0.0f);

	//回転マトリクスを作成
	XMMATRIX camRotate = XMMatrixRotationRollPitchYaw(phi, theta, 0.0f);

	//基準となるベクトルを回転させる
	eye = XMVector3TransformNormal(eye, camRotate);

	//回転の中心をプレイヤーにする（プレイヤーの座標を加算する）
	eye = XMVectorAdd(eye, *pPlayer->getPosition());

	camPos = XMVectorAdd(eye, camBias);

	//カメラの位置を更新
	vnCamera::setPosition(&camPos);

	//カメラの注視点を更新
	camTarget = XMVectorAdd(*pPlayer->getPosition(), camBias);

	vnCamera::setTarget(&camTarget);

	////極座標 -> 直交座標の変換
	//float cx = radius * cosf(theta) * cosf(phi);
	//float cy = radius * sinf(phi);
	//float cz = radius * sinf(theta) * cosf(phi);

	////回転の中心をプレイヤーにする（プレイヤーの座標を加算する）
	//cx += pPlayer->getPositionX();
	//cy += pPlayer->getPositionY();
	//cz += pPlayer->getPositionZ();

	////カメラの位置を更新
	//vnCamera::setPosition(cx, cy, cz);

	//ビルボードオブジェクト

	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	//NPCの座標をスクリーン座標に変換
	{
		XMVECTOR wPos;
		pNPC->getWorldPosition(&wPos);	//ワールド座標取得
		wPos = XMVectorAdd(wPos, XMVectorSet(0.0f, 5.0f, 0.0f, 0.0f));	//NPCの頭上に座標を移動

		//ワールド座標をビューポート座標を変換
		XMVECTOR viewport = XMVector3Transform(wPos, *vnCamera::getScreen());

		vnFont::print(0.0f, 600.0f, L"NPCのスクリーン座標 : %.3f, %.3f, %.3f, %.3f",
			XMVectorGetX(viewport),
			XMVectorGetY(viewport),
			XMVectorGetZ(viewport),
			XMVectorGetW(viewport)
		);
		
		//w除算
		float vx = XMVectorGetX(viewport);
		float vy = XMVectorGetY(viewport);
		float vz = XMVectorGetZ(viewport);
		float vw = XMVectorGetW(viewport);
		vx /= vw;
		vy /= vw;
		vz /= vw;
		vw /= vw;

		vnFont::print(0.0f, 620.0f, L"NPCのスクリーン座標 : %.3f, %.3f, %.3f, %.3f",
			vx,
			vy,
			vz,
			vw
		);

		float screenX = vx;	//-1.0f ~ 1.0fの範囲 -> 0.0f ~ 1280.0fの範囲に変換(vnMainframe::screenWidth)
		float screenY = vy;	//-1.0f ~ 1.0fの範囲 -> 0.0f ~ 720.0fの範囲に変換(vnMainframe::screenHeight)

		screenX = vnMainFrame::screenCenterX * vx + vnMainFrame::screenCenterX;
		screenY = -vnMainFrame::screenCenterY * vy + vnMainFrame::screenCenterY;

		vnFont::print(screenX, screenY, 16.0f, L"NPC");

		//vnFont::print(screenX, screenY, L"NPC : %.3f, %.3f", 
		//	screenX,
		//	screenY
		//);
	}

	vnFont::print(10.0f, 10.0f, L"PlayerPos: %.3f, %.3f, %.3f",
		pPlayer->getPositionX(),	//x座標
		pPlayer->getPositionY(), 	//y座標
		pPlayer->getPositionZ()		//z座標
	);

	vnFont::print(10.0f, 30.0f, L"NPCPos: %.3f, %.3f, %.3f",
		pNPC->getPositionX(),	//x座標
		pNPC->getPositionY(), 	//y座標
		pNPC->getPositionZ()	//z座標
	);

	vnFont::print(10.0f, 50.0f, L"PlayerRotY: %.3f",
		pPlayer->getRotationY()		//y軸の回転角度
	);

	vnFont::print(10.0f, 70.0f, L"プレイヤーの原点からの距離 : %.3f", distance);	//プレイヤーの原点からの距離
	vnFont::print(10.0f, 90.0f, L"プレイヤーとNPCの距離 : %.3f", len);				//プレイヤーとNPCの距離
	vnFont::print(10.0f, 110.0f, L"NPCの角度 : %.3f", degree);				//NPCの角度
	vnFont::print(10.0f, 130.0f, L"アイコンのローカル座標 : %.3f, %.3f, %.3f",
		pBillboard->getPositionX(),	//x座標
		pBillboard->getPositionY(),	//y座標
		pBillboard->getPositionZ()	//z座標
	);				//アイコンの位置

	XMVECTOR worldPos;
	pBillboard->getWorldPosition(&worldPos);

	vnFont::print(10.0f, 150.0f, L"アイコンのワールド座標 : %.3f, %.3f, %.3f",
		XMVectorGetX(worldPos),	//x座標
		XMVectorGetY(worldPos),	//y座標
		XMVectorGetZ(worldPos)	//z座標
	);				//アイコンの位置
	//vnFont::print(10.0f, 110.0f, L"扇形の中にいるかどうか : %s", isInFan ? L"true" : L"false");	//扇形の中にいるかどうか

	vnScene::execute();
}

//描画関数
void SceneFieldTest::render()
{
	////オブジェクトの描画
	//pPlayer->render();
	//pGround->render();
	//pSky->render();

	//for (int i = 0; i < FENCE_NUM; i++)
	//{
	//	pFence[i]->render();
	//}

	vnScene::render();
}
