#pragma once

#define FENCE_NUM (32) //フェンスの数
#define FENCE_RADIUS (37.5)	//フェンスを配置する演習の背景
#define FIELD_RADIUS (FENCE_RADIUS - 1.0f)	//移動可能半径
#define FAN_RADIUS (10.0f)	//扇形の半径

class SceneFieldTest : public vnScene
{
private:
	vnModel* pPlayer;			//プレイヤーオブジェクト
	vnModel* pShadow;			//影オブジェクト
	vnModel* pCrown;			//王冠オブジェクト

	vnModel* pNPC;				//NPCオブジェクト

	vnModel* pGround;			//地面オブジェクト
	vnModel* pSky;				//背景
	vnModel* pFan;				//扇形オブジェクト

	vnModel* pFence[FENCE_NUM];	//フェンスオブジェクト

	vnBillboard* pBillboard;	//ビルボードオブジェクト

	//極座標の情報
	float radius;	//半径
	float theta;	//角度θ(平面角,経度)
	float phi;		//角度φ(仰角,緯度)

	bool reverseH;	//水平反転
	bool reverseV;	//垂直反転

	bool isInFan;	//扇形の中にいるかどうかのフラグ

public:
	
	bool initialize();
	void terminate();
	
	void execute();
	void render();
};
