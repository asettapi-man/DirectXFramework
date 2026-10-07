#pragma once

#include "../object/cube.h"

class SceneCollisionTest : public vnScene
{
private:
	vnLight* pLight;	//ライトオブジェクト

	const int PointNum = 5;	//頂点数

	XMVECTOR point[5];	//頂点座標

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
