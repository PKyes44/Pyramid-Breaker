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

// ���� Ÿ�̸�
int RELOAD_start = 0, RELOAD_end = 0;
bool STARTtime_count = false;

//�Ƕ�̵� ���
void Pyramid(int i);
//Ű�Է�
void InputProcess();
//GUI
void GUI();
//�Ƕ�̵� �̵�
void SumPY();
//�÷��̾� ���
void RenderPlayer();
//�⺻ ����
void Init();
//�Ƕ�̵� ����
void Spawn_Py();
//���� �޴�
void StartMenu();
//���� ���� �� ����
void GameMain();
//���ӽ��� �޴�
void GameFail();
//���� ���� üũ
void Check_RELOAD();
//���� Ÿ�̸�
void RELOAD_time();
//�Ѿ� ��� �� �̵�
void SumBullet();

int main(void)
{
	//ȭ��ũ�� ����
	system("mode con cols=50 lines=40");

	//�÷��̾���ǥ ���� �� �⺻����
	Init();

	//��ŸƮ�޴�
	StartMenu();

	//���ӽ���
	GameMain();

	return 0;
}

void Init() {
	//�÷��̾� ����
	g_stPlayer.x = 24;
	g_stPlayer.y = 30;
	g_stPlayer.Active = true;

	srand(time(NULL));
	
	// CMD ��� ���� ����
	SetColor(BackColor, 0);
}

void StartMenu() {
	while (1) {
		Clear();
		GotoXY(20, 10);
		printf("�Ƕ�̵� ����");
		GotoXY(10, 30);
		printf("�����Ϸ��� �ƹ� Ű�� �����ּ���");

		// �ƹ� Ű�� �Է� ���� ��� ���� ����
		if (_kbhit()) {
			break;
		}
		Sleep(100);
	}
}

void GameMain() {
	while (1) {
		Clear();

		//�Ƕ�̵� ����
		Spawn_Py();
		
		//�Ƕ�̵�
		SumPY();

		//�Է�Ű
		InputProcess();

		//�Ѿ����
		SumBullet();

		//�Ѿ�����
		RELOAD_time();

		//����üũ
		Check_RELOAD();

		//�÷��̾� ���
		RenderPlayer();

		//�������� , ���� ���
		GUI();

		Sleep(30);
	}
}


void Spawn_Py() {
	for (int i = 0; i < MAX; i++) {
		// �Ƕ�̵尡 Ȱ��ȭ �������
		if (!g_stPY[i].Active) {
			// ���� x��ǥ �Ҵ�
			srand(time(NULL));
			g_stPY[i].x = (rand() % 25) * 2;
			
			// ��ǥ�� �ִ��� ��ġ�� �ʰ�
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
			// y��ǥ �ʱ�ȭ
			g_stPY[i].y = 0;
			g_stPY[i].Active = true;
			// ���ÿ� ������ �ʰ� �ϱ� ���� for�� ������
			break;
		}
	}
}

void InputProcess() {
	// ���� ȭ��ǥ�� �Է������� ( �������� �̵�
	if (GetAsyncKeyState(VK_LEFT) && 0x8000) {
		g_stPlayer.x -= 2;
		if (g_stPlayer.x < 0) {
			g_stPlayer.x = 0;
		}
	}
	// ������ ȭ��ǥ�� �Է������� ( ���������� �̵�
	if (GetAsyncKeyState(VK_RIGHT) && 0x8000) {
		g_stPlayer.x += 2;
		if (g_stPlayer.x > 52) {
			g_stPlayer.x = 52;
		}
	}
	// �����̽��� �Է������� ( �Ѿ˹߻�
	if (GetAsyncKeyState(VK_SPACE) && 0x8000) {
		if (!g_stBullet.Active && !g_stBullet.Need_Reload) {
			g_stBullet.x = g_stPlayer.x;
			g_stBullet.y = g_stPlayer.y - 1;
			g_stBullet.Active = true;
		}
	}
}

void SumBullet() {
	// �Ѿ��� Ȱ��ȭ �Ǿ����� ���
	if (g_stBullet.Active) {
		// �߻� �� �����ʿ�
		g_stBullet.Need_Reload = true;
		// �Ѿ���ǥ�� Ŀ�� �̵� �� ���
		GotoXY(g_stBullet.x, g_stBullet.y);
		printf("��");
		// ����������
		g_stBullet.y--;
		// cmd ���� ������� ���ֱ�
		if (g_stBullet.y <= 0) {
			g_stBullet.Active = false;
		}
	}
}

void RELOAD_time() {
	// ������ �ʿ��Ұ��
	if (g_stBullet.Need_Reload) {
		// ���������� üũ�ؼ� �ϴ� ���̸� end�� ���Ҵ� ���� �ʰ�
		if (!STARTtime_count) {
			RELOAD_end = (unsigned)time(NULL);
			// ����ð� + 2 �ؼ� start�� end�� �����ϱ���� 2��
			RELOAD_end += 2;
			STARTtime_count = true;
		}
		// ��� ����ð� ���Ҵ�
		RELOAD_start = (unsigned)time(NULL);
	}
}

void Check_RELOAD() {
	// Ÿ�̸Ӱ� 2�ʿ� �������� �� �����Ϸ�
	if (RELOAD_end - RELOAD_start == 0) {
		g_stBullet.Need_Reload = false;
		STARTtime_count = false;
	}
}

void SumPY() {
	for (int i = 0; i < MAX; i++) {
		if (g_stPY[i].Active) {
			// Ŀ�� ��ǥ�� �̵�
			GotoXY(g_stPY[i].x, g_stPY[i].y);
			// �Ƕ�̵� ����
			Pyramid(i);
			// y ��ǥ ��� ���ؼ� ����������
			g_stPY[i].y++;

			// y�� CMD �ٴڿ� ������ ���ֱ�
			if (g_stPY[i].y == 39) {
				g_stPY[i].Active = false;
			}
		}
	}
}

// �Ƕ�̵� ����
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
		//�� �ѱ�
		GotoXY(g_stPY[i].x, g_stPY[i].y + (a + 1));
	}
}

void RenderPlayer() {
	//�÷��̾� ����
	if (g_stPlayer.Active) {
		GotoXY(g_stPlayer.x, g_stPlayer.y);
		printf("��");
	}

	for (int i = 0; i < MAX; i++) {
		// �Ѿ��� �����Ǿ� ���� ���
		if (g_stBullet.Active) {
			// �Ƕ�̵�� �Ѿ��� �´���� ��� �Ѿ� ����
			if (g_stBullet.x >= g_stPY[i].x && g_stBullet.x <= g_stPY[i].x + 6) {
				if (g_stBullet.y == g_stPY[i].y + 2) {
					g_stBullet.Active = false;
					g_stPY[i].Active = false;
				}
			}
		}
		// �Ƕ�̵�� �÷��̾ �´���� ��� ���ӿ���
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
	// ���� ���
	printf("Score : %d", g_iScore);
	if (g_stBullet.Need_Reload) {
		GotoXY(30, 0);
		// ���� �ʿ�� ���
		printf("RELOAD : %d�� / 2��", RELOAD_end - RELOAD_start);
	}
	else {
		GotoXY(42, 0);
		// �����Ϸ�� ���
		printf("�����Ϸ�");
	}
}

// ���ӽ���
void GameFail() {
	Clear();
	GotoXY(19, 10);
	printf("Ŭ���� ����!");
	GotoXY(10, 30);
	printf("%15s : %d\n%12d�� �� ������ ����˴ϴ�.", "Score", g_iScore, 1);
	Sleep(1000);
	exit(0);
}