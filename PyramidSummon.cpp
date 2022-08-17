#define DIFFI 3
#define MAX 10
#define BackColor 15

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "Console.h"

struct ST_OBJ {
	int x = 0;
	int y = 0;
	bool Active = false;
	bool Need_Reload = false;
};

ST_OBJ g_stPY[MAX];
ST_OBJ g_stPlayer;
ST_OBJ g_stBullet;
int g_iScore = 0;

// 장전 타이머
int RELOAD_start = 0, RELOAD_end = 0;
bool STARTtime_count = false;

//피라미드 출력
void Pyramid(int i);
//키입력
void InputProcess();
//GUI
void GUI();
//피라미드 이동
void SumPY();
//플레이어 출력
void RenderPlayer();
//기본 세팅
void Init();
//피라미드 생성
void Spawn_Py();
//시작 메뉴
void StartMenu();
//게임 시작 후 메인
void GameMain();
//게임실패 메뉴
void GameFail();
//장전 여부 체크
void Check_RELOAD();
//장전 타이머
void RELOAD_time();
//총알 출력 및 이동
void SumBullet();

int main(void)
{
	//화면크기 설정
	system("mode con cols=50 lines=40");

	//플레이어좌표 설정 및 기본세팅
	Init();

	//스타트메뉴
	StartMenu();

	//게임시작
	GameMain();

	return 0;
}

void Init() {
	//플레이어 세팅
	g_stPlayer.x = 24;
	g_stPlayer.y = 30;
	g_stPlayer.Active = true;

	srand(time(NULL));
	
	// CMD 배경 색깔 설정
	SetColor(BackColor, 0);
}

void StartMenu() {
	while (1) {
		Clear();
		GotoXY(20, 10);
		printf("피라미드 격파");
		GotoXY(10, 30);
		printf("시작하려면 아무 키나 눌러주세요");

		// 아무 키나 입력 됐을 경우 게임 시작
		if (_kbhit()) {
			break;
		}
		Sleep(100);
	}
}

void GameMain() {
	while (1) {
		Clear();

		//피라미드 생성
		Spawn_Py();
		
		//피라미드
		SumPY();

		//입력키
		InputProcess();

		//총알출력
		SumBullet();

		//총알장전
		RELOAD_time();

		//장전체크
		Check_RELOAD();

		//플레이어 출력
		RenderPlayer();

		//장전여부 , 점수 출력
		GUI();

		Sleep(30);
	}
}


void Spawn_Py() {
	for (int i = 0; i < MAX; i++) {
		// 피라미드가 활성화 됐을경우
		if (!g_stPY[i].Active) {
			// 랜덤 x좌표 할당
			srand(time(NULL));
			g_stPY[i].x = (rand() % 25) * 2;
			
			// 좌표값 최대한 겹치지 않게
			for (int j = 0; j < i; j++) {
				while (1) {
					if (g_stPY[j].x == g_stPY[i].x) {
						g_stPY[i].x = (rand() % 25) * 2;
					}
					else {
						break;
					}
				}
			}
			// y좌표 초기화
			g_stPY[i].y = 0;
			g_stPY[i].Active = true;
			// 동시에 나오지 않게 하기 위해 for문 나가기
			break;
		}
	}
}

void InputProcess() {
	// 왼쪽 화살표를 입력했을때 ( 왼쪽으로 이동
	if (GetAsyncKeyState(VK_LEFT) && 0x8000) {
		g_stPlayer.x -= 2;
		if (g_stPlayer.x < 0) {
			g_stPlayer.x = 0;
		}
	}
	// 오른쪽 화살표를 입력했을때 ( 오른쪽으로 이동
	if (GetAsyncKeyState(VK_RIGHT) && 0x8000) {
		g_stPlayer.x += 2;
		if (g_stPlayer.x > 52) {
			g_stPlayer.x = 52;
		}
	}
	// 스페이스바 입력했을때 ( 총알발사
	if (GetAsyncKeyState(VK_SPACE) && 0x8000) {
		if (!g_stBullet.Active && !g_stBullet.Need_Reload) {
			g_stBullet.x = g_stPlayer.x;
			g_stBullet.y = g_stPlayer.y - 1;
			g_stBullet.Active = true;
		}
	}
}

void SumBullet() {
	// 총알이 활성화 되어있을 경우
	if (g_stBullet.Active) {
		// 발사 후 장전필요
		g_stBullet.Need_Reload = true;
		// 총알좌표로 커서 이동 후 출력
		GotoXY(g_stBullet.x, g_stBullet.y);
		printf("↑");
		// 떨어지도록
		g_stBullet.y--;
		// cmd 끝에 닿았을때 없애기
		if (g_stBullet.y <= 0) {
			g_stBullet.Active = false;
		}
	}
}

void RELOAD_time() {
	// 장전이 필요할경우
	if (g_stBullet.Need_Reload) {
		// 장전중인지 체크해서 하는 중이면 end가 재할당 되지 않게
		if (!STARTtime_count) {
			RELOAD_end = (unsigned)time(NULL);
			// 현재시간 + 2 해서 start가 end에 도달하기까지 2초
			RELOAD_end += 2;
			STARTtime_count = true;
		}
		// 계속 현재시간 재할당
		RELOAD_start = (unsigned)time(NULL);
	}
}

void Check_RELOAD() {
	// 타이머가 2초에 도달했을 때 장전완료
	if (RELOAD_end - RELOAD_start == 0) {
		g_stBullet.Need_Reload = false;
		STARTtime_count = false;
	}
}

void SumPY() {
	for (int i = 0; i < MAX; i++) {
		if (g_stPY[i].Active) {
			// 커서 좌표로 이동
			GotoXY(g_stPY[i].x, g_stPY[i].y);
			// 피라미드 생성
			Pyramid(i);
			// y 좌표 계속 더해서 떨어지도록
			g_stPY[i].y++;

			// y가 CMD 바닥에 갔을때 없애기
			if (g_stPY[i].y == 39) {
				g_stPY[i].Active = false;
			}
		}
	}
}

// 피라미드 생성
void Pyramid(int i) {
	int a;
	int b;
	int c;

	for (a = 0; a < DIFFI; a++)
	{
		for (b = a; b <= DIFFI - 1; b++) {
			printf(" ");
		}
		for (c = 0; c <= (a * 2); c++) {
			printf("*");
		}
		//줄 넘김
		GotoXY(g_stPY[i].x, g_stPY[i].y + (a + 1));
	}
}

void RenderPlayer() {
	//플레이어 생성
	if (g_stPlayer.Active) {
		GotoXY(g_stPlayer.x, g_stPlayer.y);
		printf("▲");
	}

	for (int i = 0; i < MAX; i++) {
		// 총알이 생성되어 있을 경우
		if (g_stBullet.Active) {
			// 피라미드와 총알이 맞닿았을 경우 총알 제거
			if (g_stBullet.x >= g_stPY[i].x && g_stBullet.x <= g_stPY[i].x + 6) {
				if (g_stBullet.y == g_stPY[i].y + 2) {
					g_stBullet.Active = false;
					g_stPY[i].Active = false;
				}
			}
		}
		// 피라미드와 플레이어가 맞닿았을 경우 게임오버
		if (g_stPY[i].y + 2 == g_stPlayer.y) {
			if (g_stPlayer.x >= g_stPY[i].x && g_stPlayer.x <= g_stPY[i].x + 4) {
				g_stPlayer.Active = false;
				GameFail();
			}
		}
	}
}

void GUI() {
	g_iScore++;
	GotoXY(0, 0);
	// 점수 출력
	printf("Score : %d", g_iScore);
	if (g_stBullet.Need_Reload) {
		GotoXY(30, 0);
		// 장전 필요시 출력
		printf("RELOAD : %d초 / 2초", RELOAD_end - RELOAD_start);
	}
	else {
		GotoXY(42, 0);
		// 장전완료시 출력
		printf("장전완료");
	}
}

// 게임실패
void GameFail() {
	Clear();
	GotoXY(19, 10);
	printf("클리어 실패!");
	GotoXY(10, 30);
	printf("%15s : %d\n%12d초 후 게임이 종료됩니다.", "Score", g_iScore, 1);
	Sleep(1000);
	exit(0);
}