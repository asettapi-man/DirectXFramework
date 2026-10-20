#pragma once

#include "../model.h"

class Player
{
private:
	vnModel* model;	//プレイヤーモデル

public:
	inline vnModel* getModel() { return model; }	//モデルの取得

	void initialize();	//初期化
	void execute();		//処理
	void render();		//描画

};
