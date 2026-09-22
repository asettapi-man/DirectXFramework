// vn_framework3D_2026.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "DirectXFramework.h"

#include "Framework/environment.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

HWND hWnd = NULL;								// ウィンドウのハンドル

//クライアント領域のサイズ
int screen_width = SCREEN_WIDTH;
int screen_height = SCREEN_HEIGHT;

bool isFullScreen = false;  //フルスクリーン変更フラグ

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_VNFRAMEWORK3D2026, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VNFRAMEWORK3D2026));

    MSG msg;

    //メインフレームの初期化
    vnMainFrame::initialize(hWnd, hInst, screen_width, screen_height);

    // メイン メッセージ ループ:
    while (true)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                goto END;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //メインフレームの実行
        vnMainFrame::execute();
    }

END:

    //メインフレームの終了
    vnMainFrame::terminate();

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VNFRAMEWORK3D2026));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;   // MAKEINTRESOURCEW(IDC_VNFRAMEWORK3D2026);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//マウスカーソルの移動範囲をクライアント領域内におさめる
void LockCursorToWindow(HWND hWnd)
{
    //画面がフルスクリーンなら範囲指定を無しにする
    if (isFullScreen) return;

    //クライアント領域の取得
    RECT rc;
    GetClientRect(hWnd, &rc);

    //クライアント領域の左上・右下を取得
    POINT topLeft;
    topLeft.x = rc.left;
    topLeft.y = rc.top;

    POINT bottomRight;
    bottomRight.x = rc.right;
    bottomRight.y = rc.bottom;

    //クライアント座標からスクリーン座標へ変換
    ClientToScreen(hWnd, &topLeft);
    ClientToScreen(hWnd, &bottomRight);

    //四つ角を取得
    rc.left = topLeft.x;
    rc.top = topLeft.y;
    rc.right = bottomRight.x;
    rc.bottom = bottomRight.y;

    //マウスカーソルの移動範囲を設定
    ClipCursor(&rc);
}

//マウスカーソルの移動範囲を削除する
void UnlockCursor()
{
    ClipCursor(nullptr);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   //HWND 
    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU, //WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   //クライアント領域が希望のサイズになるようにウィンドウ全体のサイズを計算して再設定
   RECT rw, rc;
   ::GetWindowRect(hWnd, &rw); // ウィンドウ全体のサイズ
   ::GetClientRect(hWnd, &rc); // クライアント領域のサイズ

   int new_width = (rw.right - rw.left) - (rc.right - rc.left) + screen_width;
   int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + screen_height;

   ::SetWindowPos(hWnd, nullptr, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   //WM_PAINTを発生させないようにする
   ValidateRect(hWnd, 0);

   return TRUE;
}

void FullScreen(HWND hWnd)
{
    //フルスクリーンにする処理
    if (!isFullScreen)
    {
        //ウィンドウの枠を削除
        SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

        //現在ウィンドウが表示されているモニターを取得
        HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

        MONITORINFO monitorInfo{};  //モニターの情報構造体
        monitorInfo.cbSize = sizeof(MONITORINFO);   //バイト数をWindows側に通知

        GetMonitorInfo(hMonitor, &monitorInfo); //モニターの情報を構造体に登録

        //モニターの領域全体にウィンドウを拡大する
        SetWindowPos(
            hWnd,
            HWND_TOP,
            monitorInfo.rcMonitor.left,
            monitorInfo.rcMonitor.top,
            monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
            monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
            SWP_FRAMECHANGED
        );

        isFullScreen = true;
        UnlockCursor();
    }
    else
    {
        //ウィンドウの枠を追加する
        SetWindowLong(hWnd, GWL_STYLE,  WS_OVERLAPPEDWINDOW | WS_VISIBLE);

        //クライアント領域の取得
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        //ウィンドウの大きさに戻す
        SetWindowPos(
            hWnd,
            HWND_TOP,
            clientRect.left,
            clientRect.top,
            screen_width,
            screen_height,
            SWP_FRAMECHANGED
        );

        isFullScreen = false;
        LockCursorToWindow(hWnd);
    }
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
#if 0
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;
#endif
    case WM_ACTIVATEAPP:
        //アプリケーションのアクティブ状態
        if (wParam == TRUE)
        {
            //アクティブ時
            LockCursorToWindow(hWnd);
        }
        else
        {
            //非アクティブ時
            UnlockCursor();
        }
        break;
    case WM_ENTERSIZEMOVE:
        //ウィンドウの移動・サイズ変更時
        UnlockCursor();
        break;
    case WM_EXITSIZEMOVE:
        //ウィンドウの移動・サイズ変更終了時
        LockCursorToWindow(hWnd);
        break;
    case WM_MOUSEMOVE:              //マウス座標の設定
        vnDirectInput::setMousePosition(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_KEYDOWN:				//キーが押された
        if (wParam == VK_ESCAPE)	//ESCキー
        {
            PostQuitMessage(0);		//アプリケーションを終了する
        }
        else if (wParam == VK_F11)
        {
            FullScreen(hWnd);   //フルスクリーン
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

#if 0
// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
#endif