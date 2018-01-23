# include <stdio.h>
# include <Windows.h>
# include <graphics.h>
# include <conio.h>
# include "atlimage.h"
#include <math.h>

IMAGE imge;			//用来保存截图
IMAGE img;			//用来保存截图有用的部分
int xy[800][700];   //保存img的像素
int x, y;           //坐标
DWORD* pMem;        //保存窗口显存

bool ColorFun(COLORREF color1, COLORREF color2, int deff);
bool IsNewBlock(int color);

void main()
{
	int x1, y1;         //第一个目标点的坐标
	int x2, y2;        //第二个目标点的坐标

	while (true)
	{
		//	initgraph(800, 700);            //初始化窗口
		//	pMem = GetImageBuffer(NULL);    //得到窗口显存
		//截屏保存
		system("adb shell /system/bin/screencap -p /sdcard/screenshot.png");
		system("adb pull /sdcard/screenshot.png screenshot.png");

		//由于PNG不能计算，转化一下格式
		CImage cimge;
		cimge.Load(L"screenshot.png");
		cimge.Save(L"screenshot.jpg");

		//将截图载入变量
		loadimage(&imge, _T("screenshot.jpg"));

		//切割出需要的部分
		SetWorkingImage(&imge);        //设置当前工作区   
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
		//寻找人的尾部
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

		SetWorkingImage(NULL);        //设置当前窗口为工作区

		//要跳的坐标
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



		////显示照片
		//putimage(0, 0, &img);

		//cleardevice();   //清除窗口
		//for (x = 0; x < 800; x++)
		//{
		//	for (y = 0; y < 700; y++)
		//	{
		//		int b = xy[x][y] & 0xff;  //低八位
		//		int g = (xy[x][y] >> 8) & 0xff; //去掉低八位，再取八位
		//		int r = xy[x][y] >> 16;     //高十六位

		//		pMem[y * 800 + x] = BGR(xy[x][y]);
		//	}
		//}
		//FlushBatchDraw();    //把用像素点绘制的图片显示出来
		//system("pause");

		//closegraph();   //关闭图形界面
		//printf("x1 = %d;   y1 = %d\n", x1, y1);
		//printf("x2 = %d;   y2 = %d\n", x2, y2);

		//system("pause");
		Sleep(1000);
	}

}
//颜色相似度
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
//判断这个点是不是盒子的颜色
bool IsNewBlock(int color)
{
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	//前判断是不是人
	if (ColorFun(color, RGB(55, 60, 100), 10))
	{
		return false;
	}

	//浅白色
	if (ColorFun(color, RGB(246, 246, 246), 10))
	{
		return TRUE;
	}
	else if (ColorFun(color, RGB(250, 250, 250), 10))
	{
		return TRUE;
	}
	//纯白色
	else if (ColorFun(color, RGB(255, 255, 255), 0))
	{
		return TRUE;
	}
	//墨绿色  100 148 106
	else if (ColorFun(color, RGB(100, 148, 106), 20))
	{
		return TRUE;
	}
	//深灰色 
	else if (ColorFun(color, RGB(113, 113, 113), 10))
	{
		return TRUE;
	}
	//橙色
	else if (ColorFun(color, RGB(245, 128, 58), 10))
	{
		return TRUE;
	}
	//浅绿色
	else if (ColorFun(color, RGB(186, 239, 69), 10))
	{
		return TRUE;
	}
	//木质桌子
	else if (ColorFun(color, RGB(234, 203, 174), 10))
	{
		return TRUE;
	}
	//黄色
	else if (ColorFun(color, RGB(254, 240, 89), 10))
	{
		return TRUE;
	}
	//紫色相间
	else if (r > 124 && r<134 && g>111 && g<121 && b > 219 && b<229)
		//else if (r == 129 && g == 116 && b == 224)
	{
		return TRUE;
	}
	//大圆绿柱子
	else if (r > 75 && r<85 && g>158 && g<165 && b > 85 && b<95)
	{
		return TRUE;
	}
	//红色
	else if (ColorFun(color, RGB(254, 74, 83), 10))
	{
		return TRUE;
	}
	//华岩石
	else if (ColorFun(color, RGB(152, 114, 111), 10))
	{
		return TRUE;
	}
	//马桶
	else if (ColorFun(color, RGB(117, 117, 117), 10))
	{
		return TRUE;
	}
	else if (ColorFun(color, RGB(225, 199, 142), 10))
	{
		return TRUE;
	}
	//书本
	else if (ColorFun(color, RGB(241, 241, 241), 10))
	{
		return TRUE;
	}
	//粉色盒子
	else if (ColorFun(color, RGB(255, 172, 178), 10))
	{
		return TRUE;
	}
	//奶茶杯子
	else if (ColorFun(color, RGB(73, 73, 73), 3))
	{
		return TRUE;
	}
	//类似唱片机
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
//	printf("请输入距离 : ");
//	scanf("%d", &dis);
//	time = dis * 22.5;
//	sprintf(str, "adb shell input swipe %d %d %d %d %d", tX, tY, tX + 5, tY - 10, (int)time);
//	system(str);
//}