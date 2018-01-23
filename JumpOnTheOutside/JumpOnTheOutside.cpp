# include <stdio.h>
# include <Windows.h>
# include <graphics.h>
# include <conio.h>
# include "atlimage.h"
#include <math.h>

IMAGE imge;			//���������ͼ
IMAGE img;			//���������ͼ���õĲ���
int xy[800][700];   //����img������
int x, y;           //����
DWORD* pMem;        //���洰���Դ�

bool ColorFun(COLORREF color1, COLORREF color2, int deff);
bool IsNewBlock(int color);

void main()
{
	int x1, y1;         //��һ��Ŀ��������
	int x2, y2;        //�ڶ���Ŀ��������

	while (true)
	{
		//	initgraph(800, 700);            //��ʼ������
		//	pMem = GetImageBuffer(NULL);    //�õ������Դ�
		//��������
		system("adb shell /system/bin/screencap -p /sdcard/screenshot.png");
		system("adb pull /sdcard/screenshot.png screenshot.png");

		//����PNG���ܼ��㣬ת��һ�¸�ʽ
		CImage cimge;
		cimge.Load(L"screenshot.png");
		cimge.Save(L"screenshot.jpg");

		//����ͼ�������
		loadimage(&imge, _T("screenshot.jpg"));

		//�и����Ҫ�Ĳ���
		SetWorkingImage(&imge);        //���õ�ǰ������   
		getimage(&img, 100, 600, 800, 700);

		SetWorkingImage(&img);
		for (y = 0; y < 700; y++)
		{
			for (x = 0; x < 800; x++)
			{
				xy[x][y] = getpixel(x, y);
				if (IsNewBlock(xy[x][y]))
				{
					x1 = x;
					y1 = y;
					goto next1;
				}
			}
		}
	next1:
		//Ѱ���˵�β��
		for (y = 699; y >= 0; y--)
		{
			for (x = 0; x < 800; x++)
			{
				xy[x][y] = getpixel(x, y);
				if (ColorFun(xy[x][y], RGB(55, 60, 100), 2))
				{
					x2 = x;
					y2 = y;
					goto next2;
				}
			}
		}
	next2:

		SetWorkingImage(NULL);        //���õ�ǰ����Ϊ������

		//Ҫ��������
		int xxx = x1 + 14;
		int yyy = y1 + 60;
		if (x1 > x2)
		{
			xxx = x1 + 40;
			yyy = y1 + 50;
		}
		else
		{
			xxx = x1 - 10;
			yyy = y1 + 30;
		}

		char str[100];
		int dis;
		dis = sqrt(double((xxx - x2)*(xxx - x2) + (yyy - y2)*(yyy - y2)));
		int time;
		time = dis * 1.25;
		sprintf(str, "adb shell input swipe 100 350 400 450 %d", (int)time);
		system(str);



		////��ʾ��Ƭ
		//putimage(0, 0, &img);

		//cleardevice();   //�������
		//for (x = 0; x < 800; x++)
		//{
		//	for (y = 0; y < 700; y++)
		//	{
		//		int b = xy[x][y] & 0xff;  //�Ͱ�λ
		//		int g = (xy[x][y] >> 8) & 0xff; //ȥ���Ͱ�λ����ȡ��λ
		//		int r = xy[x][y] >> 16;     //��ʮ��λ

		//		pMem[y * 800 + x] = BGR(xy[x][y]);
		//	}
		//}
		//FlushBatchDraw();    //�������ص���Ƶ�ͼƬ��ʾ����
		//system("pause");

		//closegraph();   //�ر�ͼ�ν���
		//printf("x1 = %d;   y1 = %d\n", x1, y1);
		//printf("x2 = %d;   y2 = %d\n", x2, y2);

		//system("pause");
		Sleep(1000);
	}

}
//��ɫ���ƶ�
bool ColorFun(COLORREF color1, COLORREF color2, int deff)
{
	int r1 = GetRValue(color1);
	int g1 = GetGValue(color1);
	int b1 = GetBValue(color1);

	int r2 = GetRValue(color2);
	int g2 = GetGValue(color2);
	int b2 = GetBValue(color2);

	if (sqrt(double((r2 - r1)*(r2 - r1) + (g2 - g1)*(g2 - g1) + (b2 - b1)*(b2 - b1))) < deff)
		return true;

	return false;
}
//�ж�������ǲ��Ǻ��ӵ���ɫ
bool IsNewBlock(int color)
{
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	//ǰ�ж��ǲ�����
	if (ColorFun(color, RGB(55, 60, 100), 10))
	{
		return false;
	}

	//ǳ��ɫ
	if (ColorFun(color, RGB(246, 246, 246), 10))
	{
		return TRUE;
	}
	else if (ColorFun(color, RGB(250, 250, 250), 10))
	{
		return TRUE;
	}
	//����ɫ
	else if (ColorFun(color, RGB(255, 255, 255), 0))
	{
		return TRUE;
	}
	//ī��ɫ  100 148 106
	else if (ColorFun(color, RGB(100, 148, 106), 20))
	{
		return TRUE;
	}
	//���ɫ 
	else if (ColorFun(color, RGB(113, 113, 113), 10))
	{
		return TRUE;
	}
	//��ɫ
	else if (ColorFun(color, RGB(245, 128, 58), 10))
	{
		return TRUE;
	}
	//ǳ��ɫ
	else if (ColorFun(color, RGB(186, 239, 69), 10))
	{
		return TRUE;
	}
	//ľ������
	else if (ColorFun(color, RGB(234, 203, 174), 10))
	{
		return TRUE;
	}
	//��ɫ
	else if (ColorFun(color, RGB(254, 240, 89), 10))
	{
		return TRUE;
	}
	//��ɫ���
	else if (r > 124 && r<134 && g>111 && g<121 && b > 219 && b<229)
		//else if (r == 129 && g == 116 && b == 224)
	{
		return TRUE;
	}
	//��Բ������
	else if (r > 75 && r<85 && g>158 && g<165 && b > 85 && b<95)
	{
		return TRUE;
	}
	//��ɫ
	else if (ColorFun(color, RGB(254, 74, 83), 10))
	{
		return TRUE;
	}
	//����ʯ
	else if (ColorFun(color, RGB(152, 114, 111), 10))
	{
		return TRUE;
	}
	//��Ͱ
	else if (ColorFun(color, RGB(117, 117, 117), 10))
	{
		return TRUE;
	}
	else if (ColorFun(color, RGB(225, 199, 142), 10))
	{
		return TRUE;
	}
	//�鱾
	else if (ColorFun(color, RGB(241, 241, 241), 10))
	{
		return TRUE;
	}
	//��ɫ����
	else if (ColorFun(color, RGB(255, 172, 178), 10))
	{
		return TRUE;
	}
	//�̲豭��
	else if (ColorFun(color, RGB(73, 73, 73), 3))
	{
		return TRUE;
	}
	//���Ƴ�Ƭ��
	else if (ColorFun(color, RGB(147, 147, 147), 10))
	{
		return TRUE;
	}
	return FALSE;
}

//char str[100];
//int dis;
//int tX = 100;
//int tY = 350;
//int time;
//while (true)
//{
//	printf("��������� : ");
//	scanf("%d", &dis);
//	time = dis * 22.5;
//	sprintf(str, "adb shell input swipe %d %d %d %d %d", tX, tY, tX + 5, tY - 10, (int)time);
//	system(str);
//}