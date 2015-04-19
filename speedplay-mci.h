#ifndef _SPEEDPLAY_MCI
#define _SPEEDPLAY_MCI

/*******************************************************************************************************
 *	�t�@�C�����F speedplay-mci.h
 *	�t�@�C���̓��e�FSpeedplay �� MCI�R�}���h�����֘A�̃w�b�_
 *	�쐬�ҁFfuwuyuan/�s����q
 *	�C���F
 *	���C�Z���X/Licensing:
 *	���̃t�@�C���́A����҂ł��� fuwuyuan/�s����q �ɂ����MIT License�̂��ƌ��J����Ă��܂��B
 *
 *	Copyright (c) 2015 fuwuyuan/�s����q
 *	This software is released under the MIT License, see LICENSE.txt.
 *	http://opensource.org/licenses/mit-license.php
 *******************************************************************************************************/

/*	�V�[�N�����w��t���O	*/
#define SEEK_FWD	1	//�������	(������)
#define SEEK_REV	0	//�߂�����	(�����߂�)

#define NORMAL_SPEED 1000	//�Đ����x:�W��

void change_speedMCI(UINT);		//�Đ����x��ύX����
LPTSTR getPositionReadable();	//���݈ʒu�� HumanReadable �Ŏ擾����
void initMCI(UINT);				//MCI�̏��������s��
void openMCI(LPCTSTR);			//MCI���g���ăt�@�C�����J��
void overlayDestination(HWND, UINT, UINT, UINT, UINT);
									//�r�f�I�I�[�o�[���C�\�����ݒ�
void seek_incMCI(int, UINT);		//���݈ʒu����̑��΃V�[�N���s��

#endif // _SPEEDPLAY_MCI
