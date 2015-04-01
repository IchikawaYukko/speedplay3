/*******************************************************************************************************
 *	�t�@�C�����F speedplay-mci.cpp
 *	�t�@�C���̓��e�FSpeedplay �p�� MCI�R�}���h����/���M�����R�[�h
 *	�쐬�ҁFfuwuyuan
 *	�C���F
 *	���C�Z���X/Licensing:
 *	���̃t�@�C���́A����҂ł��� fuwuyuan �ɂ���ăp�u���b�N�h���C���ɒu����Ă��܂��B
 *	����͑S���E�ɓK�p����܂��B
 *		���앨���p�u���b�N�h���C���ɒu�����Ƃ��@����s�\�ł���ꍇ�F
 *	fuwuyuan �́A���l�ɑ΂��Ă��A�@����ۂ��������ȊO�̂����Ȃ�������t���邱�ƂȂ��A
 *	������ړI�ł��̒��앨�𗘗p���邱�Ƃ��������܂��B
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
*	�֐����F	LPTSTR getPositionReadable()
*	�@�\�F	���݈ʒu�� HumanReadable �Ŏ擾����
*			(mm:ss.sss�Ƃ����`����)
*	����:	�Ȃ�
*	�ԋp�l�F	LPTSTR
**********************************************************************************/
LPTSTR getPositionReadable() {
	static TCHAR szMCIReturnString[64] = TEXT("");
	UINT intbuf,msbuf,ms,second,minute;

	/*	���݈ʒu�̎擾	*/
	mciSendString(TEXT("status speedplay position"), szMCIReturnString, 64, NULL);
	/*	UINT�֕ϊ�	*/
	intbuf = _ttoi(szMCIReturnString);
	/*	HumanReadable�֕ϊ�	*/
	msbuf = intbuf / 1000;
	ms = intbuf % 1000;
	minute = msbuf / 60;
	second = minute * 60;
	second = msbuf - second;
#if _MSC_VER >= 1400	//VisualStudio2005�ȏ�
	_stprintf_s(szMCIReturnString,sizeof szMCIReturnString / sizeof szMCIReturnString[0], TEXT("%3u:%2u.%3u"), minute, second, ms);
#else
	_stprintf(szMCIReturnString, TEXT("%3u:%2u.%3u"), minute, second, ms);
#endif // _MSC_VER >= 1400
	return szMCIReturnString;
}

/*********************************************************************************
*	�֐����F	void openMCI(LPCTSTR lpstrFile)
*	�@�\�F	MCI���g���ăt�@�C�����J��
*	����:
*		�t�@�C����( LPCTSTR lpstrFile )
*	�ԋp�l�F	�Ȃ�
**********************************************************************************/
void openMCI(LPCTSTR lpstrFile) {
	/*	�ϐ��錾	*/
	TCHAR szCommand[1024] = TEXT("");
	TCHAR szError[128] = TEXT("");
	MCIERROR lMCIError;

	/*	�R�}���h����/���o	*/
	_stprintf( szCommand, TEXT("open \"%s\" type MpegVideo alias speedplay"), lpstrFile);
	lMCIError = mciSendString(szCommand, NULL, 0, NULL);

	/*	�G���[�̏ꍇ�́A�G���[�������擾���āA���b�Z�[�W�{�b�N�X�\��	*/
	if(lMCIError != 0) {
		mciGetErrorString( lMCIError, szError, 128);
		MessageBox(NULL, szError, TEXT("null"), MB_OK);
	}

}


/*********************************************************************************
*	�֐����F	void initMCI(int speed)
*	�@�\�F	���ԒP�ʁA�Đ����x�Ȃǂ̏��������s��
*	����:
*		�Đ����x( UINT speed )
*	�ԋp�l�F	�Ȃ�
**********************************************************************************/
void initMCI(UINT speed) {
	/*	�ϐ��錾	*/
	TCHAR szCommand[1024] = TEXT("");

	/*	���ԒP�ʂ�ms(�~���b)�ɐݒ�	*/
	mciSendString( TEXT("set speedplay time format ms"), NULL, 0, NULL);
	/*	�Đ����x��ݒ�(�ʂ̃t�@�C�����J���Ă��A�O�̃t�@�C���̑��x�ݒ肪���������)	*/
	_stprintf( szCommand, TEXT("set speedplay speed %u"), speed); 
	mciSendString( szCommand, NULL, 0, NULL);
}


/*********************************************************************************
*	�֐����F	void change_speedMCI(int speed)
*	�@�\�F	�Đ����x��ύX����
*	����:
*		�Đ����x( UINT speed )
*			�W�����x:NORMAL_SPEED
*	�ԋp�l�F	�Ȃ�
**********************************************************************************/
void change_speedMCI(UINT speed) {
	TCHAR szCommand[64] = TEXT("");

	/*	�R�}���h����/���M	*/
	_stprintf( szCommand, TEXT("set speedplay speed %u"), speed);
	mciSendString( szCommand, NULL,0,NULL);
}

/*********************************************************************************
*	�֐����F	void overlayDestination(HWND hwnd)
*	�@�\�F	�r�f�I�I�[�o�[���C�\�����ݒ肷��
*	����:
*		�r�f�I�I�[�o�[���C�\����E�C���h�E�n���h��( HWND hwnd )
*		�I�[�o�[���C�̍���x���W(UINT x)
*		�I�[�o�[���C�̍���y���W(UINT y)
*		�I�[�o�[���C�̉E��x���W(UINT sizeX)
*		�I�[�o�[���C�̉E��y���W(UINT sizeY)
*	�ԋp�l�F	�Ȃ�
**********************************************************************************/
void overlayDestination(HWND hwnd,UINT x, UINT y, UINT sizeX, UINT sizeY) {
	TCHAR szCommand[1024] = TEXT("");

	/*	�R�}���h����/���M	*/
	_stprintf( szCommand, TEXT("window speedplay handle %u"), (unsigned long long) hwnd); 
	mciSendString( szCommand , NULL, 0, NULL);
	/*	�R�}���h����/���M	*/
	_stprintf( szCommand, TEXT("put speedplay destination at %u %u %u %u"), x, y, sizeX, sizeY);
	mciSendString( szCommand , NULL, 0, NULL);
}

/*********************************************************************************
*	�֐����F	void seek_incMCI(int ms,int flag)
*	�@�\�F	���݈ʒu����̑��΃V�[�N���s��
*	����:
*		�����(�~���b)( int ms )
*		�V�[�N�����t���O( UINT flag )
*			�������:SEEK_FWD
*			�߂�����:SEEK_REV
*	�ԋp�l�F	�Ȃ�
**********************************************************************************/
void seek_incMCI(int ms, UINT flag) {
	TCHAR szMCIReturnString[64] = TEXT(""),szCommand[128];
	int position;

	/*	���݈ʒu�̎擾	*/
	mciSendString(TEXT("status speedplay position"), szMCIReturnString, 64,NULL);
	/*	���݈ʒu��int�֕ϊ�	*/
	position = _ttoi(szMCIReturnString);

	/*	����/�߂����ʂ��ĕ���	*/
	switch(flag) {
		case SEEK_FWD:
			/*	position�������~���b���������Z	*/
			position += ms;
			break;
		case SEEK_REV:
			/*	position�������~���b���������Z	*/
			position -= ms;
			break;
	}

	/*	�R�}���h����/���M	*/
	_stprintf(szCommand, TEXT("play speedplay from %u"), position);
	mciSendString(szCommand, NULL, 0, NULL);
}
