#include "../../framework.h"
#include "../../Framework/environment.h"

//描画優先順位
enum ePriority
{
	PrioBG = 0,
	PrioCharacter = 100,
	PrioMessageBox = 200,
	PrioFade = 1000,
};

//初期化関数
bool SceneNovelGame::initialize()
{
	//背景のオブジェクトの作成(全画面:1280x720)
	pBG = new vnSprite(
		(float)vnMainFrame::screenCenterX, //640,//中心X
		(float)vnMainFrame::screenCenterY, //360,//中心Y
		(float)vnMainFrame::screenWidth,   //1280,//幅
		(float)vnMainFrame::screenHeight,  //720,//高さ
		L"data/novel/background.png"//画像ファイルのパス
	);
	//キャラクターオブジェクトの作成
	pChara = new vnSprite(
		(float)vnMainFrame::screenCenterX,
		(float)vnMainFrame::screenCenterY,
		400.0f, 680.0f,
		L"data/novel/character.png"
	);

	//メッセージボックス
	pMsgBox = new vnSprite(
		640.0f, 550.0f,
		1000.0f, 200.0f,
		L""
	);


	//ポリゴン(頂点)のカラー(RGB)で透明度(A)の設定(0.0～1.0)
	pMsgBox->vtx[0].r = 0.0f;
	pMsgBox->vtx[0].g = 0.0f;
	pMsgBox->vtx[0].b = 0.0f;
	pMsgBox->vtx[0].a = 0.5f;
	pMsgBox->vtx[1].r = 0.0f;
	pMsgBox->vtx[1].g = 0.0f;
	pMsgBox->vtx[1].b = 0.0f;
	pMsgBox->vtx[1].a = 0.5f;
	pMsgBox->vtx[2].r = 0.0f;
	pMsgBox->vtx[2].g = 0.0f;
	pMsgBox->vtx[2].b = 0.0f;
	pMsgBox->vtx[2].a = 0.5f;
	pMsgBox->vtx[3].r = 0.0f;
	pMsgBox->vtx[3].g = 0.0f;
	pMsgBox->vtx[3].b = 0.0f;
	pMsgBox->vtx[3].a = 0.5f;

	//フェード用スプライト
	pFade = new vnSprite(
		(float)vnMainFrame::screenCenterX, //640,//中心X
		(float)vnMainFrame::screenCenterY, //360,//中心Y
		(float)vnMainFrame::screenWidth,   //1280,//幅
		(float)vnMainFrame::screenHeight,  //720,//高さ
		L"");
	pFade->vtx[0].r = 0.0f;
	pFade->vtx[0].g = 0.0f;
	pFade->vtx[0].b = 0.0f;
	pFade->vtx[1].r = 0.0f;
	pFade->vtx[1].g = 0.0f;
	pFade->vtx[1].b = 0.0f;
	pFade->vtx[2].r = 0.0f;
	pFade->vtx[2].g = 0.0f;
	pFade->vtx[2].b = 0.0f;
	pFade->vtx[3].r = 0.0f;
	pFade->vtx[3].g = 0.0f;
	pFade->vtx[3].b = 0.0f;

	//描画優先順位の設定
	pBG->setRenderPriority(PrioBG);
	pChara->setRenderPriority(PrioCharacter);
	pMsgBox->setRenderPriority(PrioMessageBox);
	pFade->setRenderPriority(PrioFade);

	//オブジェクトの登録
	registerObject(pBG);
	registerObject(pChara);
	registerObject(pMsgBox);
	registerObject(pFade);

	//フォントの作成(フォント名, サイズ)
	pFont = vnFont::create(L"Meiryo", 48);

	//サウンドの作成(ファイル名)
	pSound = new vnSound(L"data/sound/decision1.wav");

	pSound->play();

	return true;
}

//終了関数
void SceneNovelGame::terminate()
{
	//オブジェクトの削除
	deleteObject(pBG);
	deleteObject(pChara);
	deleteObject(pMsgBox);
	deleteObject(pFade);
	//フォントを削除
	SAFE_RELEASE(pFont);
	//サウンドの削除
	delete pSound;
}

//処理関数
void SceneNovelGame::execute()
{
	//フェードイン
	if (pFade->vtx[0].a > 0.0f)
	{
		pFade->vtx[0].a -= 0.01f;
		pFade->vtx[1].a -= 0.01f;
		pFade->vtx[2].a -= 0.01f;
		pFade->vtx[3].a -= 0.01f;
	}

	//マウスの座標(スクリーン座標)
	int mouseX = vnMouse::getX();
	int mouseY = vnMouse::getY();
	//マウスのボタン
	bool mouseL = vnMouse::onL();

	//サウンドの再生
	if (vnMouse::trgL() == true)
	{
		pSound->play();
	}

	//フォントの指定(デフォルトフォントに戻す)
	vnFont::setTextFormat();

#ifdef _DEBUG
	//文字の出力(x座標, y座標, 文字列)
	vnFont::print(100.0f, 200.0f, 0xffffffff, L"文字列");

	//カラー指定(AARRGGBB)
	vnFont::print(100.0f, 220.0f, 0xffff0000, L"文字列");

	//書式設定
	int a = 123;
	vnFont::print(100.0f, 240.0f, 0xffffff00, L"a = %d", a);

	vnFont::print(100.0f, 260.0f, L"マウス座標 : (%d, %d)", mouseX, mouseY);
	vnFont::print(100.0f, 280.0f, L"左ボタン : %d", mouseL);
#endif

	//フォントの指定
	vnFont::setTextFormat(pFont);

	vnFont::print(140.0f, 450.0f, 0xffffffff, L"作成したフォントで表示");

	vnScene::execute();
}

//描画関数
void SceneNovelGame::render()
{
	vnScene::render();
}
