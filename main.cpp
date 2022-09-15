#include "DxLib.h"

const char TITLE[] = "No23�FGRAVITY OPERATION";

const int WIN_WIDTH = 768; //�E�B���h�E����
const int WIN_HEIGHT = 576;//�E�B���h�E�c��


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(TRUE);						//�E�B���h�E���[�h�ɐݒ�
	//�E�B���h�E�T�C�Y���蓮�ł͕ύX�ł����A���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText(TITLE);					// �^�C�g����ύX
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);	//��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�i���j�^�[�̉𑜓x�ɍ��킹��j
	SetWindowSizeExtendRate(1.0);				//��ʃT�C�Y��ݒ�i�𑜓x�Ƃ̔䗦�Őݒ�j
	SetBackgroundColor(0x00, 0x00, 0x00);		// ��ʂ̔w�i�F��ݒ肷��

	//Dx���C�u�����̏�����
	if (DxLib_Init() == -1) { return -1; }

	//�i�_�u���o�b�t�@�j�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);



	//�Q�[�����[�v�Ŏg���ϐ��̐錾
	char keys[256] = { 0 }; //�ŐV�̃L�[�{�[�h���p
	char oldkeys[256] = { 0 };//1���[�v�i�t���[���j�O�̃L�[�{�[�h���

	//�摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
	//�^�C�g��
	int Title[15] = { 0 };
	LoadDivGraph("title.png", 14, 14, 1, 768, 576, Title);
	int TITLETIMER = 0;
	int titleX = 0;
	int titleY = 0;
	//�Z���N�g���
	int select[2] = { 0 };
	LoadDivGraph("stageselect.png", 2, 2, 1, 768, 576, select);
	int selectTIMER = 0;
	int selectX = 0;
	int selectY = 0;
	//�Q�[���I�[�o�[
	int gameover[2] = { 0 };
	LoadDivGraph("gameover.png", 2, 2, 1, 768, 576, gameover);
	int gameoverTIMER = 0;
	int gameoverX = 0;
	int gameoverY = 0;
	//���[��
	int rule;
	rule = LoadGraph("rule.png");
	int ruleX = 0;
	int ruleY = 0;
	//�v���C���[
	int player[7] = { 0 };
	LoadDivGraph("human01.png", 6, 6, 1, 64, 64, player);
	int player2[7] = { 0 };
	LoadDivGraph("human02.png", 6, 6, 1, 64, 64, player2);
	int playertimer = 0;
	int playerX = 100;
	int playerY = 360;
	int playerR = 20;
	int jumpflag = 0;
	int playeralive = 1;
	//���S
	int death[8] = { 0 };
	LoadDivGraph("death.png", 7, 7, 1, 64, 64, death);
	int deathX = 0;
	int deathY = 0;
	int deathR = 40;
	int deathtimer = 0;
	int deathflag = 0;
	//�n�ʃG�t�F�N�g
	int ground[6] = { 0 };
	LoadDivGraph("efect.png", 5, 5, 1, 64, 64, ground);
	int ground2[6] = { 0 };
	LoadDivGraph("efect0.png", 5, 5, 1, 64, 64, ground2);
	int groundX = 0;
	int groundY = 0;
	int groundR = 40;
	int groundtimer = 0;
	int groundflag = 0;

	//�v���C���[�̑O���W
	int oldx2 = 0;
	int oldy2 = 0;

	//�u���b�N
	int blockSize = 64;
	int blockSize2 = 32;
	int block2;
	block2 = LoadGraph("block01.png");
	//�w�i
	int haikei;
	haikei = LoadGraph("background1.png");
	//�S�[��
	int goal;
	goal = LoadGraph("md01.png");
	//�N���A���
	int clear;
	clear = LoadGraph("clear.png");
	int clearX = 0;
	int clearY = 0;
	int cleartimer;
	int clearBGM = LoadSoundMem("bgm_maoudamashii_cyber36.mp3");
	//OPBGM
	int op = LoadSoundMem("FreeBGM_Beelzebub.mp3");
	//PlayBGM
	int playBGM = LoadSoundMem("RUN-THROUGH.mp3");
	//�W�����vSE
	int playSE = LoadSoundMem("Cyber04-1.mp3");
	//���SSE
	int deathSE = LoadSoundMem("se_bomb3.mp3");
	int TopPositionFlag2 = 0;
	int TopPositionFlag = 0;
	//�w�i���W
	int haikeiX = 0;
	//�X�N���[���l
	int scrollX = 0;
	//�t���O�Ǘ�
	int spaceflag = 0;

	//�}�b�v�z�u(scene1)
	int map2[10][192] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,2,0},
	{1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
	};

	//�}�b�v�z�u(scene2)
	int map[10][192] = {
	{1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0},
	{0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,0,1,1,0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,2,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,0,1,0,1,0,0,0,0,1,0,0},
	{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	//�V�[���Ǘ�
	int scene = 0;

	// �Q�[�����[�v
	while (1)
	{
		//�ŐV�̃L�[�{�[�h��񂾂������̂͂P�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		for (int i = 0; i < 256; i++) {
			oldkeys[i] = keys[i];
		}
		//�ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		//��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		//�X�V����
		if (scene == 0)
		{
			//�^�C�g���̃^�C�}�[�𓮂���
			if (TITLETIMER <= 138)
			{
				TITLETIMER++;
			}

			//�X�y�[�X�L�[�̕\�����J��Ԃ�
			if (TITLETIMER == 139)
			{
				TITLETIMER = 100;
			}

			if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0)
			{
				scene = 1;
			}

			if (CheckSoundMem(op == 0))
			{
				ChangeVolumeSoundMem(170 * 50 / 100, op);
				PlaySoundMem(op, DX_PLAYTYPE_LOOP, TopPositionFlag = 0);
			}
		}

		else if (scene == 1)
		{
			if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0)
			{
				scene = 2;
			}
		}

		else if (scene == 2)
		{
			if (keys[KEY_INPUT_DOWN] == 1 && oldkeys[KEY_INPUT_DOWN] == 0)
			{
				selectTIMER = 1;
			}

			else if (keys[KEY_INPUT_UP] == 1 && oldkeys[KEY_INPUT_UP] == 0)
			{
				selectTIMER = 0;
			}

			if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0 && selectTIMER == 0)
			{
				scene = 3;
				PlaySoundMem(playBGM, DX_PLAYTYPE_LOOP, TopPositionFlag = 1);
			}

			if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0 && selectTIMER == 1)
			{
				scene = 4;
				PlaySoundMem(playBGM, DX_PLAYTYPE_LOOP, TopPositionFlag = 1);
			}
		}

		else if (scene == 3 || scene == 4)
		{
			if (playeralive == 1)
			{
				//�V�[��3��4�̋��ʕϐ�
				playertimer++;
				if (playertimer == 6)
				{
					playertimer = 0;
				}

				//�v���C���[�̑O�ʒu���ۑ�
				oldx2 = playerX;
				oldy2 = playerY;

				playerX += 4;//�v���C���[�������ő���
				scrollX += 4;//�����X�N���[��
				haikeiX += 4;//�����Ŕw�i������

				//�W�����v�t���O�؂�ւ�
				if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0 && jumpflag == 0 && spaceflag == 1) {

					jumpflag = 1;
					ChangeVolumeSoundMem(170 * 80 / 100, playSE);
					PlaySoundMem(playSE, DX_PLAYTYPE_BACK);
				}

				else if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0 && jumpflag == 1 && spaceflag == 1) {

					jumpflag = 0;
					ChangeVolumeSoundMem(170 * 80 / 100, playSE);
					PlaySoundMem(playSE, DX_PLAYTYPE_BACK);
				}

				//�v���CBGM��炷
				if (CheckSoundMem(playBGM == 0))
				{
					StopSoundMem(op);
					ChangeVolumeSoundMem(170 * 80 / 100, playBGM);
					PlaySoundMem(playBGM, DX_PLAYTYPE_LOOP, TopPositionFlag = 0);
				}

				//�V�[��3�̏ꍇ�̓����蔻��
				if (scene == 3)
				{
					//�G�t�F�N�g�t���O��0
					if (jumpflag == 0)
					{
						if (map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 0)
						{
							//�}�b�v�`�b�v���Ȃ��Ƃ��ɉ��ɗ���������
							playerY += 10;
						}

						if (map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							//�X�y�[�X��������悤�ɂȂ�
							spaceflag = 1;

							//�G�t�F�N�g������
							groundflag = 1;
						}

						else
						{
							spaceflag = 0;
						}
					}

					//�G�t�F�N�g�t���O��1
					else if (jumpflag == 1)
					{
						if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 0)
						{
							//�}�b�v�`�b�v���Ȃ��Ƃ��ɏ�ɏオ�葱����
							playerY -= 10;
						}

						if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							//�X�y�[�X��������悤�ɂȂ�
							spaceflag = 1;

							//�G�t�F�N�g������
							groundflag = 1;
						}

						else
						{
							spaceflag = 0;
						}

					}

					//�}�b�v�`�b�v�Ƃ̓����蔻��
					if (jumpflag == 0 || jumpflag == 1)
					{

						//�E
						if (map2[(playerY - playerR) / 64][(playerX + playerR) / 64] == 1 &&
							map2[(playerY + playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							//�����������ɑO�̈ʒu�ɖ߂�
							playerX = oldx2;
						}

						//��
						if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1 &&
							map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							//�����������ɑO�̈ʒu�ɖ߂�
							playerX = oldx2;
						}

						//��
						if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1 &&
							map2[(playerY - playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							//�����������ɑO�̈ʒu�ɖ߂�
							playerY = oldy2;
						}

						//��
						if (map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1 &&
							map2[(playerY + playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							//�����������ɑO�̈ʒu�ɖ߂�
							playerY = oldy2;
						}


						//����
						if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							if (map2[(playerY - playerR) / 64][(oldx2 - playerR) / 64] == 0)
							{
								if (playerX < oldx2)
								{
									//�����������ɑO�̈ʒu�ɖ߂�
									playerX = oldx2;
								}
							}

							else if (map2[(oldy2 - playerR) / 64][(playerX - playerR) / 64] == 0)
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerY = oldy2;
							}

							else
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerX = oldx2;
								playerY = oldy2;
							}
						}

						//����
						if (map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							if (map2[(playerY + playerR) / 64][(oldx2 - playerR) / 64] == 0)
							{
								if (playerX < oldx2)
								{
									//�����������ɑO�̈ʒu�ɖ߂�
									playerX = oldx2;
								}
							}

							else if (map2[(oldy2 + playerR) / 64][(playerX - playerR) / 64] == 0)
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerY = oldy2;
							}

							else
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerX = oldx2;
								playerY = oldy2;
							}
						}

						//�E��
						if (map2[(playerY - playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							if (map2[(playerY - playerR) / 64][(oldx2 + playerR) / 64] == 0)
							{
								if (playerX > oldx2)
								{
									//�����������ɑO�̈ʒu�ɖ߂�
									playerX = oldx2;
								}
							}

							else if (map2[(oldy2 - playerR) / 64][(playerX + playerR) / 64] == 0)
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerY = oldy2;
							}

							else
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerX = oldx2;
								playerY = oldy2;
							}
						}

						//�E��
						if (map2[(playerY + playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							if (map2[(playerY + playerR) / 64][(oldx2 + playerR) / 64] == 0)
							{
								if (playerX > oldx2)
								{
									//�����������ɑO�̈ʒu�ɖ߂�
									playerX = oldx2;
								}
							}

							else if (map2[(oldy2 + playerR) / 64][(playerX + playerR) / 64] == 0)
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerY = oldy2;
							}

							else
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerX = oldx2;
								playerY = oldy2;
							}
						}
					}

					//�Q�[���N���A
					//�E
					if (map2[(playerY - playerR) / 64][(playerX + playerR) / 64] == 2 &&
						map2[(playerY + playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						scene = 5;
					}

					//��
					if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 2 &&
						map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 2)
					{
						scene = 5;
					}

					//��
					if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 2 &&
						map2[(playerY - playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						scene = 5;
					}

					//��
					if (map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 2 &&
						map2[(playerY + playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						scene = 5;
					}

					//����
					if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 2)
					{
						if (map2[(playerY - playerR) / 64][(oldx2 - playerR) / 64] == 0)
						{
							if (playerX < oldx2)
							{
								scene = 5;
							}
						}

						else if (map2[(oldy2 - playerR) / 64][(playerX - playerR) / 64] == 0)
						{
							scene = 5;
						}

						else
						{
							scene = 5;
						}
					}

					//����
					if (map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 2)
					{
						if (map2[(playerY + playerR) / 64][(oldx2 - playerR) / 64] == 0)
						{
							if (playerX < oldx2)
							{
								scene = 5;
							}
						}

						else if (map2[(oldy2 + playerR) / 64][(playerX - playerR) / 64] == 0)
						{
							scene = 5;
						}

						else
						{
							scene = 5;
						}
					}

					//�E��
					if (map2[(playerY - playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						if (map2[(playerY - playerR) / 64][(oldx2 + playerR) / 64] == 0)
						{
							if (playerX > oldx2)
							{
								scene = 5;
							}
						}

						else if (map2[(oldy2 - playerR) / 64][(playerX + playerR) / 64] == 0)
						{
							scene = 5;
						}

						else
						{
							scene = 5;
						}
					}

					//�E��
					if (map2[(playerY + playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						if (map2[(playerY + playerR) / 64][(oldx2 + playerR) / 64] == 0)
						{
							if (playerX > oldx2)
							{
								scene = 5;
							}
						}

						else if (map2[(oldy2 + playerR) / 64][(playerX + playerR) / 64] == 0)
						{
							scene = 5;
						}

						else
						{
							scene = 5;
						}
					}
				}
			}

			//�V�[��4
			if (scene == 4)
			{
				if (playeralive == 1)
				{
					if (jumpflag == 0)
					{
						if (map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 0)
						{
							//�}�b�v�`�b�v���Ȃ��Ƃ��ɉ��ɗ���������
							playerY += 10;
						}

						if (map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							//�X�y�[�X��������悤�ɂȂ�
							spaceflag = 1;

							//�G�t�F�N�g������
							groundflag = 1;
						}

						else
						{
							spaceflag = 0;
						}
					}


					else if (jumpflag == 1)
					{
						if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 0)
						{
							//�}�b�v�`�b�v���Ȃ��Ƃ��ɏ�ɏオ�葱����
							playerY -= 10;
						}

						if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							//�X�y�[�X��������悤�ɂȂ�
							spaceflag = 1;

							//�G�t�F�N�g������
							groundflag = 1;
						}

						else
						{
							spaceflag = 0;
						}
					}

					//�}�b�v�`�b�v�Ƃ̓����蔻��
					if (jumpflag == 0 || jumpflag == 1)
					{
						//�E
						if (map[(playerY - playerR) / 64][(playerX + playerR) / 64] == 1 &&
							map[(playerY + playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							//�����������ɑO�̈ʒu�ɖ߂�
							playerX = oldx2;
						}

						//��
						if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1 &&
							map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							//�����������ɑO�̈ʒu�ɖ߂�
							playerX = oldx2;
						}

						//��
						if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1 &&
							map[(playerY - playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							//�����������ɑO�̈ʒu�ɖ߂�
							playerY = oldy2;
						}

						//��
						if (map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1 &&
							map[(playerY + playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							//�����������ɑO�̈ʒu�ɖ߂�
							playerY = oldy2;
						}


						//����
						if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							if (map[(playerY - playerR) / 64][(oldx2 - playerR) / 64] == 0)
							{
								if (playerX < oldx2)
								{
									//�����������ɑO�̈ʒu�ɖ߂�
									playerX = oldx2;
								}
							}

							else if (map[(oldy2 - playerR) / 64][(playerX - playerR) / 64] == 0)
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerY = oldy2;
							}

							else
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerX = oldx2;
								playerY = oldy2;
							}
						}

						//����
						if (map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							if (map[(playerY + playerR) / 64][(oldx2 - playerR) / 64] == 0)
							{
								if (playerX < oldx2)
								{
									//�����������ɑO�̈ʒu�ɖ߂�
									playerX = oldx2;
								}
							}

							else if (map[(oldy2 + playerR) / 64][(playerX - playerR) / 64] == 0)
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerY = oldy2;
							}

							else
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerX = oldx2;
								playerY = oldy2;
							}
						}

						//�E��
						if (map[(playerY - playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							if (map[(playerY - playerR) / 64][(oldx2 + playerR) / 64] == 0)
							{
								if (playerX > oldx2)
								{
									//�����������ɑO�̈ʒu�ɖ߂�
									playerX = oldx2;
								}
							}

							else if (map[(oldy2 - playerR) / 64][(playerX + playerR) / 64] == 0)
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerY = oldy2;
							}

							else
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerX = oldx2;
								playerY = oldy2;
							}
						}

						//�E��
						if (map[(playerY + playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							if (map[(playerY + playerR) / 64][(oldx2 + playerR) / 64] == 0)
							{
								if (playerX > oldx2)
								{
									//�����������ɑO�̈ʒu�ɖ߂�
									playerX = oldx2;
								}
							}

							else if (map[(oldy2 + playerR) / 64][(playerX + playerR) / 64] == 0)
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerY = oldy2;
							}

							else
							{
								//�����������ɑO�̈ʒu�ɖ߂�
								playerX = oldx2;
								playerY = oldy2;
							}
						}
					}

					//�N���A�̂��߂̃}�b�v�`�b�v�̓����蔻��
					//�E
					if (map[(playerY - playerR) / 64][(playerX + playerR) / 64] == 2 &&
						map[(playerY + playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						scene = 5;
					}

					//��
					if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 2 &&
						map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 2)
					{
						scene = 5;
					}

					//��
					if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 2 &&
						map[(playerY - playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						scene = 5;
					}

					//��
					if (map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 2 &&
						map[(playerY + playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						scene = 5;
					}


					//����
					if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 2)
					{
						if (map[(playerY - playerR) / 64][(oldx2 - playerR) / 64] == 0)
						{
							if (playerX < oldx2)
							{
								scene = 5;
							}
						}

						else if (map[(oldy2 - playerR) / 64][(playerX - playerR) / 64] == 0)
						{
							scene = 5;
						}

						else
						{
							scene = 5;
						}
					}

					//����
					if (map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 2)
					{
						if (map[(playerY + playerR) / 64][(oldx2 - playerR) / 64] == 0)
						{
							if (playerX < oldx2)
							{
								scene = 5;
							}
						}

						else if (map[(oldy2 + playerR) / 64][(playerX - playerR) / 64] == 0)
						{
							scene = 5;
						}

						else
						{
							scene = 5;
						}
					}

					//�E��
					if (map[(playerY - playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						if (map[(playerY - playerR) / 64][(oldx2 + playerR) / 64] == 0)
						{
							if (playerX > oldx2)
							{
								scene = 5;
							}
						}

						else if (map[(oldy2 - playerR) / 64][(playerX + playerR) / 64] == 0)
						{
							scene = 5;
						}

						else
						{
							scene = 5;
						}
					}

					//�E��
					if (map[(playerY + playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						if (map[(playerY + playerR) / 64][(oldx2 + playerR) / 64] == 0)
						{
							if (playerX > oldx2)
							{
								scene = 5;
							}
						}

						else if (map[(oldy2 + playerR) / 64][(playerX + playerR) / 64] == 0)
						{
							scene = 5;
						}

						else
						{
							scene = 5;
						}
					}
				}
			}

			//�n�ʃG�t�F�N�g
			if (groundflag == 1)
			{
				//�G�t�F�N�g�̈ʒu
				groundX = playerX;
				groundY = playerY;

				//�G�t�F�N�g�̃^�C�}�[����
				if (0 <= groundtimer && groundtimer <= 4)
				{
					groundtimer++;
				}

				//�G�t�F�N�g�t���O��0�ɖ߂�
				if (groundtimer >= 4)
				{
					groundflag = 0;
				}
			}

			//�G�t�F�N�g�t���O��0�ɂ���
			else if (groundflag == 0)
			{
				groundtimer = 0;
			}

			//�v���C���[�̎��S����
			if (playerY <= 30)
			{
				playeralive = 0;
			}

			if (playerY >= 560)
			{
				playeralive = 0;
			}

			if (playerX <= scrollX)
			{
				playeralive = 0;
			}

			//�v���C���[�̎��S����
			else if (playeralive == 0)
			{
				if (TopPositionFlag2 == 0)
				{
					StopSoundMem(playBGM);
					ChangeVolumeSoundMem(170 * 80 / 100, deathSE);
					PlaySoundMem(deathSE, DX_PLAYTYPE_BACK);
					TopPositionFlag2 = 1;
				}
				deathX = playerX;
				deathY = playerY;
				deathtimer++;
			}

			//�V�[��6�ֈڍs
			if (deathtimer >= 80)
			{
				scene = 6;
			}
		}

		//�V�[��5
		else if (scene == 5)
		{
			//�v���CBGM������
			StopSoundMem(playBGM);

			//�N���A�̋Ȃ𗬂�
			ChangeVolumeSoundMem(170 * 80 / 100, clearBGM);
			PlaySoundMem(clearBGM, DX_PLAYTYPE_LOOP, TopPositionFlag = 0);
			if (clearY >= -2420)
			{
				//�����X�N���[��
				clearY = clearY - 1;
			}
		}

		//�V�[��6
		else if (scene == 6)
		{
			if (keys[KEY_INPUT_RIGHT] == 1 && oldkeys[KEY_INPUT_RIGHT] == 0)
			{
				gameoverTIMER = 1;
			}

			else if (keys[KEY_INPUT_LEFT] == 1 && oldkeys[KEY_INPUT_LEFT] == 0)
			{
				gameoverTIMER = 0;
			}

			//���X�^�[�g���̏�����
			if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0 && gameoverTIMER == 0)
			{
				scene = 0;
				TITLETIMER = 0;
				playertimer = 0;
				playerX = 100;
				playerY = 360;
				playerR = 20;
				jumpflag = 0;
				playeralive = 1;
				haikeiX = 0;
				scrollX = 0;
				spaceflag = 0;
				deathtimer = 0;
				PlaySoundMem(op, DX_PLAYTYPE_LOOP, TopPositionFlag = 1);
				StopSoundMem(playBGM);
				TopPositionFlag = 0;
				TopPositionFlag2 = 0;
			}

			//���X�^�[�g���Ȃ������ꍇ
			if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0 && gameoverTIMER == 1)
			{
				break;
			}
		}
		//�`�揈��
		if (scene == 0)
		{
			//�^�C�g�������Ԃ��Ƃɕ`��
			if (TITLETIMER >= 0 && TITLETIMER < 10)
			{
				DrawGraph(titleX, titleY, Title[0], TRUE);
			}

			else if (TITLETIMER >= 10 && TITLETIMER < 20)
			{
				DrawGraph(titleX, titleY, Title[1], TRUE);
			}

			else if (TITLETIMER >= 20 && TITLETIMER < 30)
			{
				DrawGraph(titleX, titleY, Title[2], TRUE);
			}

			else if (TITLETIMER >= 30 && TITLETIMER < 40)
			{
				DrawGraph(titleX, titleY, Title[3], TRUE);
			}

			else if (TITLETIMER >= 40 && TITLETIMER < 50)
			{
				DrawGraph(titleX, titleY, Title[4], TRUE);
			}

			else if (TITLETIMER >= 50 && TITLETIMER < 60)
			{
				DrawGraph(titleX, titleY, Title[5], TRUE);
			}

			else if (TITLETIMER >= 60 && TITLETIMER < 70)
			{
				DrawGraph(titleX, titleY, Title[6], TRUE);
			}

			else if (TITLETIMER >= 70 && TITLETIMER < 80)
			{
				DrawGraph(titleX, titleY, Title[7], TRUE);
			}

			else if (TITLETIMER >= 80 && TITLETIMER < 90)
			{
				DrawGraph(titleX, titleY, Title[8], TRUE);
			}

			else if (TITLETIMER >= 90 && TITLETIMER < 100)
			{
				DrawGraph(titleX, titleY, Title[9], TRUE);
			}

			else if (TITLETIMER >= 100 && TITLETIMER < 110)
			{
				DrawGraph(titleX, titleY, Title[10], TRUE);
			}

			else if (TITLETIMER >= 110 && TITLETIMER < 120)
			{
				DrawGraph(titleX, titleY, Title[11], TRUE);
			}

			else if (TITLETIMER >= 120 && TITLETIMER < 130)
			{
				DrawGraph(titleX, titleY, Title[12], TRUE);
			}

			else if (TITLETIMER >= 130 && TITLETIMER < 140)
			{
				DrawGraph(titleX, titleY, Title[13], TRUE);
			}
		}

		//�V�[��1(���[�����)
		else if (scene == 1)
		{
			DrawGraph(ruleX, ruleY, rule, TRUE);
		}

		//�V�[��2(�Z���N�g���)
		else if (scene == 2)
		{
			if (selectTIMER == 0)
			{
				DrawGraph(selectX, selectY, select[0], TRUE);
			}

			else if (selectTIMER == 1)
			{
				DrawGraph(selectX, selectY, select[1], TRUE);
			}
		}

		if (scene == 3 || scene == 4)
		{
			//�V�[��3�ƃV�[��4�̋��ʂ̂��̂̕`��
			DrawExtendGraph(0 - haikeiX, 0, 768 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(768 - haikeiX, 0, 1536 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(1536 - haikeiX, 0, 2304 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(2304 - haikeiX, 0, 3072 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(3072 - haikeiX, 0, 3840 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(3840 - haikeiX, 0, 4608 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(4608 - haikeiX, 0, 5376 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(5376 - haikeiX, 0, 6080 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(6080 - haikeiX, 0, 6848 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(6848 - haikeiX, 0, 7616 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(7616 - haikeiX, 0, 8384 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(8384 - haikeiX, 0, 9152 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(9152 - haikeiX, 0, 9920 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(9920 - haikeiX, 0, 10688 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(10688 - haikeiX, 0, 11456 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(11456 - haikeiX, 0, 12224 - haikeiX, 576, haikei, TRUE);
			DrawExtendGraph(12224 - haikeiX, 0, 12992 - haikeiX, 576, haikei, TRUE);

			if (playeralive == 1)
			{
				//�W�����v�t���O���Ƃ̃v���C���[�̌���(���ɂ���ꍇ)
				if (jumpflag == 0)
				{
					DrawExtendGraph(playerX - playerR - scrollX, playerY - playerR, playerX + playerR - scrollX, playerY + playerR, player[playertimer], TRUE);

					//�n�ʂ̃G�t�F�N�g
					if (groundflag == 1)
					{
						DrawExtendGraph(groundX - groundR - scrollX, groundY - groundR, groundX + groundR - scrollX, groundY + groundR, ground[groundtimer], TRUE);
					}
				}

				//�W�����v�t���O���Ƃ̃v���C���[�̌���(��ɂ���ꍇ)
				if (jumpflag == 1)
				{
					DrawExtendGraph(playerX - playerR - scrollX, playerY - playerR, playerX + playerR - scrollX, playerY + playerR, player2[playertimer], TRUE);

					//�n�ʂ̃G�t�F�N�g
					if (groundflag == 1)
					{
						DrawExtendGraph(groundX - groundR - scrollX, groundY - groundR, groundX + groundR - scrollX, groundY + groundR, ground2[groundtimer], TRUE);
					}
				}
			}

			if (playeralive == 0)
			{
				if (deathtimer <= 5)
				{
					DrawExtendGraph(deathX - deathR - scrollX, deathY - deathR, deathX + deathR - scrollX, deathY + deathR, death[1], TRUE);
				}

				else if (5 < deathtimer && deathtimer <= 10)
				{
					DrawExtendGraph(deathX - deathR - scrollX, deathY - deathR, deathX + deathR - scrollX, deathY + deathR, death[2], TRUE);
				}

				else if (10 < deathtimer && deathtimer <= 15)
				{
					DrawExtendGraph(deathX - deathR - scrollX, deathY - deathR, deathX + deathR - scrollX, deathY + deathR, death[3], TRUE);
				}

				else if (15 < deathtimer && deathtimer <= 20)
				{
					DrawExtendGraph(deathX - deathR - scrollX, deathY - deathR, deathX + deathR - scrollX, deathY + deathR, death[4], TRUE);
				}

				else if (20 < deathtimer && deathtimer <= 25)
				{
					DrawExtendGraph(deathX - deathR - scrollX, deathY - deathR, deathX + deathR - scrollX, deathY + deathR, death[5], TRUE);
				}

				else if (25 < deathtimer && deathtimer <= 30)
				{
					DrawExtendGraph(deathX - deathR - scrollX, deathY - deathR, deathX + deathR - scrollX, deathY + deathR, death[6], TRUE);
				}

				else if (30 < deathtimer && deathtimer <= 35)
				{
					DrawExtendGraph(deathX - deathR - scrollX, deathY - deathR, deathX + deathR - scrollX, deathY + deathR, death[7], TRUE);
				}
			}

			//�V�[��3�̃u���b�N�z�u
			if (scene == 3)
			{
				for (int y = 0; y < 9; y++)
				{
					for (int x = 0; x < 192; x++)
					{
						//�ʏ�u���b�N
						if (map2[y][x] == 1)
						{
							DrawGraph(x * blockSize - scrollX, y * blockSize, block2, TRUE);
						}

						//�S�[���u���b�N
						if (map2[y][x] == 2)
						{
							DrawGraph(x * blockSize + 16 - scrollX, y * blockSize + 32, goal, TRUE);
						}
					}
				}
			}

			//�V�[��4�̃u���b�N�z�u
			else if (scene == 4)
			{
				for (int y = 0; y < 9; y++)
				{
					for (int x = 0; x < 192; x++)
					{
						//�ʏ�u���b�N
						if (map[y][x] == 1)
						{
							DrawGraph(x * blockSize - scrollX, y * blockSize, block2, TRUE);
						}

						//�S�[���u���b�N
						else if (map[y][x] == 2)
						{
							DrawGraph(x * blockSize + 16 - scrollX, y * blockSize + 32, goal, TRUE);
						}
					}
				}
			}
		}

		//�V�[��5(�N���A���)
		else if (scene == 5)
		{
			DrawGraph(clearX, clearY, clear, TRUE);
		}

		//�V�[��6(�Q�[���I�[�o�[)
		else if (scene == 6)
		{
			if (gameoverTIMER == 0)
			{
				DrawGraph(gameoverX, gameoverY, gameover[0], TRUE);
			}

			else if (gameoverTIMER == 1)
			{
				DrawGraph(gameoverX, gameoverY, gameover[1], TRUE);
			}
		}

		//---------  �����܂łɃv���O�������L�q  ---------//
		ScreenFlip();//�i�_�u���o�b�t�@�j����
		// 20�~���b�ҋ@�i�^��60FPS�j
		WaitTimer(20);
		// Windows �V�X�e�����炭�������������
		if (ProcessMessage() == -1)
		{
			break;
		}
		// �d�r�b�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	//Dx���C�u�����I������
	DxLib_End();

	return 0;
}
