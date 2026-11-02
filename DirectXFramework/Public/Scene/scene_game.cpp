#include "../../framework.h"
#include "../../Framework/environment.h"

//初期化関数
bool SceneMainGame::initialize()
{	
	//== 生成 ==
	player = new Player(L"data/model/bear/", L"bear.vnm", XMFLOAT3(0.0f, 1.3f, 0.0f), XMFLOAT3(0.5f, 1.3f, 0.5f));
	player->Initialize();

	pShadow = new vnModel(L"data/model/", L"shadow.vnm");		//影
	pShadow->setTransparent(true);	//影の半透明を有効
	pShadow->setPositionY(0.01f);	//影の位置を少し浮かせる	Z-Fighting対策

	//DIffuse : デフォルト(1, 1, 1, 1)	※wが半透明度
	//Ambient : デフォルト(0, 0, 0, 0)
	//Specular: デフォルト(0, 0, 0, 1)	※w（強度）は0以下にしない

	pGround = new vnModel(L"data/model/", L"ground.vnm");		//地面

	pSky = new vnModel(L"data/model/", L"skydome.vnm");			//背景
	pSky->setLighting(false);		//背景のライティングを無効

	for (int i = 0; i < modelSize; i++)
	{
		vnModel* model = new vnModel(L"data/model/", L"ForestIsland.vnm");
		models.push_back(model);
		models[i]->setPosition(50.0f * i, 0.0f, 0.0f);
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

	for (int i = 0; i < modelSize; i++)
	{
		registerObject(models[i]);
	}

	//== 半透明のオブジェクトは不透明の後に描画する ==
	registerObject(pShadow);

	/*//== 画像 ==
	registerObject(pTitle);
	registerObject(pStartButton);
	registerObject(pQuitButton);
	registerObject(pTitleButton);
	registerObject(pDescriptionUI);
	*/

	//== 初期化 ==
	isControllable = true;	//操作可能フラグの初期化

	gameState = eGameState::GAME;	//ゲームステートの初期化
	return true;
}

//終了関数
void SceneMainGame::terminate()
{
	//== 破棄 ==
	delete player;

	deleteObject(player->GetModel());
	deleteObject(pGround);
	deleteObject(pSky);

	for (int i = 0; i < modelSize; i++)
	{
		deleteObject(models[i]);
	}

	deleteObject(pShadow);

	/*//== 画像 ==
	deleteObject(pTitle);
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
	player->Execute();
	//pShadow->setPosition(player->getModel()->getPosition());	//影の座標をプレイヤーの座標に合わせる
	pShadow->setPositionY(0.01f);	//影の座標を少し浮かせる	Z-Fighting対策

	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 100.0f, L"操作説明");
	vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 80.0f, L"移動 : W A S D");
	vnFont::print((float)vnMainFrame::screenWidth - 150.0f, (float)vnMainFrame::screenHeight - 60.0f, player->GetIsJump() ? 0x7fffb84a : 0xffffb84a, L"ジャンプ : Space");
	

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
			vnFont::print(10.0f, 90.0f, L"Jump: %s", player->GetIsJump() ? L"true" : L"false");

			vnFont::print(10.0f, 110.0f, L"vMove : %.3f, %.3f, %.3f",
				XMVectorGetX(player->GetvMove()),
				XMVectorGetY(player->GetvMove()),
				XMVectorGetZ(player->GetvMove())
			);

			vnFont::print(10.0f, 130.0f, L"velocity : %.3f, %.3f, %.3f",
				XMVectorGetX(player->Getvelocity()),
				XMVectorGetY(player->Getvelocity()),
				XMVectorGetZ(player->Getvelocity())
			);
			/*vnFont::print(10.0f, 10.0f, L"PlayerPos: %.3f, %.3f, %.3f",
				player->getModel()->getPositionX(),		//x座標
				player->getModel()->getPositionY(), 	//y座標
				player->getModel()->getPositionZ()		//z座標
			);

			vnFont::print(10.0f, 50.0f, L"PlayerRotY: %.3f",
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

			vnFont::print(10.0f, 190.0f, L"isControllable : %s", isControllable ? L"true" : L"false");

			vnFont::print(10.0f, 210.0f, L"CameraPos : %.3f, %.3f, %.3f",
				vnCamera::getPositionX(),
				vnCamera::getPositionY(),
				vnCamera::getPositionZ()
			);

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
}

void SceneMainGame::TitleSetUp()
{
	//プレイヤーの初期化
	//player->getModel()->setPosition(0.0f, 0.0f, 0.0f);	//位置
	//player->getModel()->setRotationY(0.0f);	//Y軸回転

	isControllable = true;
}