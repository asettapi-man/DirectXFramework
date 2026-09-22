#include "../../framework.h"
#include "../../Framework/environment.h"

//初期化関数
bool ScenePriorityTest::initialize()
{
	//作成できるスプライトの最大数(配列の数)
	//配列全体のバイト数を要素一つのバイト数で割る
	SpriteMax = sizeof(pSprite) / sizeof(vnSprite*);

	//ポインタの初期化(NULLクリア)
	for (int i = 0; i < SpriteMax; i++)
	{
		pSprite[i] = NULL;
	}

	//作成したスプライトの数
	SpriteNum = 0;

	return true;
}

//終了関数
void ScenePriorityTest::terminate()
{
	//生成したスプライトの削除
	for (int i = 0; i < SpriteMax; i++)
	{
		if (pSprite[i] == NULL)continue;
		deleteObject(pSprite[i]);
	}
}

//処理関数
void ScenePriorityTest::execute()
{
	vnFont::print(10, 10, L"Priority Test : %d / %d", SpriteNum, SpriteMax);

	//マウスの左クリック
	if (vnMouse::trg() == true)
	{
		//マウスカーソルの座標を取得
		float mx = (float)vnMouse::getX();
		float my = (float)vnMouse::getY();

		int spriteID = getNearestSprite(mx, my, 32.0f);

		if (spriteID >= 0)
		{	//作成済みのスプライトを掴む
			pPick = pSprite[spriteID];
			pickDifX = pPick->posX - mx;
			pickDifY = pPick->posY - my;
			setSpriteColor(pPick, 1.0f, 1.0f, 0.75f);
		}
		else
		{	//スプライトの新規作成

			//配列から空き要素を探す
			for (int i = 0; i < SpriteMax; i++)
			{
				if (pSprite[i] != NULL)continue;

				pSprite[i] = createSprite(mx, my);

				//スプライトの数を1増やす(increment)
				SpriteNum++;

				//作成したスプライトを掴み状態にする
				pPick = pSprite[i];
				pickDifX = 0.0f;
				pickDifY = 0.0f;
				setSpriteColor(pPick, 1.0f, 0.75f, 0.75f);
				break;
			}
		}
	}
	else if (vnMouse::relL())
	{
		//掴んでいるスプライトを離す
		setSpriteColor(pPick);
		pPick = NULL;
	}
	else if (vnMouse::trgR())
	{
		//スプライトの削除
		float mx = (float)vnMouse::getX();
		float my = (float)vnMouse::getY();

		int spriteID = getNearestSprite(mx, my, 32.0f);

		if (spriteID >= 0)
		{
			if (pPick == pSprite[spriteID])pPick = NULL;

			deleteObject(pSprite[spriteID]);
			pSprite[spriteID] = NULL;
			SpriteNum--;
		}
	}

	//掴んでいるスプライトの操作
	if (pPick != NULL)
	{
		pPick->posX = (float)vnMouse::getX() + pickDifX;
		pPick->posY = (float)vnMouse::getY() + pickDifY;
	}

	vnScene::execute();
}

//描画関数
void ScenePriorityTest::render()
{
	vnScene::render();
}

//スプライトを作成
vnSprite* ScenePriorityTest::createSprite(float x, float y)
{
	int disp = rand() % 10;	//0～9までのランダム値を得る

	//0～9の画像に対応するuvを計算
	float interval_u = 1.0f / 4.0f;	//画像ごとの間隔(u方向)
	float interval_v = 1.0f / 4.0f;	//画像ごとの間隔(v方向)
	float start_u = (float)(disp % 4) * interval_u;	//設定したいu値の左側の値
	float start_v = (float)(disp / 4) * interval_v;	//設定したいv値の上側の値

	//スプライトを生成
	vnSprite* ret = new vnSprite(x, y, 64.0f, 64.0f,
		L"data/image/number_icon.png",
		start_u, start_u + interval_u, start_v, start_v + interval_v);
	//描画優先順位の設定
	ret->setRenderPriority(10 - disp);

	//シーンへ登録
	registerObject(ret);

	return ret;
}

//指定座標に最も近いスプライトを取得
int ScenePriorityTest::getNearestSprite(float x, float y, float limit)
{
	float limitSq = limit * limit;

	float dist = D3D12_FLOAT32_MAX;
	int ret = -1;

	for (int i = 0; i < SpriteMax; i++)
	{
		if (pSprite[i] == NULL)continue;
		float dx = pSprite[i]->posX - x;
		float dy = pSprite[i]->posY - y;
		float dSq = dx * dx + dy * dy;
		if (dSq > limitSq)continue;
		if (dSq < dist)
		{
			dist = dSq;
			ret = i;
		}
	}
	return ret;
}

//スプライトの頂点カラーを一括設定
void ScenePriorityTest::setSpriteColor(vnSprite* p, float r, float g, float b, float a)
{
	if(!p)return;
	for (int i = 0; i < vnSprite::VertexNum; i++)
	{
		p->vtx[i].r = r;
		p->vtx[i].g = g;
		p->vtx[i].b = b;
		p->vtx[i].a = a;
	}
}
