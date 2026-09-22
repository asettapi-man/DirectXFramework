//--------------------------------------------------------------//
//	"vn_scene.h"												//
//		シーン基底クラス										//
//													2026/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

#define vnOBJECT3D_MAX (512)	//3Dオブジェクトを登録できる最大数
#define vnOBJECT2D_MAX (512)	//2Dオブジェクトを登録できる最大数

class vnScene
{
private:
	static vnObject* pObject3D_Array[vnOBJECT3D_MAX];
	static vnSprite* pSprite2D_Array[vnOBJECT2D_MAX];

public:
	vnScene();
	virtual ~vnScene();

	virtual bool initialize() = 0;
	virtual void terminate() = 0;
	
	virtual void execute();
	virtual void render();

	//3Dオブジェクト
	static bool registerObject(vnObject* pObj);	//オブジェクトの登録（配列の空きに入れる）
	static void deleteObject(vnObject* pObj);	//オブジェクトの削除（配列から削除/オブジェクト自体をdeleteする）

	//2Dオブジェクト
	static bool registerObject(vnSprite* pSp);	//オブジェクトの登録（配列の空に入れる）
	static void deleteObject(vnSprite* pSp);	//オブジェクトの削除（配列から削除/オブジェクト自体をdeleteする）
}; 
