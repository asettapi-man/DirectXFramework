#include "../../framework.h"
#include "../../Framework/environment.h"

//初期化関数
bool SceneGroundTest::initialize()
{	
	//== 生成 ==
	pPlayer = new vnModel(L"data/model/bear/", L"bear.vnm");		//プレイヤー
	pGround = new vnModel(L"data/model/", L"overpass_000.vnm");	//地面
	pGround->setScale(2.0f, 1.0f, 2.0f);

	//シーンに登録
	registerObject(pPlayer);
	registerObject(pGround);

	//== 初期化 ==
	radius = 10.0f;	//半径
	theta = 0.0f;	//角度θ
	phi = 0.0f;		//角度φ

	reverseH = false;	//水平反転
	reverseV = false;	//垂直反転
	isHit = true;	//衝突判定
	isJump = false;	//

	velocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	gravity = XMVectorSet(0.0f, -0.01f, 0.0f, 0.0f);

	debugLineColor = 0xff00ff00;

	return true;
}

//終了関数
void SceneGroundTest::terminate()
{
	//== 破棄 ==
	deleteObject(pPlayer);
	deleteObject(pGround);
}

//処理関数
void SceneGroundTest::execute()
{
#ifdef _DEBUG
#endif

	//移動方向を示す方向ベクトル
	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	bool isMoving = false;	//プレイヤーの移動フラグ

	if (vnKeyboard::on(DIK_W))
	{
		//pPlayer->addPositionZ(0.05f);
		vMove = XMVectorSetZ(vMove, 0.1f);
		isMoving = true;	//移動中
	}
	if (vnKeyboard::on(DIK_S))
	{
		//pPlayer->addPositionZ(-0.05f);
		vMove = XMVectorSetZ(vMove, -0.1f);
		isMoving = true;	//移動中
	}
	if (vnKeyboard::on(DIK_D))
	{
		//pPlayer->addPositionX(0.05f);
		vMove = XMVectorSetX(vMove, 0.1f);
		isMoving = true;	//移動中
	}
	if (vnKeyboard::on(DIK_A))
	{
		//pPlayer->addPositionX(-0.05f);
		vMove = XMVectorSetX(vMove, -0.1f);
		isMoving = true;	//移動中
	}
	if (vnKeyboard::trg(DIK_SPACE) && !isJump)
	{
		isHit = false;
		isJump = true;
		velocity = XMVectorSet(0.0f, 0.3f, 0.0f, 0.0f);
	}

	//プレイヤーの移動ベクトル（vMove）をカメラの角度（theta）に応じて回転
	XMMATRIX mrotate = XMMatrixRotationY(theta);

	//移動ベクトルに回転マトリクスを適用
	vMove = XMVector3TransformNormal(vMove, mrotate);

	//前のプレイヤーの座標を記憶
	XMVECTOR oldPlayerPos = *pPlayer->getPosition();

	//プレイヤーの座標に移動ベクトルを加算して更新
	pPlayer->addPosition(&vMove);

	if (!isHit)
	{
		velocity += gravity;
	}
	else
	{
		velocity = XMVectorZero();
	}
	pPlayer->addPositionY(XMVectorGetY(velocity));

	if (XMVectorGetY(velocity) < 0.0f)
	{
		isJump = true;
	}

	//プレイヤーの角度を進行方向に向ける
	if (isMoving)
	{
		float movePX = XMVectorGetX(vMove);	//進行方向ベクトルのX成分
		float movePZ = XMVectorGetZ(vMove);	//進行方向ベクトルのZ成分
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

	//地面とプレイヤーの当たり判定
	//頂点配列の数
	int vtxNum = pGround->getVertexNum();

	//インデックス配列の数
	int idxNum = pGround->getIndexNum();

	//頂点配列
	vnVertex3D* vtx = pGround->getVertex();

	//インデックス配列
	LPWORD idx = pGround->getIndex();

	//②プレイヤーの移動先からrayを下方向に伸ばし、各３頂点の位置の差分
	//プレイヤーの下方向にrayを飛ばす
	XMVECTOR rayPos = *pPlayer->getPosition();
	rayPos += XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);	//足元から少し上に上げる
	XMVECTOR rayDir = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

	XMMATRIX world = *pGround->getWorld();

	for (int i = 0; i < idxNum; i+=3)
	{
		//各３頂点番号を取得
		int idx0 = idx[i + 0];
		int idx1 = idx[i + 1];
		int idx2 = idx[i + 2];

		//各３頂点の位置を取得
		XMVECTOR firstPoint = XMVectorSet(
			vtx[idx0].x,
			vtx[idx0].y, 
			vtx[idx0].z, 
			0.0f
		);	//vertex0

		XMVECTOR secondPoint = XMVectorSet(
			vtx[idx1].x,
			vtx[idx1].y,
			vtx[idx1].z,
			0.0f
		);	//vertex1

		XMVECTOR thirdPoint = XMVectorSet(
			vtx[idx2].x,
			vtx[idx2].y,
			vtx[idx2].z,
			0.0f
		);	//vertex2

		//ワールド座標に変換
		firstPoint = XMVector3TransformCoord(firstPoint, world);
		secondPoint = XMVector3TransformCoord(secondPoint, world);
		thirdPoint = XMVector3TransformCoord(thirdPoint, world);

		//平面（三角形）の情報
		XMVECTOR v1, v2;
		v1 = XMVectorSubtract(secondPoint, firstPoint);
		v2 = XMVectorSubtract(thirdPoint, firstPoint);

		planeNormal = XMVector3Cross(v1, v2);	//外積で法線ベクトルを求める
		planeNormal = XMVector3Normalize(planeNormal);	//正規化

		XMVECTOR vDot = XMVector3Dot(planeNormal, firstPoint);	//ある１頂点と法線ベクトルから内積を求める
		planeDistance = -XMVectorGetX(vDot);	//距離

		float s, d;
		XMVECTOR vS = XMVector3Dot(planeNormal, rayPos);
		s = XMVectorGetX(vS);
		s += planeDistance;

		XMVECTOR vD = XMVector3Dot(planeNormal, rayPos + rayDir);
		d = XMVectorGetX(vD);
		d += planeDistance;

		float t = s / (s - d);	//傾き
		float dif = s - d;	//floatの差

		if (fabsf(dif > 0.000001f) && (t >= 0 && t <= 0.3f))
		{
			XMVECTOR hit = rayPos + rayDir * t;

			//①各３頂点の距離を求める（0 -> 1, 1 -> 2, 2 -> 0）
			XMVECTOR firstEdge = XMVectorSubtract(secondPoint, firstPoint);
			XMVECTOR secondEdge = XMVectorSubtract(thirdPoint, secondPoint);
			XMVECTOR thirdEdge = XMVectorSubtract(firstPoint, thirdPoint);

			//rayと各３頂点の位置の差分
			XMVECTOR firstHitEdge = XMVectorSubtract(hit, firstPoint);
			XMVECTOR secondHitEdge = XMVectorSubtract(hit, secondPoint);
			XMVECTOR thirdHitEdge = XMVectorSubtract(hit, thirdPoint);

			//③　①と②の外積を求める（求めたy成分で触れているか判定する）
			XMVECTOR firstCross = XMVector3Cross(firstEdge, firstHitEdge);
			XMVECTOR secondCross = XMVector3Cross(secondEdge, secondHitEdge);
			XMVECTOR thirdCross = XMVector3Cross(thirdEdge, thirdHitEdge);

			//法線ベクトルとの内積を求める
			firstCross = XMVector3Dot(firstCross, planeNormal);
			secondCross = XMVector3Dot(secondCross, planeNormal);
			thirdCross = XMVector3Dot(thirdCross, planeNormal);

			float fC = XMVectorGetX(firstCross);
			float sC = XMVectorGetX(secondCross);
			float tC = XMVectorGetX(thirdCross);

			if (fC > 0.0f && sC > 0.0f && tC > 0.0f)
			{
				isHit = true;
				isJump = false;

				//プレイヤーの位置をコライダーの上に移動
				pPlayer->setPositionY(XMVectorGetY(hit));

				vnDebugDraw::Line(&firstPoint, &secondPoint, debugLineColor);
				vnDebugDraw::Line(&secondPoint, &thirdPoint, debugLineColor);
				vnDebugDraw::Line(&thirdPoint, &firstPoint, debugLineColor);
			}
			else
			{
				vnDebugDraw::Line(&firstPoint, &secondPoint, 0xff0000ff);
				vnDebugDraw::Line(&secondPoint, &thirdPoint, 0xff0000ff);
				vnDebugDraw::Line(&thirdPoint, &firstPoint, 0xff0000ff);
			}
		}

		XMVECTOR rayEndPos = rayPos + rayDir;
		vnDebugDraw::Line(&rayPos, &rayEndPos, 0xffff0000);

		//vnFont::print(10.0f, 70.0f + i * 60.0f, L"firstCross : %.3f", fC);
		//vnFont::print(10.0f, 90.0f + i * 60.0f, L"secondCross : %.3f", sC);
		//vnFont::print(10.0f, 110.0f + i * 60.0f, L"thirdCross : %.3f", tC);
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

	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	vnFont::print(10.0f, 10.0f, L"PlayerPos: %.3f, %.3f, %.3f",
		pPlayer->getPositionX(),	//x座標
		pPlayer->getPositionY(), 	//y座標
		pPlayer->getPositionZ()		//z座標
	);

	vnFont::print(10.0f, 50.0f, L"PlayerRotY: %.3f",
		pPlayer->getRotationY()		//y軸の回転角度
	);

	vnFont::print(10.0f, 70.0f, L"isHit : %s", isHit ? L"true" : L"false");

	vnScene::execute();
}

//描画関数
void SceneGroundTest::render()
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
