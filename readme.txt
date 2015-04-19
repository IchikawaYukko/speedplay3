
		Speedplay Version 3.1

	再生速度変更に特化したメディアプレーヤー。

１ 動作環境------------------------------------------------------
	OS:	Windows 2000
		Windows XP
		Windows Server 2003
		Windows 7
		Windows 10 TP Build 10041
		（確認しているもののみ）

	一部のサウンドカードのチップセットと音声サンプリングレートでは、
	再生速度変更が正しく動作しません。

	動作を確認しているチップセット：
	Cirrus Logic CS4281
	C-Media CMI8738/C3DX
	  ↑(サンプリングレートが44100KHzの場合、再生速度を100.1%以上にすると、音が出なくなる)
	ESS ES1879
	Realtek AC'97 Audio

２ 開発環境------------------------------------------------------
	OS:Windows 10 TP Build 10041
	メモリー:4GB
	CPU:Core i3 3120M
	機種:パソコン工房 15P3000-i3-TRB

３ 開発言語------------------------------------------------------
	Microsoft Visual Studio 2010 ja-JP
		(Visual C++ 2010)

４ インストール------------------------------------------------------
	アーカイブを解凍して、適当なフォルダにぶっこんで下さい。
	このファイルが読めるということは、もう解凍できていますね？

５ アンインストール------------------------------------------------------
	解凍した時にできたファイルを全て消してください。
	レジストリはいじっていません。

６ 使用法------------------------------------------------------
	speedplay3.exe をダブルクリックして起動してください。
	実行ファイル単体で動作します。

	再生可能ファイル：
	Wave Audio (Wav)
	Mpeg Layer-2 (mp2)
	Mpeg Layer-3 (mp3)
	Windows Media Video (WMV) 要 Windows Media Player 6.4以上
	Windows Media Audio (WMA) 要 Windows Media Player 6.4以上
	MpegVideo (Mpg,Mpeg,m1v,asf)
	Video for Windows (Avi)

	各ボタンの説明：
	[再生]	再生
	[停止]	一時停止
	[|<--]	頭出し
	[開く]	ファイルを開く
	[1->]	1秒進める
	[10->]	10秒進める
	[60->]	60秒進める
	[300->]	300秒進める
	[<-1]	1秒戻す
	[<-10]	10秒戻す
	[<-60]	60秒戻す
	[<-300]	300秒戻す
	[10%↑]	再生速度 10% アップ
	[1%↑]	再生速度 1% アップ
	[0.1%↑]再生速度 0.1% アップ
	[10%↓]	再生速度 10% ダウン
	[1%↓]	再生速度 1% ダウン
	[0.1%↓]再生速度 0.1% ダウン
	[100%]	もとの再生速度に戻す
	[ﾌﾙｽｸﾘｰﾝ]	フルスクリーン表示
	[352x288]	画面サイズを352x288(PAL)にする
	[352x240]	画面サイズを352x240(NTSC)にする
	[Ver情報]	バージョン情報

７ ライセンス------------------------------------------------------
	このソフトウェアは、著作者である fuwuyuan/市川ゆり子 によってMIT Licenseのもと公開されています。

	Copyright (c) 2015 fuwuyuan/市川ゆり子
	This software is released under the MIT License, see LICENSE.txt.
	http://opensource.org/licenses/mit-license.php

	Copyright (c) fuwuyuan/市川ゆり子
	以下の条件を満たす限り、自由な複製・配布・修正を無制限に許可する。
	・上記の著作権表示と許諾書を、ソフトウェアの複製または重要な部分に記載する
	・本ソフトウェアは無保証である。自己責任で使用する。

	偶発、あるいは故意に関わらず、当アプリケーションを使用したことにより直接、
	あるいは間接的に発生した問題により利用者等が被るあらゆる損失、不利益はアプリケーション開発者は補償いたしません。

	Copyright (c) 2015 fuwuyuan/市川ゆり子
	This software is released under the MIT License, see LICENSE.txt.
	http://opensource.org/licenses/mit-license.php

	雑誌の CD-ROM 等に付属させる場合には、連絡を下さい。

											fuwuyuan. 2015 Apr 19

８ 作者について------------------------------------------------------
作者名:fuwuyuan/市川ゆり子

バグ、感想、質問、意見などいろいろ、下記のアドレスまでどうぞ。
E-Mail:ichikawayuriko あっとまーく yahoo.co.jp
Twitter:@IchikawaYukko
Homepage:http://IchikawaYukko.mydns.jp/

９ バージョンアップ履歴------------------------------------------------------

Ver 3.01
パブリックドメインからMIT Licenseに変更
GitHubにて公開

Ver 3.0
C言語に移植（Ver 2 まではHSPで開発していた）
ユニコードファイル名に対応

Ver 2.0
ソフト名を「Speedplay」に変更

Ver 1.0
初版
「speedplay.exe」という、暫定的なソフト名だった。