#include "../../framework.h"
#include "../../Framework/environment.h"
#include "../../Public/time_manager.h"

//初期化関数
bool SceneMainGame::initialize()
{	
	//== 生成 ==
	player = new Player(L"data/model/bear/", L"bear.vnm", XMFLOAT3(0.0f, 1.3f, 0.0f), XMFLOAT3(0.5f, 1.3f, 0.5f));
	player->Initialize();

	crosshairUI = new CrosshairUI();

	inventoryUI = new InventoryUI();
	inventoryUI->Initialize();

	pShadow = new vnModel(L"data/model/", L"shadow.vnm");		//影
	pShadow->setTransparent(true);	//影の半透明を有効
	pShadow->setPositionY(0.01f);	//影の位置を少し浮かせる	Z-Fighting対策

	//DIffuse : デフォルト(1, 1, 1, 1)	※wが半透明度
	//Ambient : デフォルト(0, 0, 0, 0)
	//Specular: デフォルト(0, 0, 0, 1)	※w（強度）は0以下にしない

	pGround = new vnModel(L"data/model/", L"ground.vnm");		//地面

	pSky = new vnModel(L"data/model/", L"skydome.vnm");			//背景
	pSky->setLighting(false);		//背景のライティングを無効

	for (int i = 0; i < ISLANDCOUNT; i++)
	{
		vnModel* model = new vnModel(L"data/model/Island/MiningIsland/", L"MiningIsland.vnm");
		islands.push_back(model);
		islands[i]->setPosition(50.0f * i, 0.0f, 0.0f);
	}

	for (int i = 0; i < INITMATERIALCOUNT; i++)
	{
		Material* material = new Material(MaterialType::WOOD);
		materials.push_back(material);
		materials[i]->GetModel()->setPosition(0.0f, 10.0f, 10.0f * (i + 1));
		materials[i]->GetAABBCollider().SetBoundingBoxCenter(
			materials[i]->GetModel()->getPositionX(),
			materials[i]->GetModel()->getPositionY(),
			materials[i]->GetModel()->getPositionZ()
		);

		materials[i]->Initialize();
	}

	//== 画像 ==
	/*pTitle = new vnSprite(
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
	*/
	//シーンに登録
	registerObject(player->GetModel());
	registerObject(pGround);
	registerObject(pSky);

	for (int i = 0; i < ISLANDCOUNT; i++)
	{
		registerObject(islands[i]);
	}

	for (int i = 0; i < INITMATERIALCOUNT; i++)
	{
		registerObject(materials[i]->GetModel());
	}

	//== 半透明のオブジェクトは不透明の後に描画する ==
	registerObject(pShadow);

	//== 画像 ==
	registerObject(crosshairUI->GetNormalCrosshair());
	registerObject(crosshairUI->GetActionCrosshair());

	crosshairUI->GetActionCrosshair()->setRenderEnable(false);

	/*registerObject(pTitle);
	registerObject(pStartButton);
	registerObject(pQuitButton);
	registerObject(pTitleButton);
	registerObject(pDescriptionUI);
	*/

	//== 初期化 ==
	linePos = XMVectorZero();	//Rayの位置
	lineDir = XMVectorZero();	//Rayの方向
	pickupRange = 4.0f;			//Rayの長さ

	holdRange = 4.0f;			//資材保持距離

	radius = 10.0f;	//半径
	theta = 0.0f;	//角度θ
	phi = 0.0f;		//角度φ

	reverseH = false;	//水平反転
	reverseV = false;	//垂直反転

	isControllable = true;	//操作可能フラグ
	isAddMaterial = true;	//資材追加可能フラグ

	gameState = eGameState::GAME;	//ゲームステートの初期化
	return true;
}

//終了関数
void SceneMainGame::terminate()
{
	deleteObject(player->GetModel());
	deleteObject(pGround);
	deleteObject(pSky);

	for (int i = 0; i < ISLANDCOUNT; i++)
	{
		deleteObject(islands[i]);
	}

	for (std::size_t i = 0; i < materials.size(); i++)
	{
		deleteObject(materials[i]->GetModel());
	}

	deleteObject(pShadow);

	//== 破棄 ==
	delete player;

	//== 画像 ==
	deleteObject(crosshairUI->GetNormalCrosshair());
	deleteObject(crosshairUI->GetActionCrosshair());

	/*deleteObject(pTitle);
	deleteObject(pStartButton);
	deleteObject(pQuitButton);
	deleteObject(pTitleButton);
	deleteObject(pDescriptionUI);

	//== 効果音 ==
	delete(pClickSE);
	delete(pClearSE);
	delete(pJumpSE);
	*/
}

//処理関数
void SceneMainGame::execute()
{
	player->Execute(TimeManager::GetDeltaTime(), theta);
	inventoryUI->Execute();
	//pShadow->setPosition(player->getModel()->getPosition());	//影の座標をプレイヤーの座標に合わせる
	pShadow->setPositionY(0.01f);	//影の座標を少し浮かせる	Z-Fighting対策

	float hitDistance = 0.0f;	//衝突点からカメラの位置までの距離を返す
	crosshairUI->UpdateCrosshairUI(false);

	inventoryUI->SetIsInventoryUIRender(vnKeyboard::on(DIK_TAB));

	//インベントリ画面表示中であれば視点移動をキャンセル
	if (!inventoryUI->GetIsInventoryUIRender())
	{
		//マウスの移動量を取得
		float dx = (float)vnMouse::getDX();
		float dy = (float)vnMouse::getDY();

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
	}

	//== １人称カメラ座標の計算 ==
	XMVECTOR camBias = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);	//カメラの位置のプレイヤーに対するオフセット
	XMVECTOR camTarget;	//カメラの注視点（プレイヤーの座標 + オフセット）
	XMVECTOR camPos;	//カメラの位置

	//回転していない基準となるベクトル
	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -0.01f, 0.0f);

	//回転マトリクスを作成
	XMMATRIX camRotate = XMMatrixRotationRollPitchYaw(phi, theta, 0.0f);

	//基準となるベクトルを回転させる
	eye = XMVector3TransformNormal(eye, camRotate);

	//回転の中心をプレイヤーにする（プレイヤーの座標を加算する）
	eye = XMVectorAdd(eye, *player->GetModel()->getPosition());

	camPos = XMVectorAdd(eye, camBias);

	//カメラの位置を更新
	vnCamera::setPosition(&camPos);

	//カメラの注視点を更新
	camTarget = XMVectorAdd(*player->GetModel()->getPosition(), camBias);

	vnCamera::setTarget(&camTarget);

	XMVECTOR cameraForward = XMVector3Normalize(XMVectorSubtract(camTarget, camPos));	//カメラが見ている方向ベクトル

	linePos = camPos;			//始点
	lineDir = cameraForward;	//方向

	XMVECTOR holdPos = linePos + lineDir * holdRange;

	for (std::size_t i = 0; i < materials.size(); i++)
	{
		materials[i]->Execute(TimeManager::GetDeltaTime(), holdPos);

		//資材クラスが所持しているAABBColliderとの距離を確認
		if (materials[i]->GetAABBCollider().GetBoundingBox().Intersects(
				linePos,
				lineDir,
				hitDistance)
			&& hitDistance <= pickupRange)
		{
			crosshairUI->UpdateCrosshairUI(true);

			Material* targetMat = materials[i];	//衝突対象の資材

			if (vnKeyboard::trg(DIK_E))
			{
				//Eキー押下でインベントリに資材追加
				isAddMaterial = player->AddMaterial(targetMat);
				if (isAddMaterial)
				{
					DeleteMaterialData(targetMat);
					break;
				}
			}

			if (vnMouse::onL())
			{
				targetMat->SetMaterialState(MaterialState::HOLD);
				isDebug = true;
			}
			else
			{
				targetMat->SetMaterialState(MaterialState::NONE);
				isDebug = false;
			}
		}
	}

	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 100.0f, L"操作説明");
	vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 80.0f, L"移動 : W A S D");
	vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 60.0f, player->GetIsJump() ? 0x7fffb84a : 0xffffb84a, L"ジャンプ : Space");
	
	pSky->setPosition(vnCamera::getPosition());

	vnScene::execute();

	/*if (!pBGM->isPlaying())
	{
		pBGM->play();
	}*/

	switch (gameState)
	{
	case eGameState::TITLE:
	{
		break;
	}
	case eGameState::GAME:
		vnFont::setColor(0xffffb84a);

		if (isControllable)
		{

#ifdef _DEBUG
			vnFont::print(10.0f, 0.0f, L"player_vMove : %.3f, %.3f, %.3f",
				XMVectorGetX(player->GetvMove()),
				XMVectorGetY(player->GetvMove()),
				XMVectorGetZ(player->GetvMove())
			);

			vnFont::print(10.0f, 20.0f, L"player_velocity : %.3f, %.3f, %.3f",
				XMVectorGetX(player->Getvelocity()),
				XMVectorGetY(player->Getvelocity()),
				XMVectorGetZ(player->Getvelocity())
			);

			vnFont::print(10.0f, 40.0f, L"player_playerPos : %.3f, %.3f, %.3f",
				player->GetModel()->getPositionX(),
				player->GetModel()->getPositionY(),
				player->GetModel()->getPositionZ()
			);

			/*vnFont::print(10.0f, 50.0f, L"PlayerRotY: %.3f",
				player->getModel()->getRotationY()		//y軸の回転角度
			);

			BoundingBox& playerBox = player->getModel()->getAABBCollider().getBoundingBox();
			vnFont::print(10.0f, 70.0f, L"PlayerCollider: %.3f, %.3f, %.3f",
				playerBox.Center.x,	//AABBの中心座標X
				playerBox.Center.y,	//AABBの中心座標Y
				playerBox.Center.z	//AABBの中心座標Z
			);*/

			/*BoundingBox& goalPos = pCheckerFlag->getAABBCollider().getBoundingBox();
			vnFont::print(10.0f, 170.0f, L"pCheckFlagColliderPos : %.3f, %.3f, %.3f",
				goalPos.Center.x,
				goalPos.Center.y,
				goalPos.Center.z
			);*/

			vnFont::print(10.0f, 60.0f, L"Camera_Pos : %.3f, %.3f, %.3f",
				vnCamera::getPositionX(),
				vnCamera::getPositionY(),
				vnCamera::getPositionZ()
			);

			vnFont::print(10.0f, 90.0f, L"player_speedDown : %.3f",
				player->GetSpeedDown()
			);

			vnFont::print(10.0f, 110.0f, L"inventory_totalWeight : %.3f",
				player->GetInventory().GetTotalWeight()
			);

			vnFont::print(10.0f, 140.0f, L"isControllable : %s", isControllable ? L"true" : L"false");

			vnFont::print(10.0f, 160.0f, L"isDebug : %s", isDebug ? L"true" : L"false");

			vnFont::print(10.0f, 180.0f, L"isAddMaterial : %s", isAddMaterial ? L"true" : L"false");

			vnFont::print(10.0f, 200.0f, L"holdPos : %.3f, %.3f, %.3f",
				XMVectorGetX(holdPos),
				XMVectorGetY(holdPos),
				XMVectorGetZ(holdPos)
			);

			vnFont::print(10.0f, 220.0f, L"Inventory_materialNum : %zu",
				player->GetInventory().GetInventory().size()
			);

			vnFont::print(10.0f, 240.0f, L"InventoryUI_slotNum : %zu",
				inventoryUI->GetInventoryUI().size()
			);


			vnFont::print((float)vnMainFrame::screenWidth - 150.0f, 0.0f, L"DeltaTime : %.3f", TimeManager::GetDeltaTime());

			/*for (int i = 0; i < BLOCK_NUM; i++)
			{
				BoundingBox& blockBox = pBlock[i]->getAABBCollider().getBoundingBox();
				vnFont::print(10.0f, 250.0f + i * 20.0f, L"BlockCollider%d: %.3f, %.3f, %.3f",
					i,
					blockBox.Center.x,	//AABBの中心座標X
					blockBox.Center.y,	//AABBの中心座標Y
					blockBox.Center.z	//AABBの中心座標Z
				);
			}*/
#endif

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
		}
		break;

	case eGameState::RESULT:
		vnFont::setColor(0xffffb84a);
		vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 100.0f, L"操作説明");
		vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 80.0f, L"移動 : W A S D");
		vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 60.0f, player->GetIsJump() ? 0x7fffb84a : 0xffffb84a, L"ジャンプ : Space");

		vnFont::setColor(0xffff0000);
		vnFont::print((float)vnMainFrame::screenCenterX, (float)vnMainFrame::screenCenterY, L"ゴール！");
		break;
	}
}

//描画関数
void SceneMainGame::render()
{
	vnScene::render();

	player->Render();

	if (inventoryUI->GetIsInventoryUIRender())
	{
		inventoryUI->Render(player->GetInventory());
	}

#ifdef _DEBUG
	for (std::size_t i = 0; i < materials.size(); i++)
	{
		materials[i]->Render();	//当たり判定の枠組み表示
	}
#endif
}

bool SceneMainGame::DeleteMaterialData(Material* material)
{
	auto itr = std::find(materials.begin(), materials.end(), material);

	if (itr != materials.end())
	{
		materials.erase(itr);
		deleteObject(material->GetModel());
		return true;
	}

	return false;
}

void SceneMainGame::TitleSetUp()
{
	//プレイヤーの初期化
	//player->getModel()->setPosition(0.0f, 0.0f, 0.0f);	//位置
	//player->getModel()->setRotationY(0.0f);	//Y軸回転

	isControllable = true;
}