#pragma once

#include "../Object/player.h"

#define BLOCK_NUM 16
class SceneMainGame : public vnScene
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

	Player* player;				//プレイヤー
	vnModel* pShadow;			//影オブジェクト

	vnModel* pGround;			//地面オブジェクト
	vnModel* pSky;				//背景

	/*//== 画像 ==
	vnSprite* pTitle;	//タイトル画面
	vnSprite* pStartButton;	//スタートボタン
	vnSprite* pQuitButton;	//終了ボタン
	vnSprite* pTitleButton;	//タイトルへ戻るボタン
	vnSprite* pDescriptionUI;	//説明UI背景
	*/
	//極座標の情報
	float radius;	//半径
	float theta;	//角度θ(平面角,経度)
	float phi;		//角度φ(仰角,緯度)

	bool reverseH;	//水平反転
	bool reverseV;	//垂直反転

	XMVECTOR vMove;		//移動ベクトル
	XMVECTOR velocity;	//速度ベクトル
	XMVECTOR gravity;	//重力ベクトル

	float jumpForce;	//ジャンプ力

	bool isControllable;	//操作可能フラグ
	bool isJump;	//ジャンプ可能フラグ

	XMFLOAT3 playerPos;	//プレイヤーの座標

	/*vnSound* pBGM;	//BGM

	vnSound* pClickSE;	//クリック時の効果音
	vnSound* pClearSE;	//クリア時の効果音
	vnSound* pJumpSE;	//ジャンプ時の効果音
	*/
public:
	
	bool initialize();
	void terminate();
	
	void execute();
	void render();

	//再度ゲームプレイできるように初期化
	void TitleSetUp();

	//== AABBの当たり判定 ==
	bool CheckPlayerCollision(Player* p, AABBCollider* c);
};