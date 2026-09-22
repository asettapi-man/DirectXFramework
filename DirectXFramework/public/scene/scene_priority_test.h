#pragma once

class ScenePriorityTest : public vnScene
{
private:
	vnSprite	*pSprite[512];	//スプライトの配列
	int			SpriteMax;		//作成できるスプライトの最大数
	int			SpriteNum;		//作成したスプライトの数

	//掴み情報
	vnSprite	*pPick;
	float		pickDifX;
	float		pickDifY;

	//スプライトを作成
	vnSprite* createSprite(float x, float y);

	//指定座標に最も近いスプライトを取得(limit : スプライトからの有効距離)
	int getNearestSprite(float x, float y, float limit);

	//スプライトの頂点カラーを一括設定
	void setSpriteColor(vnSprite* p, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

public:
	//初期化
	bool initialize();
	//終了
	void terminate();

	//処理
	void execute();
	//描画
	void render();
};
