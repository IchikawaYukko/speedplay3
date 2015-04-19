#ifndef _SPEEDPLAY_MCI
#define _SPEEDPLAY_MCI

/*******************************************************************************************************
 *	ファイル名： speedplay-mci.h
 *	ファイルの内容：Speedplay の MCIコマンド処理関連のヘッダ
 *	作成者：fuwuyuan/市川ゆり子
 *	修正：
 *	ライセンス/Licensing:
 *	このファイルは、著作者である fuwuyuan/市川ゆり子 によってMIT Licenseのもと公開されています。
 *
 *	Copyright (c) 2015 fuwuyuan/市川ゆり子
 *	This software is released under the MIT License, see LICENSE.txt.
 *	http://opensource.org/licenses/mit-license.php
 *******************************************************************************************************/

/*	シーク方向指定フラグ	*/
#define SEEK_FWD	1	//送り方向	(早送り)
#define SEEK_REV	0	//戻し方向	(巻き戻し)

#define NORMAL_SPEED 1000	//再生速度:標準

void change_speedMCI(UINT);		//再生速度を変更する
LPTSTR getPositionReadable();	//現在位置を HumanReadable で取得する
void initMCI(UINT);				//MCIの初期化を行う
void openMCI(LPCTSTR);			//MCIを使ってファイルを開く
void overlayDestination(HWND, UINT, UINT, UINT, UINT);
									//ビデオオーバーレイ表示先を設定
void seek_incMCI(int, UINT);		//現在位置からの相対シークを行う

#endif // _SPEEDPLAY_MCI
