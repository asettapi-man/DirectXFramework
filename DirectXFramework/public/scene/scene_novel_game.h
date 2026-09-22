#pragma once

class SceneNovelGame : public vnScene
{
private:
	vnSprite* pBG;
	vnSprite* pChara;
	vnSprite* pMsgBox;

	vnSprite* pFade;

	IDWriteTextFormat* pFont;

	vnSound* pSound;

public:
	//‰Šú‰»
	bool initialize();
	//I—¹
	void terminate();

	//ˆ—
	void execute();
	//•`‰æ
	void render();
};
