/*******************************************************************************************************
 *	ファイル名： speedplay-mci.cpp
 *	ファイルの内容：Speedplay 用の MCIコマンド生成/送信処理コード
 *	作成者：fuwuyuan
 *	修正：
 *	ライセンス/Licensing:
 *	このファイルは、著作者である fuwuyuan によってパブリックドメインに置かれています。
 *	これは全世界に適用されます。
 *		著作物をパブリックドメインに置くことが法律上不可能である場合：
 *	fuwuyuan は、何人に対しても、法律上課される条件以外のいかなる条件も付することなく、
 *	あらゆる目的でこの著作物を利用することを許諾します。
 *
 *	This source code has been released into the public domain by its author, fuwuyuan.
 *  This applies worldwide.
*******************************************************************************************************/

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include "speedplay-mci.h"

/*********************************************************************************
*	関数名：	LPTSTR getPositionReadable()
*	機能：	現在位置を HumanReadable で取得する
*			(mm:ss.sssという形式で)
*	引数:	なし
*	返却値：	LPTSTR
**********************************************************************************/
LPTSTR getPositionReadable() {
	static TCHAR szMCIReturnString[64] = TEXT("");
	UINT intbuf,msbuf,ms,second,minute;

	/*	現在位置の取得	*/
	mciSendString(TEXT("status speedplay position"), szMCIReturnString, 64, NULL);
	/*	UINTへ変換	*/
	intbuf = _ttoi(szMCIReturnString);
	/*	HumanReadableへ変換	*/
	msbuf = intbuf / 1000;
	ms = intbuf % 1000;
	minute = msbuf / 60;
	second = minute * 60;
	second = msbuf - second;
#if _MSC_VER >= 1400	//VisualStudio2005以上
	_stprintf_s(szMCIReturnString,sizeof szMCIReturnString / sizeof szMCIReturnString[0], TEXT("%3u:%2u.%3u"), minute, second, ms);
#else
	_stprintf(szMCIReturnString, TEXT("%3u:%2u.%3u"), minute, second, ms);
#endif // _MSC_VER >= 1400
	return szMCIReturnString;
}

/*********************************************************************************
*	関数名：	void openMCI(LPCTSTR lpstrFile)
*	機能：	MCIを使ってファイルを開く
*	引数:
*		ファイル名( LPCTSTR lpstrFile )
*	返却値：	なし
**********************************************************************************/
void openMCI(LPCTSTR lpstrFile) {
	/*	変数宣言	*/
	TCHAR szCommand[1024] = TEXT("");
	TCHAR szError[128] = TEXT("");
	MCIERROR lMCIError;

	/*	コマンド生成/送出	*/
	_stprintf( szCommand, TEXT("open \"%s\" type MpegVideo alias speedplay"), lpstrFile);
	lMCIError = mciSendString(szCommand, NULL, 0, NULL);

	/*	エラーの場合は、エラー原因を取得して、メッセージボックス表示	*/
	if(lMCIError != 0) {
		mciGetErrorString( lMCIError, szError, 128);
		MessageBox(NULL, szError, TEXT("null"), MB_OK);
	}

}


/*********************************************************************************
*	関数名：	void initMCI(int speed)
*	機能：	時間単位、再生速度などの初期化を行う
*	引数:
*		再生速度( UINT speed )
*	返却値：	なし
**********************************************************************************/
void initMCI(UINT speed) {
	/*	変数宣言	*/
	TCHAR szCommand[1024] = TEXT("");

	/*	時間単位をms(ミリ秒)に設定	*/
	mciSendString( TEXT("set speedplay time format ms"), NULL, 0, NULL);
	/*	再生速度を設定(別のファイルを開いても、前のファイルの速度設定が引きつがれる)	*/
	_stprintf( szCommand, TEXT("set speedplay speed %u"), speed); 
	mciSendString( szCommand, NULL, 0, NULL);
}


/*********************************************************************************
*	関数名：	void change_speedMCI(int speed)
*	機能：	再生速度を変更する
*	引数:
*		再生速度( UINT speed )
*			標準速度:NORMAL_SPEED
*	返却値：	なし
**********************************************************************************/
void change_speedMCI(UINT speed) {
	TCHAR szCommand[64] = TEXT("");

	/*	コマンド生成/送信	*/
	_stprintf( szCommand, TEXT("set speedplay speed %u"), speed);
	mciSendString( szCommand, NULL,0,NULL);
}

/*********************************************************************************
*	関数名：	void overlayDestination(HWND hwnd)
*	機能：	ビデオオーバーレイ表示先を設定する
*	引数:
*		ビデオオーバーレイ表示先ウインドウハンドル( HWND hwnd )
*		オーバーレイの左上x座標(UINT x)
*		オーバーレイの左上y座標(UINT y)
*		オーバーレイの右下x座標(UINT sizeX)
*		オーバーレイの右下y座標(UINT sizeY)
*	返却値：	なし
**********************************************************************************/
void overlayDestination(HWND hwnd,UINT x, UINT y, UINT sizeX, UINT sizeY) {
	TCHAR szCommand[1024] = TEXT("");

	/*	コマンド生成/送信	*/
	_stprintf( szCommand, TEXT("window speedplay handle %u"), (unsigned long long) hwnd); 
	mciSendString( szCommand , NULL, 0, NULL);
	/*	コマンド生成/送信	*/
	_stprintf( szCommand, TEXT("put speedplay destination at %u %u %u %u"), x, y, sizeX, sizeY);
	mciSendString( szCommand , NULL, 0, NULL);
}

/*********************************************************************************
*	関数名：	void seek_incMCI(int ms,int flag)
*	機能：	現在位置からの相対シークを行う
*	引数:
*		送り量(ミリ秒)( int ms )
*		シーク方向フラグ( UINT flag )
*			送り方向:SEEK_FWD
*			戻し方向:SEEK_REV
*	返却値：	なし
**********************************************************************************/
void seek_incMCI(int ms, UINT flag) {
	TCHAR szMCIReturnString[64] = TEXT(""),szCommand[128];
	int position;

	/*	現在位置の取得	*/
	mciSendString(TEXT("status speedplay position"), szMCIReturnString, 64,NULL);
	/*	現在位置をintへ変換	*/
	position = _ttoi(szMCIReturnString);

	/*	送り/戻し判別して分岐	*/
	switch(flag) {
		case SEEK_FWD:
			/*	positionを引数ミリ秒分だけ加算	*/
			position += ms;
			break;
		case SEEK_REV:
			/*	positionを引数ミリ秒分だけ減算	*/
			position -= ms;
			break;
	}

	/*	コマンド生成/送信	*/
	_stprintf(szCommand, TEXT("play speedplay from %u"), position);
	mciSendString(szCommand, NULL, 0, NULL);
}
