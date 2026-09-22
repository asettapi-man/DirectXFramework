#pragma once

#define BLOCK_NUM 16
class SceneAssingment : public vnScene
{
private:
	enum class eGameState
	{
		TITLE,
		GAME,
		RESULT
	};

	enum class eDirection
	{
		NONE,
		X_RIGHT,
		X_LEFT,
		Y_TOP,
		Y_DOWN,
		Z_FRONT,
		Z_BACK,
	};

	eGameState gameState;	//ゲームステート
	eDirection direction;	//衝突方向

	vnModel* pPlayer;			//プレイヤーオブジェクト
	vnModel* pShadow;			//影オブジェクト

	vnModel* pGround;			//地面オブジェクト
	vnModel* pSky;				//背景

	vnModel* pBlock[BLOCK_NUM];	//ブロックオブジェクト

	vnModel* pCheckerFlag;	//ゴール

	//== 画像 ==
	vnSprite* pTitle;	//タイトル画面
	vnSprite* pStartButton;	//スタートボタン
	vnSprite* pQuitButton;	//終了ボタン
	vnSprite* pTitleButton;	//タイトルへ戻るボタン
	vnSprite* pDescriptionUI;	//説明UI背景

	//極座標の情報
	float radius;	//半径
	float theta;	//角度θ(平面角,経度)
	float phi;		//角度φ(仰角,緯度)

	bool reverseH;	//水平反転
	bool reverseV;	//垂直反転

	XMVECTOR vMove;		//移動ベクトル
	XMVECTOR velocity;	//速度ベクトル
	XMVECTOR jumpVec;	//ジャンプベクトル
	XMVECTOR gravity;	//重力ベクトル

	bool isControllable;	//操作可能フラグ
	bool isJump;	//ジャンプ可能フラグ
	std::vector<bool> isFlip;	//箱移動反転フラグ

	XMFLOAT3 playerPos;	//プレイヤーの座標
	XMFLOAT3 blockPos;	//ブロックの座標
	XMFLOAT3 goalPos;	//ゴールの座標

	std::vector<XMFLOAT3> vStartBlockPos;	//動く床の初期値保存配列

	vnSound* pBGM;	//BGM

	vnSound* pClickSE;	//クリック時の効果音
	vnSound* pClearSE;	//クリア時の効果音
	vnSound* pJumpSE;	//ジャンプ時の効果音

public:
	
	bool initialize();
	void terminate();
	
	void execute();
	void render();

	//再度ゲームプレイできるように初期化
	void TitleSetUp();

	//ブロックの折り返し移動
	void MoveToTarget(vnModel* pBlock, int n);

	//画像の当たり判定
	bool CheckImageMouseOver(vnSprite* sprite);

	//画像へのマウスオーバー時の色変更
	void MouseOverImageColor(vnSprite* sprite, float r = 0.5f, float g = 0.5f, float b = 0.5f);

	//画像の通常時色
	void ImageColor(vnSprite* sprite);

	//== AABBの当たり判定 ==
	bool CheckBoundingBoxCollision(vnModel* m1, vnModel* m2);
};