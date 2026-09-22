//--------------------------------------------------------------//
//	"vn_scene.cpp"												//
//		シーン基底クラス										//
//													2026/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../Framework/environment.h"

vnObject* vnScene::pObject3D_Array[vnOBJECT3D_MAX];
vnSprite* vnScene::pSprite2D_Array[vnOBJECT2D_MAX];

//コンストラクタ
vnScene::vnScene()
{
	for (int i = 0; i < vnOBJECT3D_MAX; i++)
	{
		pObject3D_Array[i] = NULL;
	}

	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		pSprite2D_Array[i] = NULL;
	}
}

//デストラクタ
vnScene::~vnScene()
{
	for (int i = 0; i < vnOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == NULL) continue;
		delete pObject3D_Array[i];
		pObject3D_Array[i] = NULL;
	}

	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		if (pSprite2D_Array[i] == NULL) continue;
		delete pSprite2D_Array[i];
		pSprite2D_Array[i] = NULL;
	}
}

//処理関数
void vnScene::execute()
{
	//3Dオブジェクトのexecute実行
	for (int i = 0; i < vnOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == NULL) continue;
		if (pObject3D_Array[i]->isExecuteEnable() == false) continue;
		pObject3D_Array[i]->execute();
	}

	//2Dオブジェクトのexecute実行
	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		if (pSprite2D_Array[i] == NULL) continue;
		if (pSprite2D_Array[i]->isExecuteEnable() == false) continue;
		pSprite2D_Array[i]->execute();
	}
}

//描画関数
void vnScene::render()
{
	//3Dオブジェクトのrender実行
	for (int i = 0; i < vnOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == NULL) continue;
		if (pObject3D_Array[i]->isRenderEnable() == false) continue;
		pObject3D_Array[i]->render();
	}

	//2Dオブジェクトの描画優先順位に従って配列をソート
	int pSpriteSub[vnOBJECT2D_MAX];//2Dオブジェクトの添え字番号
	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		pSpriteSub[i] = NULL;	//初期化
	}

/*	size_t sprite2DArraySize = sizeof(pSprite2D_Array) / sizeof(pSprite2D_Array[0]);
	for (int i = 0; i < sprite2DArraySize - 1; i++)
	{
		int pSpritePriority = 100;	//検索用最大値
		for (int j = i; j < sprite2DArraySize - 1; j++)
		{
			if (pSprite2D_Array[j] != NULL && pSpritePriority > pSprite2D_Array[j]->getRenderPriority())
			{
				pSpritePriority = pSprite2D_Array[j]->getRenderPriority();	//最小値を更新
				pSpriteSub[i] = j;	//最小値がいる添え字番号を保存
			}
		}

		if (pSprite2D_Array[i] != NULL && pSpriteSub[i] >= 0)
		{
			//入れ変え
			auto tmp = pSprite2D_Array[i];
			pSprite2D_Array[i] = pSprite2D_Array[pSpriteSub[i]];
			pSprite2D_Array[pSpriteSub[i]] = tmp;
		}
	}*/

	//2Dオブジェクトのrender実行
	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		if (pSprite2D_Array[i] == NULL) continue;
		if (pSprite2D_Array[i]->isRenderEnable() == false) continue;
		pSprite2D_Array[i]->render();
	}
}

//3Dオブジェクトの登録
bool vnScene::registerObject(vnObject* pObj)
{
	if (pObj == NULL) return false;

	for (int i = 0; i < vnOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == NULL)
		{
			pObject3D_Array[i] = pObj;
			return true;
		}
	}
	return false;
}

//3Dオブジェクトの削除
void vnScene::deleteObject(vnObject* pObj)
{
	if (pObj == NULL) return;

	for (int i = 0; i < vnOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == pObj)
		{
			delete pObject3D_Array[i];
			pObject3D_Array[i] = NULL;
			return;
		}
	}
	return;
}

//2Dオブジェクトの登録
bool vnScene::registerObject(vnSprite* pSp)
{
	if (pSp == NULL) return false;

	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		if (pSprite2D_Array[i] == NULL)
		{
			pSprite2D_Array[i] = pSp;
			return true;
		}
	}
	return false;
}

//2Dオブジェクトの削除
void vnScene::deleteObject(vnSprite* pSp)
{
	if (pSp == NULL) return;

	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		if (pSprite2D_Array[i] == pSp)
		{
			delete pSprite2D_Array[i];
			pSprite2D_Array[i] = NULL;
			return;
		}
	}
	return;
}
