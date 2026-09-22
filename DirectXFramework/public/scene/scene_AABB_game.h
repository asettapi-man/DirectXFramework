#pragma once

#define FENCE_NUM	(32)		//フェンスモデルの数

#define FENCE_RADIUS (37.5f)	//フェンスを配置する円周の半径
#define FIELD_RADIUS (36.5f)	//プレイヤーが移動できる円周の半径

#define AABB_MAX (256)

//AABBオブジェクト
class cObjAABB : public vnModel
{
public:
	XMVECTOR center;	//AABBの中心(vnModel::Positionからの相対値
	XMVECTOR size;		//AABBのサイズ

	cObjAABB(const WCHAR* folder, const WCHAR* file);
};

class SceneAABBGame : public vnScene
{
private:

	//背景モデル関連
	vnModel* pSky;
	vnModel* pGround;
	vnModel* pFence[FENCE_NUM];

	//プレイヤーオブジェクト
	cObjAABB* pPlayer;

	//オブジェクト関連
	int AABBNum;
	cObjAABB* pBox[AABB_MAX];

	//UI関連
	vnSprite* pUI;

	//カメラ関連(極座標)
	float	cam_theta;
	float	cam_phi;
	float	cam_distance;
	float	init_theta;
	float	init_phi;
	float	init_distance;

	//カメラ関連(補間)
	XMVECTOR CameraTrg;
	XMVECTOR CameraPos;
	float CameraTrgRate;
	float CameraPosRate;

	//カメラ関連(操作)
	float camSpeedTheta;
	float camSpeedPhi;
	float camSpeedRadius;
	XMVECTOR camBias;

	//プレイヤーアクション関連(移動)
	float		speed;

	//プレイヤーアクション関連(ジャンプ)
	bool air;
	XMVECTOR	jumpForce;
	XMVECTOR	velocity;
	XMVECTOR	gravity;

	//プレイヤー情報
	XMVECTOR	startPos;
	XMVECTOR	peekPos;
	XMVECTOR	peekRot;
	XMVECTOR	peekCam;
	
	//ゲーム管理
	float		peekHeight;

	//衝突方向
	enum eDirection
	{
		None,
		X_Pos,
		X_Neg,
		Y_Pos,
		Y_Neg,
		Z_Pos,
		Z_Neg,
	};
	//AABB同士の衝突の検出・解消
	eDirection collideAABB(cObjAABB *p1, cObjAABB *p2);

	//AABBオブジェクトの作成
	cObjAABB* createObjAABB(float posX, float posY, float posZ, float sizeX, float sizeY, float sizeZ);

	//プレイヤーの移動(入力処理)
	bool movePlayer(vnObject* p);

	//カメラの操作(入力処理)
	bool operateCamera(bool update);

	//移動範囲の制限
	void checkFence(vnModel* p);

	//ゲーム情報の確保
	void savePeek();


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
