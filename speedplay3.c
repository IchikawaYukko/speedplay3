/************************************************************************************
 *	Speedplay version 3
 *	�t�@�C�����F speedplay.cpp
 *	�t�@�C���̓��e�FSpeedplay ���C���v���O����
 *	�쐬�ҁFfuwuyuan/�s����q
 *	���C�Z���X/Licensing:
 *	���̃t�@�C���́A����҂ł��� fuwuyuan/�s����q �ɂ����MIT License�̂��ƌ��J����Ă��܂��B
 *
 *	Copyright (c) 2015 fuwuyuan/�s����q
 *	This software is released under the MIT License, see LICENSE.txt.
 *	http://opensource.org/licenses/mit-license.php
************************************************************************************/
// TODO �V�[�N�o�[�����
// TODO ���[�v�Đ��ł���悤�ɂ���
// TODO �{�����[���o�[
// TODO �^�C�g���o�[�ɍĐ����̃t�@�C�����\��
// TODO �ݒ���[�J���Ɠ����ɍĐ��@����/���Ȃ�]

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <commdlg.h>
#include <mmsystem.h>
#include <tchar.h>
#include "speedplay-mci.h"
#include "resource.h"

//�{�^��ID��萔��`
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

#define ID_TIMER			1	//�^�C�}ID
#define TIMER_INTERVAL	50	//�^�C�}���荞�݊Ԋu

//�\���́F�{�^��������ƈʒu�AID
struct {
	TCHAR szText[20];
	int		posx;
	int		posy;
	int		id;
} button[] = {
	TEXT("�Đ�"),     0, 48,  ID_PLAY,
	TEXT("��~"),    64, 48,  ID_STOP,
	TEXT("|<--"),   128, 48,  ID_HEAD,
	TEXT("�J��"),   192, 48,  ID_OPEN,
	TEXT("1->"),      0, 72,  ID_FWD1,
	TEXT("10->"),    64, 72 , ID_FWD10,
	TEXT("60->"),   128, 72,  ID_FWD60,
	TEXT("300->"),  192, 72,  ID_FWD300,
	TEXT("<-1"),      0, 96,  ID_REV1,
	TEXT("<-10"),    64, 96,  ID_REV10,
	TEXT("<-60"),   128, 96,  ID_REV60,
	TEXT("<-300"),  192, 96,  ID_REV300,
	TEXT("10%��"),   0, 120, ID_SPEED10U,
	TEXT("1%��"),    64, 120, ID_SPEED1U,
	TEXT("10%��"),    0, 144, ID_SPEED10D,
	TEXT("1%��"),    64, 144, ID_SPEED1D,
	TEXT("100%"),   128, 120, ID_SPEED100,
	TEXT("Full"),     0, 168, ID_FULL,
	TEXT("352x288"), 64, 168, ID_PAL,
	TEXT("352x240"),128, 168, ID_NTSC,
	TEXT("Ver���"),192, 168, ID_VER,
};

//�{�^������萔��`
#define BUTTON_NUM (sizeof button / sizeof button[0])


HWND full_hwnd;	//�t���X�N���[���\���p�A�E�B���h�E�n���h��

/*	�v���g�^�C�v�錾	*/
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

	/*	�E�C���h�E�N���X�̒�`	*/
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
	/*	�t���X�N���[���p�E�C���h�E�N���X�̒�`	*/
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

	/*	�E�C���h�E�N���X�̓o�^	*/
	if (!RegisterClass (&wndclass)) {
		MessageBox(NULL, TEXT("Program requires WindowsNT!"),
					szAppName, MB_ICONERROR);
		return 0;
	}
	/*	�t���X�N���[���p�E�C���h�E�N���X�̓o�^	*/
	if (!RegisterClass (&full_wndclass)) {
		MessageBox(NULL, TEXT("Program requires WindowsNT!"),
					szAppName, MB_ICONERROR);
		return 0;
	}

	/*	���C���E�B���h�E�̍쐬	*/
	hwnd = CreateWindow (szAppName, TEXT("Speedplay version 3.01"),
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,CW_USEDEFAULT,
						608 + 8,288 + 34,
						NULL, NULL, hInstance, NULL);
	/*	�t���X�N���[���p�E�B���h�E�̍쐬	*/
	full_hwnd = CreateWindow (szAppName2, TEXT("Speedplay version 3.01"),
						WS_POPUP,
						0,0,
						GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CXSCREEN),
						NULL, NULL, hInstance, NULL);
	/*	�쐬�����E�B���h�E�̕\��	*/
	ShowWindow(hwnd, iCmdShow);

	/*	���b�Z�[�W���[�v	*/
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

/*	�E�B���h�E�v���V�[�W��	*/
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HWND hwndButton[BUTTON_NUM];	//�{�^���n���h��
	static HWND hwndStaticSpeed;			//���x�\�����@�X�^�e�B�b�N�R���g���[���n���h��
	static HWND hwndStaticTime;			//���ԕ\�����@�X�^�e�B�b�N�R���g���[���n���h��
	HDROP hdrop;
	/*	�t�@�C���I���_�C�A���O�p�A�t�B���^�[������	*/
	static TCHAR szFilter[] = TEXT("MediaFile\0*.dat;*.wav;*.mp3;*.mpg;*.m1v\0")	 \
							  TEXT("Video-CD (*.dat)\0*.dat\0") \
							  TEXT("MP3 Audio (*.mp3)\0*.mp3\0") \
							  TEXT("Wave Audio (*.wav)\0*.wav\0") \
							  TEXT("MPEG-1 Video (*.mpg)\0*.mpg;*.m1v\0") \
							  TEXT("AllFiles (*.*)\0*.*\0\0");
	static UINT speed;	//�Đ����x�ێ��ϐ�
	int i;
	TCHAR szTimerBuffer[32] = TEXT("");	//�^�C�}�Ăяo�����A�����񐶐��p�o�b�t�@
	TCHAR szFile[MAX_PATH] = TEXT("");	//�t�@�C�����i�[�p
	OPENFILENAME file;					//�u�t�@�C�����J���v�_�C�A���O�ݒ�\����
	/*	�����񐶐�	*/
	TCHAR szVerinfo[] = \
	TEXT("Speedplay Version 3.01\nCoded by fuwuyuan/�s����q 2001-2015\n\n") \
	TEXT("This software is released under the MIT License, see LICENSE.txt.\n");
	/*	���b�Z�[�W�ɂ���ĕ���	*/
	switch(message) {
		case WM_CREATE :
			speed = NORMAL_SPEED;	//�Đ����x�ێ��ϐ���������

			/*	�{�^�����쐬	*/
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
			/*	���x�\����(�X�^�e�B�b�N�R���g���[��)���쐬	*/
			hwndStaticSpeed = CreateWindow (TEXT("static"),
								TEXT("100��"),
								WS_CHILD | WS_VISIBLE | SS_LEFT,
								0, 0,
								128,48,
								hwnd,
								NULL,
								((LPCREATESTRUCT) lParam)->hInstance,
								NULL);
			/*	���ԕ\����(�X�^�e�B�b�N�R���g���[��)���쐬	*/
			hwndStaticTime = CreateWindow (TEXT("static"),
								TEXT("  0: 0.000"),
								WS_CHILD | WS_VISIBLE | SS_LEFT,
								128, 0,
								128,48,
								hwnd,
								NULL,
								((LPCREATESTRUCT) lParam)->hInstance,
								NULL);
			/*	�^�C�}�[�쐬	*/
			SetTimer(hwnd, ID_TIMER, TIMER_INTERVAL, NULL);
			DragAcceptFiles(hwnd, TRUE);	//�h���b�O�A���h�h���b�v�L����
			return 0;
		case WM_DROPFILES:
			hdrop = (HDROP) wParam;
			DragQueryFile(hdrop, 0, szFile, sizeof(szFile));
			mciSendString( TEXT("close speedplay"),NULL,0,NULL); //	���݊J���Ă���f�o�C�X�����
			/*	�_�C�A���O�őI�����ꂽ�t�@�C�����J��	*/
			openMCI(szFile);
			/*	�r�f�I�\����E�C���h�E�n���h����ݒ�	*/
			/*	�r�f�I�\���T�C�Y��352x288(PAL-VideoCD)�ɐݒ�	*/
			overlayDestination(hwnd, 256, 0, 352, 288);
			initMCI(speed);	//MCI����̏��������s��
			mciSendString( TEXT("play speedplay"),NULL,0,NULL);
			DragFinish(hdrop);
			return 0;
		case WM_TIMER :
			/*	�Đ����x�����񐶐�	*/

//#define _stprintf_s _stprintf

//#if _MSC_VER >= 1400	//VisualStudio2005�ȏ�
			_stprintf_s( szTimerBuffer, sizeof szTimerBuffer / sizeof szTimerBuffer[0], TEXT("%u%%"), speed / 10);
			//#else
			//_stprintf( szTimerBuffer, TEXT("%u%%"), speed / 10);
//#endif // _MSC_VER >= 1400
			/*	�Đ����x�\��	*/
			SetWindowText( hwndStaticSpeed, szTimerBuffer);
			/*	�Đ��ʒu�\��	*/
			SetWindowText( hwndStaticTime, getPositionReadable());
			return 0;
		case WM_SIZE :
		case WM_PAINT :
			break;
		case WM_KEYUP :
			switch(wParam) {
				case VK_SPACE:
					//�X�y�[�X�L�[�œ��o��
					// TODO �X�y�[�X�ōĐ��A��~�ł���悤�ɉ���
					mciSendString( TEXT("play speedplay from 0"),NULL,0,NULL);
					break;
			}
		case WM_COMMAND :
			switch( LOWORD(wParam) ) {
				/*	�Đ��{�^��	*/
				case ID_PLAY :
					/*	�Đ��R�}���h���M	*/
					mciSendString( TEXT("play speedplay"),NULL,0,NULL);

					break;
				/*	��~�{�^��	*/
				case ID_STOP :
					/*	��~�R�}���h���M	*/
					mciSendString( TEXT("stop speedplay"),NULL,0,NULL);

					break;
				/*	���o���{�^��	*/
				case ID_HEAD :
					/*	���o���R�}���h���M	*/
					mciSendString( TEXT("play speedplay from 0"),NULL,0,NULL);

					break;
				/*	�J���{�^��	*/
				case ID_OPEN :
					/*	�t�@�C�����J���_�C�A���O�p�\���̏�����	*/
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

					/*	�t�@�C���I���_�C�A���O��\��	*/
					/*	�L�����Z���������ꂽ��Abreak;	*/
					if ( GetOpenFileName(&file) != TRUE ) break;

					/*	���݊J���Ă���f�o�C�X�����	*/
					mciSendString( TEXT("close speedplay"),NULL,0,NULL);

					/*	�_�C�A���O�őI�����ꂽ�t�@�C�����J��	*/
					openMCI(szFile);
					/*	�r�f�I�\����E�C���h�E�n���h����ݒ�	*/
					/*	�r�f�I�\���T�C�Y��352x288(PAL-VideoCD)�ɐݒ�	*/
					overlayDestination(hwnd, 256, 0, 352, 288);
					/*	MCI����̏��������s��	*/
					initMCI(speed);
					break;
				/*	1�b����{�^��	*/
				case ID_FWD1:
					/*	�V�[�N	*/
					seek_incMCI(1000, SEEK_FWD);
					break;
				/*	10�b����{�^��	*/
				case ID_FWD10:
					seek_incMCI(10000, SEEK_FWD);
					break;
				/*	60�b����{�^��	*/
				case ID_FWD60:
					seek_incMCI(60000, SEEK_FWD);
					break;
				/*	300�b����{�^��	*/
				case ID_FWD300:
					seek_incMCI(300000, SEEK_FWD);
					break;
				/*	1�b�߂��{�^��	*/
				case ID_REV1:
					seek_incMCI(1000, SEEK_REV);
					break;
				/*	10�b�߂��{�^��	*/
				case ID_REV10:
					seek_incMCI(10000, SEEK_REV);
					break;
				/*	60�b�߂��{�^��	*/
				case ID_REV60:
					seek_incMCI(60000, SEEK_REV);
					break;
				/*	300�b�߂��{�^��	*/
				case ID_REV300:
					seek_incMCI(300000, SEEK_REV);
					break;
				/*	�Đ����x�{10���{�^��	*/
				case ID_SPEED10U:
					/*	�Đ����x�ێ��ϐ���ݒ�	*/
					speed += 100;
					/*	�Đ����x��ݒ�	*/
					change_speedMCI(speed);
					break;
				/*	�Đ����x�{1���{�^��	*/
				case ID_SPEED1U:
					speed += 10;
					change_speedMCI(speed);
					break;
				/*	�Đ����x�[10���{�^��	*/
				case ID_SPEED10D:
					speed -= 100;
					change_speedMCI(speed);
					break;
				/*	�Đ����x�[1���{�^��	*/
				case ID_SPEED1D:
					speed -= 10;
					change_speedMCI(speed);
					break;
				/*	�Đ����x100���{�^��	*/
				case ID_SPEED100:
					speed = NORMAL_SPEED;
					change_speedMCI(NORMAL_SPEED);
					break;
				/*	�t���X�N���[���\���{�^��	*/
				case ID_FULL:
					/*	�t���X�N���[���p�E�B���h�E�\��	*/
					ShowWindow(full_hwnd, SW_SHOW);
					/*	�I�[�o�[���C�\�����ύX	*/
					overlayDestination(full_hwnd, 0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
					break;
				/*	352x288�\���{�^��	*/
				case ID_PAL:
					/*	�t���X�N���[���p�E�B���h�E��\��	*/
					ShowWindow(full_hwnd, SW_HIDE);
					/*	�I�[�o�[���C�\�����ύX	*/
					overlayDestination(hwnd, 256,0,352,288);
					break;
				/*	352x240�\���{�^��	*/
				case ID_NTSC:
					/*	�t���X�N���[���p�E�B���h�E��\��	*/
					ShowWindow(full_hwnd, SW_HIDE);
					/*	�I�[�o�[���C�\�����ύX	*/
					overlayDestination(hwnd, 256,24,352,240);
					break;
				/*	�o�[�W�������{�^��	*/
				case ID_VER:
					/*	�_�C�A���O�\��	*/
					MessageBox(hwnd, szVerinfo, TEXT("Speedplay Version 3.01"), MB_ICONINFORMATION | MB_OK);
					break;
			}
			return 0;
		case WM_DESTROY :
			/*	�I������	*/
			mciSendString( TEXT("close speedplay"),NULL,0,NULL);
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/*	�t���X�N���[���p�@�E�B���h�E�v���V�[�W��	*/
LRESULT CALLBACK FullWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, message, wParam, lParam);
}
