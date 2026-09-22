//--------------------------------------------------------------//
//	"vn_environment.h"											//
//		フレームワーク共通環境ヘッダー							//
//													2026/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//ライブラリ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")
#pragma comment(lib, "winmm.lib")

#if (_MSC_VER>=1930)

//Visual Studio 2022
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/Bin/Desktop_2022_Win10/x64/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "DirectXTex/Bin/Desktop_2022_Win10/x64/Release/DirectXTex.lib")
#endif

#else

//Visual Studio 2019
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/Bin/Desktop_2019_Win10/x64/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "DirectXTex/Bin/Desktop_2019_Win10/x64/Release/DirectXTex.lib")
#endif

#endif

//共通ヘッダファイルのインクルード
#include <stdio.h>
#include <assert.h>
#include <mmsystem.h>
#include <locale.h>
#include <time.h>
#include <random>
#include <vector>

//DirectXヘッダー
#include <d2d1_3.h>
#include <dwrite.h>
#include <d3d11on12.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#define	DIRECTINPUT_VERSION	0x0800
#include <dinput.h>
#include <Xinput.h>
#include <xaudio2.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>	//当たり判定用

#include "../DirectXTex/DirectXTex.h"

//ネームスペース
using namespace DirectX;

//一般マクロ
#define SCREEN_WIDTH	(1280)	//クライアント領域の幅(ピクセル)
#define SCREEN_HEIGHT	(720)	//クライアント領域の高さ(ピクセル)

//リリース
#define SAFE_RELEASE(p) {if(p){(p)->Release();(p)=NULL;}}

//フレームワークヘッダー
#include "DirectX/shader.h"
#include "DirectX/Direct3D.h"
#include "DirectX/Direct2D.h"
#include "DirectX/DirectInput.h"
#include "DirectX/XInput.h"
#include "DirectX/XAudio2.h"

#include "mouse.h"
#include "keyboard.h"
#include "joystick.h"
#include "sound.h"

#include "font.h"

#include "camera.h"
#include "light.h"
#include "debugDraw.h"

#include "../Public/object.h"
#include "../Public/model.h"
#include "../Public/sprite.h"
#include "../Public/billboard.h"

#include "../Public/scene/scene.h"
#include "../Public/scene/scene_table.h"

#include "../Public/Collider/AABBCollider.h"

#include "mainframe.h"
