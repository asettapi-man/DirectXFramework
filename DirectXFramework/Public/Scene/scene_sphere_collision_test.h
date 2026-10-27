#pragma once

#include "../object/cube.h"

class SceneSphereCollisionTest : public vnScene
{
private:
	vnLight* pLight;	//ライトオブジェクト
	vnModel* pSphere;	//球体オブジェクト
	vnModel* pPoint[2];	//ポイントオブジェクト

	//デバッグ用情報
	const int PointNum = 3;	//頂点数
	XMVECTOR points[3];	//頂点座標

	//直線の情報
	XMVECTOR linePos;	//直線上の一点
	XMVECTOR lineDir;	//直線の方向ベクトル

	//球の情報
	XMVECTOR spherePos;
	float sphereRadius;

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
