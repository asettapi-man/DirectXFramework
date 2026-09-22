//--------------------------------------------------------------//
//	"scene_table.h"												//
//		シーンテーブル											//
//													2026/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//シーンの種類
enum eSceneTable
{
	Boot,
	FieldTest,
	CubeTest,
	PriorityTest,
	ModelTest,
	SpriteTest,
	JoystickTest,
	SeTest,
	FontTest,
	NovelGame,
	Assingment,
	SceneMax,
};

//シーンクラス定義ファイル
#include "scene.h"
#include "scene_boot.h"
#include "scene_field_test.h"
#include "scene_model_test.h"
#include "scene_cube_test.h"
#include "scene_priority_test.h"
#include "scene_sprite_test.h"
#include "scene_joystick_test.h"
#include "scene_se_test.h"
#include "scene_font_test.h"
#include "scene_novel_game.h"
#include "scene_assingment_game.h"


//シーン切り替え関数(予約)
void switchScene(eSceneTable scene);
//現在のシーン取得(実処理)
void switchScene();
