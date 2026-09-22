//--------------------------------------------------------------//
//	"scene_table.cpp"											//
//		シーンテーブル											//
//													2026/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../Framework/environment.h"

//起動時のシーン
eSceneTable initialScene = eSceneTable::Boot;
//現在のシーン
eSceneTable currentScene = (eSceneTable)-1;
//切り替え予約のシーン
eSceneTable reserveScene = initialScene;


//各シーンの名前
WCHAR SceneName[(int)eSceneTable::SceneMax][32] =
{
	L"Boot",
	L"Field Test",
	L"Cube Test",
	L"PriorityTest",
	L"Model Test",
	L"Sprite Test",
	L"Joystick Test",
	L"SE Test",
	L"Font Test",
	L"Novel Game",
	L"Assingment",
};

//ステージ切り替え関数(予約)
void switchScene(eSceneTable scene)
{
	reserveScene = scene;
}

//現在のステージ取得(実処理)
void switchScene()
{
	if(currentScene==reserveScene)
	{
		return;
	}
	
	vnMainFrame::terminateScene();
	
	vnScene *pScene=NULL;
	switch(reserveScene)
	{
	case Boot:			pScene = new SceneBoot();			break;
	case FieldTest:		pScene = new SceneFieldTest();		break;
	case CubeTest:		pScene = new SceneCubeTest();		break;
	case PriorityTest:	pScene = new ScenePriorityTest();	break;
	case ModelTest:		pScene = new SceneModelTest();		break;
	case SpriteTest:	pScene = new SceneSpriteTest();		break;
	case JoystickTest:	pScene = new SceneJoystickTest();	break;
	case SeTest:		pScene = new SceneSeTest();			break;
	case FontTest:		pScene = new SceneFontTest();		break;
	case NovelGame:		pScene = new SceneNovelGame();		break;
	case Assingment:	pScene = new SceneAssingment();		break;
	}
	
	vnMainFrame::initializeScene(pScene);
	
	currentScene = reserveScene;
}
