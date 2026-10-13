#pragma once

#include "../object/cube.h"

class SceneCollisionTest : public vnScene
{
private:
	vnLight* pLight;	//ライトオブジェクト
	vnModel* pPoint;	//ポイントオブジェクト

	//デバッグ用情報
	const int PointNum = 5;	//頂点数
	XMVECTOR points[5];	//頂点座標

	//直線の情報
	XMVECTOR linePos;	//直線上の一点
	XMVECTOR lineDir;	//直線の方向ベクトル

	//平面の情報
	//平面方程式 ax + by + cz = d
	XMVECTOR planeNormal;	//平面の法線ベクトル
	float PlaneDistance;	//平面と原点の距離

	//極座標の情報
	float radius;	//半径
	float theta;	//角度θ(平面角,経度)
	float phi;		//角度φ(仰角,緯度)

	//操作の種類
	int column;	//行
	int	row;	//列

	enum class eOperation
	{
		On,
		Off,
	};

	eOperation e[5][3];

public:
	
	bool initialize();
	void terminate();
	
	void execute();
	void render();
};
