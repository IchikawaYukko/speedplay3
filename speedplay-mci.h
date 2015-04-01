#ifndef _SPEEDPLAY_MCI
#define _SPEEDPLAY_MCI

/*******************************************************************************************************
 *	�t�@�C�����F speedplay-mci.h
 *	�t�@�C���̓��e�FSpeedplay �� MCI�R�}���h�����֘A�̃w�b�_
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
