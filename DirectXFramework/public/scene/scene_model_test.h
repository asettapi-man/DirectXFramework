#pragma once

#include "../object/cube.h"

class SceneModelTest : public vnScene
{
private:
	//cCube* pCube;	//キューブオブジェクト
	vnModel* pModel;	//モデルオブジェクト

	cCube* pTheta;
	cCube* pPhi;	
	vnLight* pLight;	//ライトオブジェクト

	//極座標の情報
	float radius;	//半径
	float theta;	//角度θ(平面角,経度)
	float phi;		//角度φ(仰角,緯度)

	//操作の種類
	enum eOperation
	{
		PositionX,
		PositionY,
		PositionZ,
		RotateX,
		RotateY,
		RotateZ,
		ScaleX,
		ScaleY,
		ScaleZ,
		//CameraFovY,
		//CameraAspect,
		LightDirX,
		LightDirY,
		LightDirZ,
		LightColR,
		LightColG,
		LightColB,
		LightAmbR,
		LightAmbG,
		LightAmbB,
		Radius,
		Theta,
		Phi,
		OperationMax
	};

	XMVECTOR resetPos;
	XMVECTOR resetRot;
	XMVECTOR resetScl;
	XMVECTOR resetLightDir;
	XMVECTOR resetLightCol;
	XMVECTOR resetLightAmb;

	int	Cursor;

public:
	
	bool initialize();
	void terminate();
	
	void execute();
	void render();
};
