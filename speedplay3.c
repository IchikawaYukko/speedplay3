/************************************************************************************
 *	Speedplay version 3
 *	ファイル名： speedplay.cpp
 *	ファイルの内容：Speedplay メインプログラム
 *	作成者：fuwuyuan/市川ゆり子
 *	ライセンス/Licensing:
 *	このファイルは、著作者である fuwuyuan/市川ゆり子 によってMIT Licenseのもと公開されています。
 *
 *	Copyright (c) 2015 fuwuyuan/市川ゆり子
 *	This software is released under the MIT License, see LICENSE.txt.
 *	http://opensource.org/licenses/mit-license.php
************************************************************************************/
// TODO シークバーを作る
// TODO ループ再生できるようにする
// TODO ボリュームバー
// TODO タイトルバーに再生中のファイル名表示
// TODO 設定画面[開くと同時に再生　する/しない]

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <commdlg.h>
#include <mmsystem.h>
#include <tchar.h>
#include "speedplay-mci.h"
#include "resource.h"

//ボタンIDを定数定義
#define ID_PLAY		0
#define ID_STOP		1
#define ID_HEAD		2
#define ID_OPEN		3
#define ID_FWD1		4
#define ID_FWD10	5
#define ID_FWD60	6
#define ID_FWD300	7
#define ID_REV1		8
#define ID_REV10	9
#define ID_REV60	10
#define ID_REV300	11
#define ID_SPEED10U	12
#define ID_SPEED1U	13
#define ID_SPEED10D	14
#define ID_SPEED1D	15
#define ID_SPEED100	16
#define ID_FULL		17
#define ID_PAL		18
#define ID_NTSC		19
#define ID_VER		20

#define ID_TIMER			1	//タイマID
#define TIMER_INTERVAL	50	//タイマ割り込み間隔

//構造体：ボタン文字列と位置、ID
struct {
	TCHAR szText[20];
	int		posx;
	int		posy;
	int		id;
} button[] = {
	TEXT("再生"),     0, 48,  ID_PLAY,
	TEXT("停止"),    64, 48,  ID_STOP,
	TEXT("|<--"),   128, 48,  ID_HEAD,
	TEXT("開く"),   192, 48,  ID_OPEN,
	TEXT("1->"),      0, 72,  ID_FWD1,
	TEXT("10->"),    64, 72 , ID_FWD10,
	TEXT("60->"),   128, 72,  ID_FWD60,
	TEXT("300->"),  192, 72,  ID_FWD300,
	TEXT("<-1"),      0, 96,  ID_REV1,
	TEXT("<-10"),    64, 96,  ID_REV10,
	TEXT("<-60"),   128, 96,  ID_REV60,
	TEXT("<-300"),  192, 96,  ID_REV300,
	TEXT("10%↑"),   0, 120, ID_SPEED10U,
	TEXT("1%↑"),    64, 120, ID_SPEED1U,
	TEXT("10%↓"),    0, 144, ID_SPEED10D,
	TEXT("1%↓"),    64, 144, ID_SPEED1D,
	TEXT("100%"),   128, 120, ID_SPEED100,
	TEXT("Full"),     0, 168, ID_FULL,
	TEXT("352x288"), 64, 168, ID_PAL,
	TEXT("352x240"),128, 168, ID_NTSC,
	TEXT("Ver情報"),192, 168, ID_VER,
};

//ボタン数を定数定義
#define BUTTON_NUM (sizeof button / sizeof button[0])


HWND full_hwnd;	//フルスクリーン表示用、ウィンドウハンドル

/*	プロトタイプ宣言	*/
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK FullWndProc (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR szCmdLine, int iCmdShow) {
	static TCHAR szAppName[] = TEXT("Speedplay version 3");
	static TCHAR szAppName2[] = TEXT("FullScreen");
	HWND hwnd;
	MSG msg;
	BOOL bRet;
	WNDCLASS wndclass,full_wndclass;

	/*	ウインドウクラスの定義	*/
	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstance;
	wndclass.hIcon			= NULL;
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH) GetStockObject (BLACK_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= szAppName;
	/*	フルスクリーン用ウインドウクラスの定義	*/
	full_wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	full_wndclass.lpfnWndProc	= FullWndProc;
	full_wndclass.cbClsExtra		= 0;
	full_wndclass.cbWndExtra		= 0;
	full_wndclass.hInstance		= hInstance;
	full_wndclass.hIcon			= NULL;
	full_wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	full_wndclass.hbrBackground	= (HBRUSH) GetStockObject (BLACK_BRUSH);
	full_wndclass.lpszMenuName	= NULL;
	full_wndclass.lpszClassName	= szAppName2;

	/*	ウインドウクラスの登録	*/
	if (!RegisterClass (&wndclass)) {
		MessageBox(NULL, TEXT("Program requires WindowsNT!"),
					szAppName, MB_ICONERROR);
		return 0;
	}
	/*	フルスクリーン用ウインドウクラスの登録	*/
	if (!RegisterClass (&full_wndclass)) {
		MessageBox(NULL, TEXT("Program requires WindowsNT!"),
					szAppName, MB_ICONERROR);
		return 0;
	}

	/*	メインウィンドウの作成	*/
	hwnd = CreateWindow (szAppName, TEXT("Speedplay version 3.01"),
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,CW_USEDEFAULT,
						608 + 8,288 + 34,
						NULL, NULL, hInstance, NULL);
	/*	フルスクリーン用ウィンドウの作成	*/
	full_hwnd = CreateWindow (szAppName2, TEXT("Speedplay version 3.01"),
						WS_POPUP,
						0,0,
						GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CXSCREEN),
						NULL, NULL, hInstance, NULL);
	/*	作成したウィンドウの表示	*/
	ShowWindow(hwnd, iCmdShow);

	/*	メッセージループ	*/
	while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0) { 
	    if (bRet == -1) {
	        // handle the error and possibly exit
	    } else {
	        TranslateMessage(&msg); 
	        DispatchMessage(&msg); 
	    }
	}
	return (int) msg.wParam;
}

/*	ウィンドウプロシージャ	*/
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HWND hwndButton[BUTTON_NUM];	//ボタンハンドル
	static HWND hwndStaticSpeed;			//速度表示部　スタティックコントロールハンドル
	static HWND hwndStaticTime;			//時間表示部　スタティックコントロールハンドル
	HDROP hdrop;
	/*	ファイル選択ダイアログ用、フィルター文字列	*/
	static TCHAR szFilter[] = TEXT("MediaFile\0*.dat;*.wav;*.mp3;*.mpg;*.m1v\0")	 \
							  TEXT("Video-CD (*.dat)\0*.dat\0") \
							  TEXT("MP3 Audio (*.mp3)\0*.mp3\0") \
							  TEXT("Wave Audio (*.wav)\0*.wav\0") \
							  TEXT("MPEG-1 Video (*.mpg)\0*.mpg;*.m1v\0") \
							  TEXT("AllFiles (*.*)\0*.*\0\0");
	static UINT speed;	//再生速度保持変数
	int i;
	TCHAR szTimerBuffer[32] = TEXT("");	//タイマ呼び出し時、文字列生成用バッファ
	TCHAR szFile[MAX_PATH] = TEXT("");	//ファイル名格納用
	OPENFILENAME file;					//「ファイルを開く」ダイアログ設定構造体
	/*	文字列生成	*/
	TCHAR szVerinfo[] = \
	TEXT("Speedplay Version 3.01\nCoded by fuwuyuan/市川ゆり子 2001-2015\n\n") \
	TEXT("This software is released under the MIT License, see LICENSE.txt.\n");
	/*	メッセージによって分岐	*/
	switch(message) {
		case WM_CREATE :
			speed = NORMAL_SPEED;	//再生速度保持変数を初期化

			/*	ボタンを作成	*/
			for (i = 0;i < BUTTON_NUM;i++) {
				hwndButton[i] = CreateWindow (TEXT("button"),
									button[i].szText,
									WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
									button[i].posx, button[i].posy,
									64,24,
									hwnd,
									(HMENU) (INT_PTR) button[i].id,
									((LPCREATESTRUCT) lParam)->hInstance,
									NULL);
			}
			/*	速度表示部(スタティックコントロール)を作成	*/
			hwndStaticSpeed = CreateWindow (TEXT("static"),
								TEXT("100％"),
								WS_CHILD | WS_VISIBLE | SS_LEFT,
								0, 0,
								128,48,
								hwnd,
								NULL,
								((LPCREATESTRUCT) lParam)->hInstance,
								NULL);
			/*	時間表示部(スタティックコントロール)を作成	*/
			hwndStaticTime = CreateWindow (TEXT("static"),
								TEXT("  0: 0.000"),
								WS_CHILD | WS_VISIBLE | SS_LEFT,
								128, 0,
								128,48,
								hwnd,
								NULL,
								((LPCREATESTRUCT) lParam)->hInstance,
								NULL);
			/*	タイマー作成	*/
			SetTimer(hwnd, ID_TIMER, TIMER_INTERVAL, NULL);
			DragAcceptFiles(hwnd, TRUE);	//ドラッグアンドドロップ有効化
			return 0;
		case WM_DROPFILES:
			hdrop = (HDROP) wParam;
			DragQueryFile(hdrop, 0, szFile, sizeof(szFile));
			mciSendString( TEXT("close speedplay"),NULL,0,NULL); //	現在開いているデバイスを閉じる
			/*	ダイアログで選択されたファイルを開く	*/
			openMCI(szFile);
			/*	ビデオ表示先ウインドウハンドルを設定	*/
			/*	ビデオ表示サイズを352x288(PAL-VideoCD)に設定	*/
			overlayDestination(hwnd, 256, 0, 352, 288);
			initMCI(speed);	//MCI周りの初期化を行う
			mciSendString( TEXT("play speedplay"),NULL,0,NULL);
			DragFinish(hdrop);
			return 0;
		case WM_TIMER :
			/*	再生速度文字列生成	*/

//#define _stprintf_s _stprintf

//#if _MSC_VER >= 1400	//VisualStudio2005以上
			_stprintf_s( szTimerBuffer, sizeof szTimerBuffer / sizeof szTimerBuffer[0], TEXT("%u%%"), speed / 10);
			//#else
			//_stprintf( szTimerBuffer, TEXT("%u%%"), speed / 10);
//#endif // _MSC_VER >= 1400
			/*	再生速度表示	*/
			SetWindowText( hwndStaticSpeed, szTimerBuffer);
			/*	再生位置表示	*/
			SetWindowText( hwndStaticTime, getPositionReadable());
			return 0;
		case WM_SIZE :
		case WM_PAINT :
			break;
		case WM_KEYUP :
			switch(wParam) {
				case VK_SPACE:
					//スペースキーで頭出し
					// TODO スペースで再生、停止できるように改良
					mciSendString( TEXT("play speedplay from 0"),NULL,0,NULL);
					break;
			}
		case WM_COMMAND :
			switch( LOWORD(wParam) ) {
				/*	再生ボタン	*/
				case ID_PLAY :
					/*	再生コマンド送信	*/
					mciSendString( TEXT("play speedplay"),NULL,0,NULL);

					break;
				/*	停止ボタン	*/
				case ID_STOP :
					/*	停止コマンド送信	*/
					mciSendString( TEXT("stop speedplay"),NULL,0,NULL);

					break;
				/*	頭出しボタン	*/
				case ID_HEAD :
					/*	頭出しコマンド送信	*/
					mciSendString( TEXT("play speedplay from 0"),NULL,0,NULL);

					break;
				/*	開くボタン	*/
				case ID_OPEN :
					/*	ファイルを開くダイアログ用構造体初期化	*/
					file.lStructSize		= sizeof (OPENFILENAME);
					file.hwndOwner			= hwnd;
					file.hInstance			= NULL;
					file.lpstrFilter		= szFilter;
					file.lpstrCustomFilter	= NULL;
					file.nMaxCustFilter		= 0;
					file.nFilterIndex		= 0;
					file.lpstrFile			= szFile;
					file.nMaxFile			= MAX_PATH;
					file.lpstrFileTitle		= NULL;
					file.nMaxFileTitle		= MAX_PATH;
					file.lpstrInitialDir	= NULL;
					file.lpstrTitle			= NULL;
					file.Flags				= OFN_CREATEPROMPT;
					file.nFileOffset		= 0;
					file.nFileExtension		= 0;
					file.lpstrDefExt		= TEXT("mp3");
					file.lCustData			= 0L;
					file.lpfnHook			= NULL;
					file.lpTemplateName		= NULL;

					/*	ファイル選択ダイアログを表示	*/
					/*	キャンセルが押されたら、break;	*/
					if ( GetOpenFileName(&file) != TRUE ) break;

					/*	現在開いているデバイスを閉じる	*/
					mciSendString( TEXT("close speedplay"),NULL,0,NULL);

					/*	ダイアログで選択されたファイルを開く	*/
					openMCI(szFile);
					/*	ビデオ表示先ウインドウハンドルを設定	*/
					/*	ビデオ表示サイズを352x288(PAL-VideoCD)に設定	*/
					overlayDestination(hwnd, 256, 0, 352, 288);
					/*	MCI周りの初期化を行う	*/
					initMCI(speed);
					break;
				/*	1秒送りボタン	*/
				case ID_FWD1:
					/*	シーク	*/
					seek_incMCI(1000, SEEK_FWD);
					break;
				/*	10秒送りボタン	*/
				case ID_FWD10:
					seek_incMCI(10000, SEEK_FWD);
					break;
				/*	60秒送りボタン	*/
				case ID_FWD60:
					seek_incMCI(60000, SEEK_FWD);
					break;
				/*	300秒送りボタン	*/
				case ID_FWD300:
					seek_incMCI(300000, SEEK_FWD);
					break;
				/*	1秒戻しボタン	*/
				case ID_REV1:
					seek_incMCI(1000, SEEK_REV);
					break;
				/*	10秒戻しボタン	*/
				case ID_REV10:
					seek_incMCI(10000, SEEK_REV);
					break;
				/*	60秒戻しボタン	*/
				case ID_REV60:
					seek_incMCI(60000, SEEK_REV);
					break;
				/*	300秒戻しボタン	*/
				case ID_REV300:
					seek_incMCI(300000, SEEK_REV);
					break;
				/*	再生速度＋10％ボタン	*/
				case ID_SPEED10U:
					/*	再生速度保持変数を設定	*/
					speed += 100;
					/*	再生速度を設定	*/
					change_speedMCI(speed);
					break;
				/*	再生速度＋1％ボタン	*/
				case ID_SPEED1U:
					speed += 10;
					change_speedMCI(speed);
					break;
				/*	再生速度ー10％ボタン	*/
				case ID_SPEED10D:
					speed -= 100;
					change_speedMCI(speed);
					break;
				/*	再生速度ー1％ボタン	*/
				case ID_SPEED1D:
					speed -= 10;
					change_speedMCI(speed);
					break;
				/*	再生速度100％ボタン	*/
				case ID_SPEED100:
					speed = NORMAL_SPEED;
					change_speedMCI(NORMAL_SPEED);
					break;
				/*	フルスクリーン表示ボタン	*/
				case ID_FULL:
					/*	フルスクリーン用ウィンドウ表示	*/
					ShowWindow(full_hwnd, SW_SHOW);
					/*	オーバーレイ表示先を変更	*/
					overlayDestination(full_hwnd, 0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
					break;
				/*	352x288表示ボタン	*/
				case ID_PAL:
					/*	フルスクリーン用ウィンドウ非表示	*/
					ShowWindow(full_hwnd, SW_HIDE);
					/*	オーバーレイ表示先を変更	*/
					overlayDestination(hwnd, 256,0,352,288);
					break;
				/*	352x240表示ボタン	*/
				case ID_NTSC:
					/*	フルスクリーン用ウィンドウ非表示	*/
					ShowWindow(full_hwnd, SW_HIDE);
					/*	オーバーレイ表示先を変更	*/
					overlayDestination(hwnd, 256,24,352,240);
					break;
				/*	バージョン情報ボタン	*/
				case ID_VER:
					/*	ダイアログ表示	*/
					MessageBox(hwnd, szVerinfo, TEXT("Speedplay Version 3.01"), MB_ICONINFORMATION | MB_OK);
					break;
			}
			return 0;
		case WM_DESTROY :
			/*	終了処理	*/
			mciSendString( TEXT("close speedplay"),NULL,0,NULL);
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/*	フルスクリーン用　ウィンドウプロシージャ	*/
LRESULT CALLBACK FullWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, message, wParam, lParam);
}
