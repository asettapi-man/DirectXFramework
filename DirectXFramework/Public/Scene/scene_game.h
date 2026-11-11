#pragma once

#include "../Object/player.h"
#include "../material.h"
#include "../UI/crosshair_ui.h"
#include "../UI/inventory_ui.h"

class SceneMainGame : public vnScene
{
private:
	enum class eGameState
	{
		TITLE,
		GAME,
		RESULT
	};

	static constexpr int ISLANDCOUNT = 0;	//島の生成数
	static constexpr int INITMATERIALCOUNT = 2;	//開始時に生成する資材数	

	eGameState gameState;	//ゲームステート

	Player* player;				//プレイヤー
	CrosshairUI* crosshairUI;	//クロスヘア
	InventoryUI* inventoryUI;	//インベントリUI

	vnModel* pShadow;			//影オブジェクト
	vnModel* pGround;			//地面オブジェクト
	vnModel* pSky;				//背景

	std::vector<vnModel*> islands;	//島オブジェクト管理配列
	std::vector<Material*> materials;	//資材オブジェクト管理配列

	/*//== 画像 ==
	vnSprite* pTitle;	//タイトル画面
	vnSprite* pStartButton;	//スタートボタン
	vnSprite* pQuitButton;	//終了ボタン
	vnSprite* pTitleButton;	//タイトルへ戻るボタン
	vnSprite* pDescriptionUI;	//説明UI背景
	*/

	bool isControllable;		//操作可能フラグ
	bool isAddMaterial;			//資材追加フラグ

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

	bool DeleteMaterialData(Material* material);	//シーンから資材削除関数

	void TitleSetUp();	//再度ゲームプレイできるように初期化
};