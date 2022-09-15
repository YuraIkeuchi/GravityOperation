#include "DxLib.h"

const char TITLE[] = "No23：GRAVITY OPERATION";

const int WIN_WIDTH = 768; //ウィンドウ横幅
const int WIN_HEIGHT = 576;//ウィンドウ縦幅


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(TRUE);						//ウィンドウモードに設定
	//ウィンドウサイズを手動では変更できず、かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText(TITLE);					// タイトルを変更
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);	//画面サイズの最大サイズ、カラービット数を設定（モニターの解像度に合わせる）
	SetWindowSizeExtendRate(1.0);				//画面サイズを設定（解像度との比率で設定）
	SetBackgroundColor(0x00, 0x00, 0x00);		// 画面の背景色を設定する

	//Dxライブラリの初期化
	if (DxLib_Init() == -1) { return -1; }

	//（ダブルバッファ）描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);



	//ゲームループで使う変数の宣言
	char keys[256] = { 0 }; //最新のキーボード情報用
	char oldkeys[256] = { 0 };//1ループ（フレーム）前のキーボード情報

	//画像などのリソースデータの変数宣言と読み込み
	//タイトル
	int Title[15] = { 0 };
	LoadDivGraph("title.png", 14, 14, 1, 768, 576, Title);
	int TITLETIMER = 0;
	int titleX = 0;
	int titleY = 0;
	//セレクト画面
	int select[2] = { 0 };
	LoadDivGraph("stageselect.png", 2, 2, 1, 768, 576, select);
	int selectTIMER = 0;
	int selectX = 0;
	int selectY = 0;
	//ゲームオーバー
	int gameover[2] = { 0 };
	LoadDivGraph("gameover.png", 2, 2, 1, 768, 576, gameover);
	int gameoverTIMER = 0;
	int gameoverX = 0;
	int gameoverY = 0;
	//ルール
	int rule;
	rule = LoadGraph("rule.png");
	int ruleX = 0;
	int ruleY = 0;
	//プレイヤー
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
	//死亡
	int death[8] = { 0 };
	LoadDivGraph("death.png", 7, 7, 1, 64, 64, death);
	int deathX = 0;
	int deathY = 0;
	int deathR = 40;
	int deathtimer = 0;
	int deathflag = 0;
	//地面エフェクト
	int ground[6] = { 0 };
	LoadDivGraph("efect.png", 5, 5, 1, 64, 64, ground);
	int ground2[6] = { 0 };
	LoadDivGraph("efect0.png", 5, 5, 1, 64, 64, ground2);
	int groundX = 0;
	int groundY = 0;
	int groundR = 40;
	int groundtimer = 0;
	int groundflag = 0;

	//プレイヤーの前座標
	int oldx2 = 0;
	int oldy2 = 0;

	//ブロック
	int blockSize = 64;
	int blockSize2 = 32;
	int block2;
	block2 = LoadGraph("block01.png");
	//背景
	int haikei;
	haikei = LoadGraph("background1.png");
	//ゴール
	int goal;
	goal = LoadGraph("md01.png");
	//クリア画面
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
	//ジャンプSE
	int playSE = LoadSoundMem("Cyber04-1.mp3");
	//死亡SE
	int deathSE = LoadSoundMem("se_bomb3.mp3");
	int TopPositionFlag2 = 0;
	int TopPositionFlag = 0;
	//背景座標
	int haikeiX = 0;
	//スクロール値
	int scrollX = 0;
	//フラグ管理
	int spaceflag = 0;

	//マップ配置(scene1)
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

	//マップ配置(scene2)
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

	//シーン管理
	int scene = 0;

	// ゲームループ
	while (1)
	{
		//最新のキーボード情報だったものは１フレーム前のキーボード情報として保存
		for (int i = 0; i < 256; i++) {
			oldkeys[i] = keys[i];
		}
		//最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		//画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		//更新処理
		if (scene == 0)
		{
			//タイトルのタイマーを動かす
			if (TITLETIMER <= 138)
			{
				TITLETIMER++;
			}

			//スペースキーの表示を繰り返す
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
				//シーン3と4の共通変数
				playertimer++;
				if (playertimer == 6)
				{
					playertimer = 0;
				}

				//プレイヤーの前位置情報保存
				oldx2 = playerX;
				oldy2 = playerY;

				playerX += 4;//プレイヤーが自分で走る
				scrollX += 4;//自動スクロール
				haikeiX += 4;//自動で背景が動く

				//ジャンプフラグ切り替え
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

				//プレイBGMを鳴らす
				if (CheckSoundMem(playBGM == 0))
				{
					StopSoundMem(op);
					ChangeVolumeSoundMem(170 * 80 / 100, playBGM);
					PlaySoundMem(playBGM, DX_PLAYTYPE_LOOP, TopPositionFlag = 0);
				}

				//シーン3の場合の当たり判定
				if (scene == 3)
				{
					//エフェクトフラグが0
					if (jumpflag == 0)
					{
						if (map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 0)
						{
							//マップチップがないときに下に落ち続ける
							playerY += 10;
						}

						if (map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							//スペースが押せるようになる
							spaceflag = 1;

							//エフェクトが発生
							groundflag = 1;
						}

						else
						{
							spaceflag = 0;
						}
					}

					//エフェクトフラグが1
					else if (jumpflag == 1)
					{
						if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 0)
						{
							//マップチップがないときに上に上がり続ける
							playerY -= 10;
						}

						if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							//スペースが押せるようになる
							spaceflag = 1;

							//エフェクトが発生
							groundflag = 1;
						}

						else
						{
							spaceflag = 0;
						}

					}

					//マップチップとの当たり判定
					if (jumpflag == 0 || jumpflag == 1)
					{

						//右
						if (map2[(playerY - playerR) / 64][(playerX + playerR) / 64] == 1 &&
							map2[(playerY + playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							//当たった時に前の位置に戻す
							playerX = oldx2;
						}

						//左
						if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1 &&
							map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							//当たった時に前の位置に戻す
							playerX = oldx2;
						}

						//上
						if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1 &&
							map2[(playerY - playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							//当たった時に前の位置に戻す
							playerY = oldy2;
						}

						//下
						if (map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1 &&
							map2[(playerY + playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							//当たった時に前の位置に戻す
							playerY = oldy2;
						}


						//左上
						if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							if (map2[(playerY - playerR) / 64][(oldx2 - playerR) / 64] == 0)
							{
								if (playerX < oldx2)
								{
									//当たった時に前の位置に戻す
									playerX = oldx2;
								}
							}

							else if (map2[(oldy2 - playerR) / 64][(playerX - playerR) / 64] == 0)
							{
								//当たった時に前の位置に戻す
								playerY = oldy2;
							}

							else
							{
								//当たった時に前の位置に戻す
								playerX = oldx2;
								playerY = oldy2;
							}
						}

						//左下
						if (map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							if (map2[(playerY + playerR) / 64][(oldx2 - playerR) / 64] == 0)
							{
								if (playerX < oldx2)
								{
									//当たった時に前の位置に戻す
									playerX = oldx2;
								}
							}

							else if (map2[(oldy2 + playerR) / 64][(playerX - playerR) / 64] == 0)
							{
								//当たった時に前の位置に戻す
								playerY = oldy2;
							}

							else
							{
								//当たった時に前の位置に戻す
								playerX = oldx2;
								playerY = oldy2;
							}
						}

						//右上
						if (map2[(playerY - playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							if (map2[(playerY - playerR) / 64][(oldx2 + playerR) / 64] == 0)
							{
								if (playerX > oldx2)
								{
									//当たった時に前の位置に戻す
									playerX = oldx2;
								}
							}

							else if (map2[(oldy2 - playerR) / 64][(playerX + playerR) / 64] == 0)
							{
								//当たった時に前の位置に戻す
								playerY = oldy2;
							}

							else
							{
								//当たった時に前の位置に戻す
								playerX = oldx2;
								playerY = oldy2;
							}
						}

						//右下
						if (map2[(playerY + playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							if (map2[(playerY + playerR) / 64][(oldx2 + playerR) / 64] == 0)
							{
								if (playerX > oldx2)
								{
									//当たった時に前の位置に戻す
									playerX = oldx2;
								}
							}

							else if (map2[(oldy2 + playerR) / 64][(playerX + playerR) / 64] == 0)
							{
								//当たった時に前の位置に戻す
								playerY = oldy2;
							}

							else
							{
								//当たった時に前の位置に戻す
								playerX = oldx2;
								playerY = oldy2;
							}
						}
					}

					//ゲームクリア
					//右
					if (map2[(playerY - playerR) / 64][(playerX + playerR) / 64] == 2 &&
						map2[(playerY + playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						scene = 5;
					}

					//左
					if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 2 &&
						map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 2)
					{
						scene = 5;
					}

					//上
					if (map2[(playerY - playerR) / 64][(playerX - playerR) / 64] == 2 &&
						map2[(playerY - playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						scene = 5;
					}

					//下
					if (map2[(playerY + playerR) / 64][(playerX - playerR) / 64] == 2 &&
						map2[(playerY + playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						scene = 5;
					}

					//左上
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

					//左下
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

					//右上
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

					//右下
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

			//シーン4
			if (scene == 4)
			{
				if (playeralive == 1)
				{
					if (jumpflag == 0)
					{
						if (map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 0)
						{
							//マップチップがないときに下に落ち続ける
							playerY += 10;
						}

						if (map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							//スペースが押せるようになる
							spaceflag = 1;

							//エフェクトが発生
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
							//マップチップがないときに上に上がり続ける
							playerY -= 10;
						}

						if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							//スペースが押せるようになる
							spaceflag = 1;

							//エフェクトが発生
							groundflag = 1;
						}

						else
						{
							spaceflag = 0;
						}
					}

					//マップチップとの当たり判定
					if (jumpflag == 0 || jumpflag == 1)
					{
						//右
						if (map[(playerY - playerR) / 64][(playerX + playerR) / 64] == 1 &&
							map[(playerY + playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							//当たった時に前の位置に戻す
							playerX = oldx2;
						}

						//左
						if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1 &&
							map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							//当たった時に前の位置に戻す
							playerX = oldx2;
						}

						//上
						if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1 &&
							map[(playerY - playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							//当たった時に前の位置に戻す
							playerY = oldy2;
						}

						//下
						if (map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1 &&
							map[(playerY + playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							//当たった時に前の位置に戻す
							playerY = oldy2;
						}


						//左上
						if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							if (map[(playerY - playerR) / 64][(oldx2 - playerR) / 64] == 0)
							{
								if (playerX < oldx2)
								{
									//当たった時に前の位置に戻す
									playerX = oldx2;
								}
							}

							else if (map[(oldy2 - playerR) / 64][(playerX - playerR) / 64] == 0)
							{
								//当たった時に前の位置に戻す
								playerY = oldy2;
							}

							else
							{
								//当たった時に前の位置に戻す
								playerX = oldx2;
								playerY = oldy2;
							}
						}

						//左下
						if (map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 1)
						{
							if (map[(playerY + playerR) / 64][(oldx2 - playerR) / 64] == 0)
							{
								if (playerX < oldx2)
								{
									//当たった時に前の位置に戻す
									playerX = oldx2;
								}
							}

							else if (map[(oldy2 + playerR) / 64][(playerX - playerR) / 64] == 0)
							{
								//当たった時に前の位置に戻す
								playerY = oldy2;
							}

							else
							{
								//当たった時に前の位置に戻す
								playerX = oldx2;
								playerY = oldy2;
							}
						}

						//右上
						if (map[(playerY - playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							if (map[(playerY - playerR) / 64][(oldx2 + playerR) / 64] == 0)
							{
								if (playerX > oldx2)
								{
									//当たった時に前の位置に戻す
									playerX = oldx2;
								}
							}

							else if (map[(oldy2 - playerR) / 64][(playerX + playerR) / 64] == 0)
							{
								//当たった時に前の位置に戻す
								playerY = oldy2;
							}

							else
							{
								//当たった時に前の位置に戻す
								playerX = oldx2;
								playerY = oldy2;
							}
						}

						//右下
						if (map[(playerY + playerR) / 64][(playerX + playerR) / 64] == 1)
						{
							if (map[(playerY + playerR) / 64][(oldx2 + playerR) / 64] == 0)
							{
								if (playerX > oldx2)
								{
									//当たった時に前の位置に戻す
									playerX = oldx2;
								}
							}

							else if (map[(oldy2 + playerR) / 64][(playerX + playerR) / 64] == 0)
							{
								//当たった時に前の位置に戻す
								playerY = oldy2;
							}

							else
							{
								//当たった時に前の位置に戻す
								playerX = oldx2;
								playerY = oldy2;
							}
						}
					}

					//クリアのためのマップチップの当たり判定
					//右
					if (map[(playerY - playerR) / 64][(playerX + playerR) / 64] == 2 &&
						map[(playerY + playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						scene = 5;
					}

					//左
					if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 2 &&
						map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 2)
					{
						scene = 5;
					}

					//上
					if (map[(playerY - playerR) / 64][(playerX - playerR) / 64] == 2 &&
						map[(playerY - playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						scene = 5;
					}

					//下
					if (map[(playerY + playerR) / 64][(playerX - playerR) / 64] == 2 &&
						map[(playerY + playerR) / 64][(playerX + playerR) / 64] == 2)
					{
						scene = 5;
					}


					//左上
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

					//左下
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

					//右上
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

					//右下
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

			//地面エフェクト
			if (groundflag == 1)
			{
				//エフェクトの位置
				groundX = playerX;
				groundY = playerY;

				//エフェクトのタイマー処理
				if (0 <= groundtimer && groundtimer <= 4)
				{
					groundtimer++;
				}

				//エフェクトフラグを0に戻す
				if (groundtimer >= 4)
				{
					groundflag = 0;
				}
			}

			//エフェクトフラグを0にする
			else if (groundflag == 0)
			{
				groundtimer = 0;
			}

			//プレイヤーの死亡条件
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

			//プレイヤーの死亡処理
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

			//シーン6へ移行
			if (deathtimer >= 80)
			{
				scene = 6;
			}
		}

		//シーン5
		else if (scene == 5)
		{
			//プレイBGMを消す
			StopSoundMem(playBGM);

			//クリアの曲を流す
			ChangeVolumeSoundMem(170 * 80 / 100, clearBGM);
			PlaySoundMem(clearBGM, DX_PLAYTYPE_LOOP, TopPositionFlag = 0);
			if (clearY >= -2420)
			{
				//自動スクロール
				clearY = clearY - 1;
			}
		}

		//シーン6
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

			//リスタート時の初期化
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

			//リスタートしなかった場合
			if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0 && gameoverTIMER == 1)
			{
				break;
			}
		}
		//描画処理
		if (scene == 0)
		{
			//タイトルを時間ごとに描画
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

		//シーン1(ルール画面)
		else if (scene == 1)
		{
			DrawGraph(ruleX, ruleY, rule, TRUE);
		}

		//シーン2(セレクト画面)
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
			//シーン3とシーン4の共通のものの描画
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
				//ジャンプフラグごとのプレイヤーの向き(下にいる場合)
				if (jumpflag == 0)
				{
					DrawExtendGraph(playerX - playerR - scrollX, playerY - playerR, playerX + playerR - scrollX, playerY + playerR, player[playertimer], TRUE);

					//地面のエフェクト
					if (groundflag == 1)
					{
						DrawExtendGraph(groundX - groundR - scrollX, groundY - groundR, groundX + groundR - scrollX, groundY + groundR, ground[groundtimer], TRUE);
					}
				}

				//ジャンプフラグごとのプレイヤーの向き(上にいる場合)
				if (jumpflag == 1)
				{
					DrawExtendGraph(playerX - playerR - scrollX, playerY - playerR, playerX + playerR - scrollX, playerY + playerR, player2[playertimer], TRUE);

					//地面のエフェクト
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

			//シーン3のブロック配置
			if (scene == 3)
			{
				for (int y = 0; y < 9; y++)
				{
					for (int x = 0; x < 192; x++)
					{
						//通常ブロック
						if (map2[y][x] == 1)
						{
							DrawGraph(x * blockSize - scrollX, y * blockSize, block2, TRUE);
						}

						//ゴールブロック
						if (map2[y][x] == 2)
						{
							DrawGraph(x * blockSize + 16 - scrollX, y * blockSize + 32, goal, TRUE);
						}
					}
				}
			}

			//シーン4のブロック配置
			else if (scene == 4)
			{
				for (int y = 0; y < 9; y++)
				{
					for (int x = 0; x < 192; x++)
					{
						//通常ブロック
						if (map[y][x] == 1)
						{
							DrawGraph(x * blockSize - scrollX, y * blockSize, block2, TRUE);
						}

						//ゴールブロック
						else if (map[y][x] == 2)
						{
							DrawGraph(x * blockSize + 16 - scrollX, y * blockSize + 32, goal, TRUE);
						}
					}
				}
			}
		}

		//シーン5(クリア画面)
		else if (scene == 5)
		{
			DrawGraph(clearX, clearY, clear, TRUE);
		}

		//シーン6(ゲームオーバー)
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

		//---------  ここまでにプログラムを記述  ---------//
		ScreenFlip();//（ダブルバッファ）裏面
		// 20ミリ秒待機（疑似60FPS）
		WaitTimer(20);
		// Windows システムからくる情報を処理する
		if (ProcessMessage() == -1)
		{
			break;
		}
		// ＥＳＣキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	//Dxライブラリ終了処理
	DxLib_End();

	return 0;
}
