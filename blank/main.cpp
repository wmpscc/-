/*  ��Ŀ�е��꾡���ͣ�http://t.cn/RHjgacn  */

#include <graphics.h>
#include <math.h>
#include<string.h>
#include<stdio.h>
#include <windows.h>  
#pragma comment(lib, "Winmm.lib")            //For MCI(Media Control Interface��ý����ƽӿ�)  

#define Path_Music_resPiano "..//resplus//piano"	//�������
#define Path_Music_Fail "..//resplus//fail.mp3"		//ʧ������
#define Path_Music_Launch "..//resplus//chuanshao.mp3"	//��������
#define Path_pre "..//resplus//piano//"	//��Ƶ·��ǰ׺
#define Path_end ".mp3"	//��Ƶ·����׺
#define WINDOW_HIGHT 800	//���ڸ߶�
#define WINDOW_WIDTH 480	//���ڿ��
#define Blank_HIGHT 160		//����߶�
#define Blank_WIDTH 120		//������
#define Blank_COLOR EGERGB(00,00,00)	//������ɫ
#define COLOR_WHITE EGERGB(0xFF, 0xFF, 0xFF)	//��ɫ
#define FPS 100	//FPS
#define SLEEP 10 //�ӳ�����
#define TopLineHight 20	//������ʾ����߶�
#define v 1 // ��ʼ�ٶ�

//-------��������--------
void drawTopLine(int);	//���Ƽ�����
void initFile();	//��ʼ���ļ���Դ
void debugouttext(int i, char c[]); 	//�������
DWORD WINAPI music_play(LPVOID pParam); 	//��������
void music_play_control(char key[]);	//�������ֲ���
DWORD WINAPI GameControl();		//����
void GameView();	//	��ͼ
void creatView();	//������ͼ

												//-------ȫ�ֱ���--------
int mScore = 0;	//����
char mCharsScore[50];	//�����ַ���
char track[1024][12];	//����
int mLength = 0;	//������
int below = 0;	//������һ�����鶨���y��
PIMAGE totalimg;	//�ܻ���
PIMAGE topline;	//������ʾ����
PIMAGE live;	//����ֵͼ��
int randtrack[3000] = { -1000 };	//�����������
int inter_y = 0;	//����ͼƬʱ�õ���y
bool failplay = false;	//��Ϸ�Ƿ����
int accelerate = 0;	//��Ϸ���ٶ�
int liveValue = 10;	//����ֵ

char Path_A_Breeze_From_Alabama[] = "..//resplus//A Breeze From Alabama.txt";
char Path_Happy_New_Year[] = "..//resplus//Happy New Year.txt";
char Path_Jasmine[] = "..//resplus//Jasmine.txt";
char Path_Little_Star[] = "..//resplus//Little Star.txt";
char Path_lyricwaltz[] = "..//resplus//lyricwaltz.txt";
char Path_Merry_Christmas[] = "..//resplus//Merry Christmas.txt";
char Path_Music[50];

typedef struct ThreadParameter {	//��Ƶ�����̲߳���
	char filename[200];
	char key[30];
};

ThreadParameter tpp;	//��Ƶ�����̴߳��νṹ��
int main()	//������
{
	//--------������������----------begin
	ThreadParameter tp;
	strcpy(tp.filename, Path_Music_Launch);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)music_play, &tp, 0, NULL);
	//--------������������----------end

	//------------��ʼ��------------begin
	setinitmode(RENDER_MANUAL);
	initgraph(WINDOW_WIDTH, WINDOW_HIGHT, INIT_RENDERMANUAL);	//��ʼ������
	topline = newimage(WINDOW_WIDTH, TopLineHight + 1);	//��ʼ�����ڶ�����ʾ���򻭲�
	live = newimage(20, 20);
	setbkcolor(EGERGB(0xDC, 0x14, 0x3C), live);		//����ֵͼƬ����
	cleardevice(live);
	strcpy(Path_Music, Path_Jasmine);	//Ĭ�Ϲؿ�
	//------------��ʼ��------------end

	//----------ѡ��ؿ�------------begin
	setfont(20, 0, "����");
	outtextxy(10, 10, "���¶�Ӧ��ĸ����ؿ�");
	outtextxy(10, 35, "A:	A Breeze From Alabama");
	outtextxy(10, 55, "B:	����");
	outtextxy(10, 75, "C:	Happy New Year");
	outtextxy(10, 95, "D:	Little Star");
	outtextxy(10, 115, "E:	lyricwaltz");
	outtextxy(10, 135, "F:	Merry Christmas");
	switch (getch())
	{
	case 'A':
		strcpy(Path_Music, Path_A_Breeze_From_Alabama);
		break;
	case 'B':
		strcpy(Path_Music, Path_Jasmine);
		break;
	case 'C':
		strcpy(Path_Music, Path_Happy_New_Year);
		break;
	case 'D':
		strcpy(Path_Music, Path_Little_Star);
		break;
	case 'E':
		strcpy(Path_Music, Path_lyricwaltz);
		break;
	case 'F':
		strcpy(Path_Music, Path_Merry_Christmas);
		break;
	default:
		break;
	}
	//----------ѡ��ؿ�------------end
	initFile();	//��������ļ�
	creatView();	//������ͼ
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GameControl, NULL, 0, NULL);	//�������̿����߳�

	GameView();	//��ͼ
	getch();	//��ֹ��Ϸ�������˳�
	return 0;
}

void initFile()	//��ʼ���ļ���Դ
{
	FILE *fp = NULL;
	fp = fopen(Path_Music, "r");
	if (fp != NULL)
	{
		while (fscanf(fp, "%s", track[mLength]) != EOF)
		{	//�������м�
			mLength++;
		}
	}
	fclose(fp);
}

DWORD WINAPI ThreadDrawTrack()
{
	setrendermode(RENDER_MANUAL);
	int y = inter_y;
	int addh = 0;
	ThreadParameter tp;
	strcpy(tp.filename, Path_Music_Fail);
	for (; is_run(); delay_fps(FPS))
	{
		if (failplay)
		{
			cleardevice();
			sprintf(mCharsScore, "%d", mScore);
			setfont(100, 0, "����");
			setfontbkcolor(EGERGB(0xff, 0x00, 0x00));
			outtextxy(WINDOW_WIDTH / 2 + 20, WINDOW_HIGHT / 2, mCharsScore);
			outtextxy(0, WINDOW_HIGHT / 2, "�÷�:");
			break;
		}
		if (-1 * y >= 0)
		{
			break;
		}
		putimage(0, -1 * y, totalimg);
		y -= v + accelerate;
		below = addh - mScore*Blank_HIGHT - 160;
		if (below >= WINDOW_HIGHT)
		{
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)music_play, &tp, 0, NULL);
			failplay = true;
		}
		addh += v;
		drawTopLine(liveValue);	//������ʾ�÷�
	}
	return 1;
}

void GameView()	//	��ͼ
{
	randomize();	//��ʼ�������
	cleardevice();	//����
	setcaption("��Ȱ׿� - 1707004544 ���");		//���ڱ���
	setbkcolor(EGERGB(0xff, 0xff, 0xff));			//����ɫ
	setbkcolor(EGERGB(0xff, 0xff, 0xff),topline);	//������ʾ���򱳾�ɫ
	drawTopLine(liveValue);
	ThreadDrawTrack();
}

DWORD WINAPI GameControl()	//����
{
	int curr = 0;
	int k = 0, index = 0;
	ThreadParameter tp;
	strcpy(tp.filename, Path_Music_Fail);	//���̴߳��νṹ�帳ֵʧ������·��
	for (; k != key_esc;)
	{
		if (kbhit())
		{
			k = getch();
			if (k == 'S')
			{
				if (randtrack[mLength - curr - 1] == 1)
				{
					music_play_control(track[index++]);		//�������������ִ�����
					mScore++;	//�÷ּ�һ
					drawTopLine(liveValue);	//ˢ�¼Ʒֱ�ǩ
				}
				else
				{
					liveValue--;
					if (liveValue <= 0)
					{
						CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)music_play, &tp, 0, NULL);	//����ʧ�������߳�
						failplay = true;	//�趨ʧ�ܱ�־Ϊ��
					}
					else
					{
						continue;
					}
				}
			}
			else if (k == 'D')
			{
				if (randtrack[mLength - curr - 1] == 2)
				{
					music_play_control(track[index++]);
					mScore++;
					drawTopLine(liveValue);
				}
				else
				{
					liveValue--;
					if (liveValue <= 0)
					{
						CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)music_play, &tp, 0, NULL);	//����ʧ�������߳�
						failplay = true;	//�趨ʧ�ܱ�־Ϊ��
					}
					else
					{
						continue;
					}
				}
			}
			else if (k == '4')
			{
				if (randtrack[mLength - curr - 1] == 3)
				{
					music_play_control(track[index++]);
					mScore++;
					drawTopLine(liveValue);
				}
				else
				{
					liveValue--;
					if (liveValue <= 0)
					{
						CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)music_play, &tp, 0, NULL);	//����ʧ�������߳�
						failplay = true;	//�趨ʧ�ܱ�־Ϊ��
					}
					else
					{
						continue;
					}
				}
			}
			else if (k == '5')
			{
				if (randtrack[mLength - curr - 1] == 4)
				{
					music_play_control(track[index++]);
					mScore++;
					drawTopLine(liveValue);
				}
				else
				{
					liveValue--;
					if (liveValue <= 0)
					{
						CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)music_play, &tp, 0, NULL);	//����ʧ�������߳�
						failplay = true;	//�趨ʧ�ܱ�־Ϊ��
					}
					else
					{
						continue;
					}
				}
			}
			curr++;
			accelerate = mScore / 30;	//�趨����ÿ25�������ٶ�
		}
		if (failplay)
		{
			api_sleep(500);	//�ȴ����������߳�����
			return 0;
		}
	}
	return 0;
}

void drawTopLine(int n)	//���Ƽ�����
{
	int width = 0;
	cleardevice(topline);
	sprintf(mCharsScore, "%d", mScore);		//intתchar*
	setfont(40, 0, "����");		//�趨������ʽ
	setfontbkcolor(EGERGB(0xff, 0x00, 0x00));	//�������屳��ɫ
	outtextrect(WINDOW_WIDTH - 40, 0, 20, 20, mCharsScore, topline);	//�ڴ����������
	for (int i = 0; i < n; i++)
	{
		putimage(topline, width, 0, live);
		width += 22;
	}
	putimage(0, 0, topline);
}

void creatView()
{
	int point[8];	//�ı��ε㼯
	int x = 0, y = 0;	//����������
	randomize();	//��ʼ�������
	totalimg = newimage(WINDOW_WIDTH, Blank_HIGHT*mLength);	//��ʼ���ܻ���
	setbkcolor(COLOR_WHITE, totalimg);	//�����ܻ�������
	cleardevice(totalimg);	//����ܻ���
	for (int i = 0; i < mLength; i++)
	{
		randtrack[i] = random(4) + 1;	//����0-3�����������һ��Ϊ���λ��
		y = inter_y;
		x = (randtrack[i] - 1) * Blank_WIDTH;
		point[0] = x;
		point[1] = y;
		point[2] = x + Blank_WIDTH;
		point[3] = y;
		point[4] = x + Blank_WIDTH;
		point[5] = y + Blank_HIGHT;
		point[6] = x;
		point[7] = y + Blank_HIGHT;
		setfillstyle(SOLID_FILL, Blank_COLOR);	//���÷�����ɫ
		fillpoly(4, point, totalimg);	//���Ʒ���
		inter_y += Blank_HIGHT;	//���û��鹫���߶�
	}
}

void music_play_control(char key[])	//�������ֲ���
{
	int flage = 0;
	char copy[30];
	char buff[10][30];	//ͬʱ���µļ�ֵ
	char temp[200];
	memset(temp, '\0', sizeof(temp));
	//�����ض���ʽ����ÿ������
	if (key[0] != '(')
	{
		strcat(temp, Path_pre);
		strcat(temp, key);
		strcat(temp, Path_end);
		strcpy(tpp.filename, temp);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)music_play, &tpp, 0, NULL);
	}
	else
	{
		memset(buff, '\0', sizeof(buff));
		strcpy(copy, key);
		for (int i = 0; i < strlen(copy); i++)
		{
			if (copy[i] == '.')
			{
				flage++;
				copy[i] = ' ';
			}
			else if (copy[i] == '(' || copy[i] == ')')
			{
				copy[i] = ' ';
			}
		}
		if (flage == 0)
		{
			sscanf(copy, "%s", buff[0]);
		}
		else if (flage == 1)
		{
			sscanf(copy, "%s %s", buff[0], buff[1]);
		}
		else if (flage == 2)
		{
			sscanf(copy, "%s %s %s", buff[0], buff[1], buff[2]);
		}
		else if (flage == 3)
		{
			sscanf(copy, "%s %s %s %s", buff[0], buff[1], buff[2], buff[3]);
		}
		for (int i = 0; i < flage; i++)
		{
			strcat(temp, Path_pre);
			strcat(temp, buff[i]);
			strcat(temp, Path_end);
			strcpy(tpp.filename, temp);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)music_play, &tpp, 0, NULL);	//�������ֲ����߳�
			api_sleep(200);
		}
	}
}
DWORD WINAPI music_play(LPVOID pParam)	//��������
{
	//mciSendString(tp->filename, NULL, 0, NULL); //��Ч���ŷ�ʽ

	//��Ч������������
	ThreadParameter *tp = (ThreadParameter *)pParam;
	MCI_OPEN_PARMS open;//����򿪽ṹ�����
	open.lpstrElementName = tp->filename;//������
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, DWORD_PTR(&open));//��
	MCI_PLAY_PARMS play;//���岥�Žṹ�����
	mciSendCommand(open.wDeviceID, MCI_PLAY, 0, DWORD_PTR(&play));//������ʽ����
	return 0;
}

void debugouttext(int i, char c[])	//�������
{
	//�ڴ���������ض���Ϣ��������
	char temp[20];
	if (c != NULL)
	{
		outtextxy(0, 0, c);
	}
	else
	{
		sprintf(temp, "%d", i);
		outtextxy(10, 10, temp);
	}

}

