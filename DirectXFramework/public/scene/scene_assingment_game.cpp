#include "../../framework.h"
#include "../../Framework/environment.h"

//初期化関数
bool SceneAssingment::initialize()
{	
	//== 生成 ==
	pPlayer = new vnModel(L"data/model/bear/", L"bear.vnm");	//プレイヤー
	playerPos = XMFLOAT3(
		pPlayer->getPositionX(), 
		pPlayer->getPositionY() + pPlayer->getAABBCollider().getBoundingBox().Extents.y, 
		pPlayer->getPositionZ());	//プレイヤーの座標を取得

	pPlayer->getAABBCollider().initialize(playerPos, XMFLOAT3(0.5f, 1.3f, 0.5f));	//AABBの初期化

	pShadow = new vnModel(L"data/model/", L"shadow.vnm");		//影
	pShadow->setTransparent(true);	//影の半透明を有効
	pShadow->setPositionY(0.01f);	//影の位置を少し浮かせる	Z-Fighting対策
	pShadow->getAABBCollider().setEnable(false);	//影の当たり判定を無効にする

	//DIffuse : デフォルト(1, 1, 1, 1)	※wが半透明度
	//Ambient : デフォルト(0, 0, 0, 0)
	//Specular: デフォルト(0, 0, 0, 1)	※w（強度）は0以下にしない

	pGround = new vnModel(L"data/model/", L"ground.vnm");		//地面
	pGround->getAABBCollider().setEnable(false);	//影の当たり判定を無効にする

	pSky = new vnModel(L"data/model/", L"skydome.vnm");			//背景
	pSky->setLighting(false);		//背景のライティングを無効
	pSky->getAABBCollider().setEnable(false);	//影の当たり判定を無効にする

	pCheckerFlag = new vnModel(L"data/model/", L"CheckerFlag.vnm");	//ゴール
	pCheckerFlag->getAABBCollider().initialize(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 3.0f, 1.0f));	//当たり判定の初期化

	{
		//std::random_device rd;
		std::mt19937 mt(4/*rd()*/);
		int i;
		float x, z;
		float radius = 17.0f;
		for (i = 0; i < BLOCK_NUM - 1; i++)
		{
			pBlock[i] = new vnModel(L"data/model/primitive/", L"cube.vnm");	//ブロック
			float radian = 360.0f / (float)BLOCK_NUM * i * XM_PI / 180.0f;	//ラジアン角度

			//極座標 -> 直交座標
			x = cos(radian) * radius;
			z = sin(radian) * radius;

			pBlock[i]->setPosition(x, 3.0f * i + 1.0f, z);		//位置を設定

			//ブロックの大きさを乱数で設定
			std::uniform_real_distribution<float> scaleDist(0.5f, 1.5f);
			XMVECTOR blockScale = XMVectorSet(scaleDist(mt), scaleDist(mt), scaleDist(mt), 0.0f);
			pBlock[i]->setScale(&blockScale);	//ブロックのサイズ設定

			//ブロックの色を乱数で設定
			std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);
			XMVECTOR ambient = XMVectorSet(colorDist(mt), colorDist(mt), colorDist(mt), 1.0f);
			pBlock[i]->setAmbient(&ambient);

			blockPos = XMFLOAT3(pBlock[i]->getPositionX(), pBlock[i]->getPositionY(), pBlock[i]->getPositionZ());	//ブロックの座標を取得
			pBlock[i]->getAABBCollider().initialize(blockPos, XMFLOAT3(XMVectorGetX(blockScale), XMVectorGetY(blockScale), XMVectorGetZ(blockScale)));	//AABBの初期化

			if (i == 0) continue;
			if (i % 4 != 0) continue;

			//動く床の初期値追加
			vStartBlockPos.push_back(blockPos);
			isFlip.push_back(false);
		}

		//最後のブロックの位置
		pBlock[i] = new vnModel(L"data/model/primitive/", L"cube.vnm");	//ブロック
		float radian = 360.0f / (float)BLOCK_NUM * i * XM_PI / 180.0f;	//ラジアン角度

		//極座標 -> 直交座標
		x = cos(radian) * radius;
		z = sin(radian) * radius;

		pBlock[i]->setPosition(x, 3.0f * i + 1.0f, z);		//位置を設定

		//ブロックの色を乱数で設定
		std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);
		XMVECTOR ambient = XMVectorSet(colorDist(mt), colorDist(mt), colorDist(mt), 1.0f);
		pBlock[i]->setAmbient(&ambient);

		blockPos = XMFLOAT3(pBlock[i]->getPositionX(), pBlock[i]->getPositionY(), pBlock[i]->getPositionZ());	//ブロックの座標を取得
		XMVECTOR blockScale = XMVectorSet(3.0f, 1.0f, 3.0f, 0.0f);
		pBlock[i]->setScale(&blockScale);
		pBlock[i]->getAABBCollider().initialize(blockPos, XMFLOAT3(XMVectorGetX(blockScale), XMVectorGetY(blockScale), XMVectorGetZ(blockScale)));	//AABBの初期化

		//ゴールの位置
		pCheckerFlag->setPosition(x, 3.0f * i + 1.0f, z);
		goalPos = XMFLOAT3(
			pCheckerFlag->getPositionX(),
			pCheckerFlag->getPositionY() + pCheckerFlag->getAABBCollider().getBoundingBox().Extents.y,
			pCheckerFlag->getPositionZ()
		);
		pCheckerFlag->getAABBCollider().setBoundingBoxCenter(goalPos);
	}

	//== 画像 ==
	pTitle = new vnSprite(
		(float)vnMainFrame::screenCenterX,
		(float)vnMainFrame::screenCenterY,
		vnMainFrame::screenWidth,
		vnMainFrame::screenHeight,
		L"data/image/Title.png"
	);

	pStartButton = new vnSprite(
		(float)vnMainFrame::screenCenterX,
		(float)vnMainFrame::screenCenterY + 100.0f,
		400.0f,
		100.0f,
		L"data/image/startButton.png"
	);

	pQuitButton = new vnSprite(
		(float)vnMainFrame::screenCenterX,
		(float)vnMainFrame::screenCenterY + 250.0f,
		400.0f,
		100.0f,
		L"data/image/quitButton.png"
	);

	pTitleButton = new vnSprite(
		(float)vnMainFrame::screenCenterX,
		(float)vnMainFrame::screenCenterY + 250.0f,
		400.0f,
		100.0f,
		L"data/image/titleButton.png"
	);
	pTitleButton->setRenderEnable(false);	//非表示

	pDescriptionUI = new vnSprite(
		(float)vnMainFrame::screenWidth - 90.0f,
		(float)vnMainFrame::screenHeight - 70.0f,
		180.0f,
		60.0f,
		L""
	);
	pDescriptionUI->setRenderEnable(false);	//非表示

	for (int i = 0; i < vnSprite::VertexNum; i++)
	{
		pDescriptionUI->vtx[i].r = 0.0f;
		pDescriptionUI->vtx[i].g = 0.0f;
		pDescriptionUI->vtx[i].b = 0.0f;
		pDescriptionUI->vtx[i].a = 0.5f;
	}

	pBGM = new vnSound(L"data/sound/bgm.wav");	//BGM
	pClickSE = new vnSound(L"data/sound/cursor2.wav");	//クリック時の効果音
	pClearSE = new vnSound(L"data/sound/clear.wav");	//クリア時の効果音
	pJumpSE = new vnSound(L"data/sound/jump.wav");	//ジャンプ時の効果音

	//シーンに登録
	registerObject(pPlayer);
	registerObject(pGround);
	registerObject(pSky);
	registerObject(pCheckerFlag);
	for (int i = 0; i < BLOCK_NUM; i++)
	{
		registerObject(pBlock[i]);
	}

	//== 半透明のオブジェクトは不透明の後に描画する ==
	registerObject(pShadow);

	//== 画像 ==
	registerObject(pTitle);
	registerObject(pStartButton);
	registerObject(pQuitButton);
	registerObject(pTitleButton);
	registerObject(pDescriptionUI);

	//== 初期化 ==

	radius = 10.0f;	//半径
	theta = 0.0f;	//角度θ
	phi = 0.0f;		//角度φ

	reverseH = false;	//水平反転
	reverseV = false;	//垂直反転

	velocity = XMVectorZero();	//ベクトルaの初期化
	jumpVec = XMVectorSet(0.0f, 0.3f, 0.0f, 0.0f);	//ベクトルbの初期化
	gravity = XMVectorSet(0.0f, -0.01f, 0.0f, 0.0f);	//重力ベクトルの初期化

	isControllable = true;	//操作可能フラグの初期化
	isJump = true;	//ジャンプ可能フラグの初期化

	gameState = eGameState::TITLE;	//ゲームステートの初期化
	direction = eDirection::NONE;	//衝突方向の初期化

	return true;
}

//終了関数
void SceneAssingment::terminate()
{
	//== 破棄 ==
	deleteObject(pPlayer);
	deleteObject(pGround);
	deleteObject(pSky);
	deleteObject(pCheckerFlag);
	for (int i = 0; i < BLOCK_NUM; i++)
	{
		deleteObject(pBlock[i]);
	}

	deleteObject(pShadow);

	//== 画像 ==
	deleteObject(pTitle);
	deleteObject(pStartButton);
	deleteObject(pQuitButton);
	deleteObject(pTitleButton);
	deleteObject(pDescriptionUI);

	//== 効果音 ==
	delete(pClickSE);
	delete(pClearSE);
	delete(pJumpSE);
}

//処理関数
void SceneAssingment::execute()
{
	if (!pBGM->isPlaying())
	{
		pBGM->play();
	}

	switch (gameState)
	{
	case eGameState::TITLE:
	{
		if (CheckImageMouseOver(pStartButton))
		{
			//マウスオーバー時の色変更
			MouseOverImageColor(pStartButton);

			//左クリック時
			if (vnMouse::trgL())
			{
				pClickSE->play();
				TitleSetUp();

				//表示
				pDescriptionUI->setRenderEnable(true);

				//非表示状態
				pTitle->setRenderEnable(false);
				pStartButton->setRenderEnable(false);
				pQuitButton->setRenderEnable(false);

				ShowCursor(false);	//マウスカーソルの非表示

				gameState = eGameState::GAME;
			}
		}
		else
		{
			ImageColor(pStartButton);
		}

		//終了ボタン
		if (CheckImageMouseOver(pQuitButton))
		{
			//マウスオーバー時の色変更
			MouseOverImageColor(pQuitButton);

			//左クリック時
			if (vnMouse::trgL())
			{
				PostQuitMessage(0);	//アプリケーション終了
			}
		}
		else
		{
			ImageColor(pQuitButton);
		}
		break;
	}
	case eGameState::GAME:
		vnFont::setColor(0xffffb84a);

		if (isControllable)
		{
			bool isMovingForPlayer = false;	//プレイヤーの移動フラグ
			//移動方向を示す方向ベクトル
			vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			if (vnKeyboard::on(DIK_W))
			{
				vMove = XMVectorSetZ(vMove, 0.1f);
				isMovingForPlayer = true;	//移動中
			}
			if (vnKeyboard::on(DIK_S))
			{
				vMove = XMVectorSetZ(vMove, -0.1f);
				isMovingForPlayer = true;	//移動中
			}
			if (vnKeyboard::on(DIK_D))
			{
				vMove = XMVectorSetX(vMove, 0.1f);
				isMovingForPlayer = true;	//移動中
			}
			if (vnKeyboard::on(DIK_A))
			{
				vMove = XMVectorSetX(vMove, -0.1f);
				isMovingForPlayer = true;	//移動中
			}

			//ジャンプ処理
			if (vnKeyboard::trg(DIK_SPACE) && isJump)
			{
				pJumpSE->play();
				isJump = false;	//ジャンプ中はジャンプできない
				velocity = jumpVec;
			}

#ifdef _DEBUG
			if (vnKeyboard::on(DIK_UP))
			{
				vMove = XMVectorSetY(vMove, 10.0f);
			}

			{
				vnFont::print(10.0f, 10.0f, L"PlayerPos: %.3f, %.3f, %.3f",
					pPlayer->getPositionX(),	//x座標
					pPlayer->getPositionY(), 	//y座標
					pPlayer->getPositionZ()		//z座標
				);

				vnFont::print(10.0f, 50.0f, L"PlayerRotY: %.3f",
					pPlayer->getRotationY()		//y軸の回転角度
				);

				BoundingBox& playerBox = pPlayer->getAABBCollider().getBoundingBox();
				vnFont::print(10.0f, 70.0f, L"PlayerCollider: %.3f, %.3f, %.3f",
					playerBox.Center.x,	//AABBの中心座標X
					playerBox.Center.y,	//AABBの中心座標Y
					playerBox.Center.z	//AABBの中心座標Z
				);

				vnFont::print(10.0f, 90.0f, L"Jump: %s", isJump ? L"true" : L"false");

				vnFont::print(10.0f, 110.0f, L"vMove : %.3f, %.3f, %.3f",
					XMVectorGetX(vMove),
					XMVectorGetY(vMove),
					XMVectorGetZ(vMove)
				);

				vnFont::print(10.0f, 130.0f, L"velocity : %.3f, %.3f, %.3f",
					XMVectorGetX(velocity),
					XMVectorGetY(velocity),
					XMVectorGetZ(velocity)
				);

				vnFont::print(10.0f, 150.0f, L"pCheckFlagPos : %.3f, %.3f, %.3f",
					pCheckerFlag->getPositionX(),
					pCheckerFlag->getPositionY(),
					pCheckerFlag->getPositionZ()
				);

				BoundingBox& goalPos = pCheckerFlag->getAABBCollider().getBoundingBox();
				vnFont::print(10.0f, 170.0f, L"pCheckFlagColliderPos : %.3f, %.3f, %.3f",
					goalPos.Center.x,
					goalPos.Center.y,
					goalPos.Center.z
				);

				vnFont::print(10.0f, 190.0f, L"isControllable : %s", isControllable ? L"true" : L"false");

				vnFont::print(10.0f, 210.0f, L"CameraPos : %.3f, %.3f, %.3f",
					vnCamera::getPositionX(),
					vnCamera::getPositionY(),
					vnCamera::getPositionZ()
				);

				for (int i = 0; i < BLOCK_NUM; i++)
				{
					BoundingBox& blockBox = pBlock[i]->getAABBCollider().getBoundingBox();
					vnFont::print(10.0f, 250.0f + i * 20.0f, L"BlockCollider%d: %.3f, %.3f, %.3f",
						i,
						blockBox.Center.x,	//AABBの中心座標X
						blockBox.Center.y,	//AABBの中心座標Y
						blockBox.Center.z	//AABBの中心座標Z
					);
				}
			}
#endif
			//落下処理
			velocity += gravity;
			pPlayer->addPositionY(XMVectorGetY(velocity));

			if (XMVectorGetY(velocity) < 0.0f)
			{
				//落下中はジャンプ不可
				isJump = false;
			}

			//プレイヤーの移動ベクトル（vMove）をカメラの角度（theta）に応じて回転
			XMMATRIX mrotate = XMMatrixRotationY(theta);

			//移動ベクトルに回転マトリクスを適用
			vMove = XMVector3TransformNormal(vMove, mrotate);

			//プレイヤーの座標に移動ベクトルを加算して更新
			pPlayer->addPosition(&vMove);

			pShadow->setPosition(pPlayer->getPosition());	//影の座標をプレイヤーの座標に合わせる
			pShadow->setPositionY(0.01f);	//影の座標を少し浮かせる	Z-Fighting対策

			//プレイヤーの角度を進行方向に向ける
			if (isMovingForPlayer)
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

			//動く床の作成
			int n = 0;
			for (int i = 0; i < BLOCK_NUM - 1; i++)
			{
				if (i == 0) continue;
				if (i % 4 != 0) continue;

				MoveToTarget(pBlock[i], n);
				n++;
			}

			//AABBの中心座標を設定
			playerPos = XMFLOAT3(
				pPlayer->getPositionX(),
				pPlayer->getPositionY() + pPlayer->getAABBCollider().getBoundingBox().Extents.y,
				pPlayer->getPositionZ());	//プレイヤーの座標を取得

			pPlayer->getAABBCollider().setBoundingBoxCenter(playerPos);	//AABBの中心座標を設定

			//ブロックの当たり判定
			for (int i = 0; i < BLOCK_NUM; i++)
			{
				if (CheckBoundingBoxCollision(pPlayer, pBlock[i]))
				{
					OutputDebugStringW(L"衝突\n");	//デバッグ文

					if (direction != eDirection::Y_TOP)
					{
						//オブジェクトの上部から落ちたらジャンプ不可
						isJump = false;
					}
				}
			}

			//ゴールフラグとの当たり判定
			if (CheckBoundingBoxCollision(pPlayer, pCheckerFlag))
			{
				pClearSE->play();
				isControllable = false;
				pTitleButton->setRenderEnable(true);	//表示
				ShowCursor(true);
				SetCursorPos(vnMainFrame::screenCenterX, vnMainFrame::screenCenterY);

				gameState = eGameState::RESULT;
			}

			//地面判定
			if (pPlayer->getPositionY() < 0.0f)
			{
				pPlayer->setPositionY(0.0f);
				velocity = XMVectorZero();
				isJump = true;	//地面に着地したらジャンプ可能
			}

			//AABBの中心座標を再設定
			playerPos = XMFLOAT3(
				pPlayer->getPositionX(),
				pPlayer->getPositionY() + pPlayer->getAABBCollider().getBoundingBox().Extents.y,
				pPlayer->getPositionZ());	//プレイヤーの座標を取得

			pPlayer->getAABBCollider().setBoundingBoxCenter(playerPos);	//AABBの中心座標を設定

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
		}

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

		vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 100.0f, L"操作説明");
		vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 80.0f, L"移動 : W A S D");
		vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 60.0f, isJump ? 0xffffb84a : 0x7fffb84a, L"ジャンプ : Space");

		vnScene::execute();
		break;

	case eGameState::RESULT:
		vnFont::setColor(0xffffb84a);
		vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 100.0f, L"操作説明");
		vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 80.0f, L"移動 : W A S D");
		vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 60.0f, isJump ? 0xffffb84a : 0x7fffb84a, L"ジャンプ : Space");

		vnFont::setColor(0xffff0000);
		vnFont::print((float)vnMainFrame::screenCenterX, (float)vnMainFrame::screenCenterY, L"ゴール！");

		if (CheckImageMouseOver(pTitleButton))
		{
			MouseOverImageColor(pTitleButton);

			if (vnMouse::trgL())
			{
				//左クリック時
				pClickSE->play();

				//表示
				pTitle->setRenderEnable(true);
				pStartButton->setRenderEnable(true);
				pQuitButton->setRenderEnable(true);

				//非表示
				pTitleButton->setRenderEnable(false);	
				pDescriptionUI->setRenderEnable(false);
				gameState = eGameState::TITLE;
			}
		}
		else
		{
			ImageColor(pTitleButton);
		}
		break;
	}
}

//描画関数
void SceneAssingment::render()
{
	vnScene::render();
}

void SceneAssingment::TitleSetUp()
{
	//プレイヤーの初期化
	pPlayer->setPosition(0.0f, 0.0f, 0.0f);	//位置
	pPlayer->setRotationY(0.0f);	//Y軸回転

	//カメラの初期化
	theta = 0.0f;	//角度θ
	phi = 0.0f;		//角度φ

	isControllable = true;
}

//箱の横移動処理
void SceneAssingment::MoveToTarget(vnModel* pBlock, int n)
{
	float startPos = vStartBlockPos[n].z - 3.0f;
	float endPos = vStartBlockPos[n].z + 3.0f;
	float targetPos = isFlip[n] ? startPos : endPos;

	//自身の位置から目的地までの差分
	XMVECTOR direction = XMVectorSubtract(
		XMVectorSet(0.0f, 0.0f, targetPos, 0.0f),
		XMVectorSet(0.0f, 0.0f, pBlock->getPositionZ(), 0.0f)
	);

	//正規化して方向ベクトルだけを取得
	direction = XMVector3Normalize(direction);

	if (XMVectorGetZ(direction) > 0.1f)
	{
		pBlock->addPositionZ(0.03f);
	}
	else
	{
		pBlock->addPositionZ(-0.03f);
	}

	//目的地までの距離を求める
	XMVECTOR distance = XMVectorSubtract(
		XMVectorSet(0.0f, 0.0f, targetPos, 0.0f),
		XMVectorSet(0.0f, 0.0f, pBlock->getPositionZ(), 0.0f)
	);

	float dis = XMVectorGetZ(XMVector3Length(distance));

	if (dis < 0.1f)
	{
		pBlock->setPositionZ(targetPos);
		isFlip[n] = !isFlip[n];
		targetPos = isFlip[n] ? startPos : endPos;
	}

	//当たり判定の再設定
	pBlock->getAABBCollider().setBoundingBoxCenter(
		XMFLOAT3(
			pBlock->getPositionX(),
			pBlock->getPositionY(),
			pBlock->getPositionZ()
		)
	);
}

/// <summary>
/// マウスポインタが画像の範囲内か判定
/// </summary>
/// <param name="sprite">画像</param>
/// <returns></returns>
bool SceneAssingment::CheckImageMouseOver(vnSprite* sprite)
{
	//マウスのポインタ位置
	float dx = vnMouse::getX();
	float dy = vnMouse::getY();

	//画像サイズの半分を取得
	float halfSizeX = sprite->sizeX / 2.0f;
	float halfSizeY = sprite->sizeY / 2.0f;

	//四つ角の位置を計算
	float left = sprite->posX - halfSizeX;
	float right = sprite->posX + halfSizeX;
	float top = sprite->posY - halfSizeY;
	float bottom = sprite->posY + halfSizeY;

	if ((dx >= left && dx <= right) && (dy >= top && dy <= bottom))
	{
		return true;
	}

	return false;
}

/// <summary>
/// マウスオーバー時の色変更
/// </summary>
/// <param name="sprite">画像</param>
/// <param name="r">赤色の度合</param>
/// <param name="g">緑色の度合</param>
/// <param name="b">青色の度合</param>
void SceneAssingment::MouseOverImageColor(vnSprite* sprite, float r, float g, float b)
{
	for (int i = 0; i < vnSprite::VertexNum; i++)
	{
		sprite->vtx[i].r = r;
		sprite->vtx[i].g = g;
		sprite->vtx[i].b = b;
	}
}

/// <summary>
/// 通常時の色設定
/// </summary>
/// <param name="sprite">画像</param>
void SceneAssingment::ImageColor(vnSprite* sprite)
{
	for (int i = 0; i < vnSprite::VertexNum; i++)
	{
		sprite->vtx[i].r = 1.0f;
		sprite->vtx[i].g = 1.0f;
		sprite->vtx[i].b = 1.0f;
	}
}

//== AABBの当たり判定 ==
bool SceneAssingment::CheckBoundingBoxCollision(vnModel* m1, vnModel* m2)
{
	//AABBの取得
	AABBCollider& m1Box = m1->getAABBCollider();
	AABBCollider& m2Box = m2->getAABBCollider();

	//BoundingBoxの取得
	BoundingBox& m1BoundingBox = m1Box.getBoundingBox();
	BoundingBox& m2BoundingBox = m2Box.getBoundingBox();

	if (m1Box.Intersects(m2Box))
	{
		//x, y, zの重なりの大きさを計算
		float overlapX = m1BoundingBox.Extents.x + m2BoundingBox.Extents.x - abs(m1BoundingBox.Center.x - m2BoundingBox.Center.x);
		float overlapY = m1BoundingBox.Extents.y + m2BoundingBox.Extents.y - abs(m1BoundingBox.Center.y - m2BoundingBox.Center.y);
		float overlapZ = m1BoundingBox.Extents.z + m2BoundingBox.Extents.z - abs(m1BoundingBox.Center.z - m2BoundingBox.Center.z);

		//プレイヤーのx, z軸への移動成分
		float playerMoveX = 0.0f;// XMVectorGetX(vMove);
		float playerMoveZ = 0.0f;// XMVectorGetZ(vMove);
		if (overlapX < overlapY)
		{
			if (overlapX < overlapZ)
			{
				//X方向の衝突処理
				if (m1BoundingBox.Center.x > m2BoundingBox.Center.x)
				{
					//m1がm2より右にある場合
					direction = eDirection::X_RIGHT;
					if (playerMoveX < 0.0f)
					{
						m1->addPositionX(-playerMoveX);
					}
					m1->addPositionX(overlapX);
				}
				else
				{
					//m1がm2より左にある場合
					direction = eDirection::X_LEFT;
					if (playerMoveX > 0.0f)
					{
						m1->addPositionX(-playerMoveX);
					}
					m1->addPositionX(-overlapX);
				}
			}
			else
			{
				//Z方向の衝突処理
				if (m1BoundingBox.Center.z > m2BoundingBox.Center.z)
				{
					//m1がm2より手前にある場合
					direction = eDirection::Z_FRONT;
					if (playerMoveZ < 0.0f)
					{
						m1->addPositionZ(-playerMoveZ);
					}
					m1->addPositionZ(overlapZ);
				}
				else
				{
					//m1がm2より後ろにある場合
					direction = eDirection::Z_BACK;
					if (playerMoveZ > 0.0f)
					{
						m1->addPositionZ(-playerMoveZ);
					}
					m1->addPositionZ(-overlapZ);
				}
			}
		}
		else
		{
			if (overlapY < overlapZ)
			{
				//Y方向の衝突処理
				if (m1BoundingBox.Center.y > m2BoundingBox.Center.y)
				{
					//m1がm2より上にある場合
					direction = eDirection::Y_TOP;
					m1->addPositionY(overlapY);
					if (XMVectorGetY(velocity) < 0.0f)
					{
						velocity = XMVectorZero();	//速度ベクトルをリセット
					}
					isJump = true;	//ジャンプ可能フラグを有効にする
				}
				else
				{
					//m1がm2より下にある場合
					direction = eDirection::Y_DOWN;
					m1->addPositionY(-overlapY);
					velocity = XMVectorZero();	//速度ベクトルをリセット
				}
			}
			else
			{
				//Z方向の衝突処理
				if (m1BoundingBox.Center.z > m2BoundingBox.Center.z)
				{
					//m1がm2より手前にある場合
					direction = eDirection::Z_FRONT;
					if (playerMoveZ < 0.0f)
					{
						m1->addPositionZ(-playerMoveZ);
					}
					m1->addPositionZ(overlapZ);
				}
				else
				{
					//m1がm2より後ろにある場合
					direction = eDirection::Z_BACK;
					if (playerMoveZ > 0.0f)
					{
						m1->addPositionZ(-playerMoveZ);
					}
					m1->addPositionZ(-overlapZ);
				}
			}
		}
		return true;
	}
	else
	{
		direction = eDirection::NONE;
		return false;
	}
}
