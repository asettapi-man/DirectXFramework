#pragma once

#define FENCE_NUM (32) //フェンスの数
#define FENCE_RADIUS (37.5)	//フェンスを配置する演習の背景
#define FIELD_RADIUS (FENCE_RADIUS - 1.0f)	//移動可能半径
#define FAN_RADIUS (10.0f)	//扇形の半径

class SceneGroundTest : public vnScene
{
private:
	vnModel* pPlayer;			//プレイヤーオブジェクト
	vnModel* pGround;			//地面オブジェクト

	XMVECTOR planeNormal;	//平面（三角形）の法線ベクトル
	float planeDistance;	//平面までの距離

	DWORD debugLineColor;	//デバッグ用コライダーの線

	//極座標の情報
	float radius;	//半径
	float theta;	//角度θ(平面角,経度)
	float phi;		//角度φ(仰角,緯度)

	bool reverseH;	//水平反転
	bool reverseV;	//垂直反転

	XMVECTOR velocity;	//速度
	XMVECTOR gravity;	//重力加速度

	XMVECTOR linePos;	//線分の位置
	XMVECTOR lineDir;	//線分の向き
	float jumpForce;	//ジャンプ力
	bool isJump;	//ジャンプフラグ

	//線分と地形（三角形群）の衝突を調べる関数
	//outに結果を出力
	bool isCollide(XMVECTOR* out, XMVECTOR* linePos, XMVECTOR* lineDir, float length);	
public:
	
	bool initialize();
	void terminate();
	
	void execute();
	void render();
};
