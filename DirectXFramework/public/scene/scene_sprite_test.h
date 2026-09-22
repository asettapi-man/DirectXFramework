//--------------------------------------------------------------//
//	"scene_sprite_test.h"										//
//		スプライトテスト										//
//													2026/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class SceneSpriteTest : public vnScene
{
private:
	vnSprite* pSprite;

	//操作の種類
	enum eOperation
	{
		PositionX,
		PositionY,
		ScaleX,
		ScaleY,
		Rotate,
		OperationMax,
	};
	
	const int ImageCount = 4;
	int counter;

	int	Cursor;

public:
	bool initialize();
	void terminate();
	
	void execute();
	void render();
};
