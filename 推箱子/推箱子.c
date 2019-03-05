#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#define U 1
#define D 2
#define L 3 
#define R 4		//按键状态，U：上 ；D：下；L：左 R：右
#define ofx	8
#define ofy	4	//x,y方向偏移量
typedef struct MAP		//地图
{
	int wall[50][2];	//墙
	int box[9][2];		//箱子
	int des[9][2];		//终点
	int overlap[9][2];//箱子与终点的重叠位置
	int all,now;			//总箱子个数,到位箱子个数
	int x,y;					//起点
}MAP,*map;
//全局变量//
int player[2];	//玩家位置
int nextp[2],nextb[2];	//玩家下一步位置,箱子下一步位置
MAP Pass[5];	//关卡数组
map Group,p;	//关卡数组指针,当前关卡指针
int level;		//关卡等级
int status;		//玩家状态
int boxc[9][2],overlapc[9][2];		//箱子状态栈,重叠箱子状态栈
int nowbox;		//当前到位箱子个数
int regame;		//是否重玩
//声明全部函数//
void Pos(int x,int y);	//设置光标位置
void welcometogame();	//开始界面
void initgame();		//初始化游戏
void startgame();	//开始游戏
void copybox(int copy[9][2],int b[9][2]);	//复制箱子信息
void loadmap();		//加载地图
void printfoverlap(int b[9][2]);	//打印重叠箱子位置
void move();	//玩家移动
void trans(int p[2],int q[2]);		//状态传递
void inputmap(int a[][2]);	//输入地图数据
void gamecircle();	//控制游戏
int main()
{
	system("mode con cols=80 lines=30");	//控制台宽度80，高度30
	welcometogame();
	initgame();
	for(level=0;level<4;level++)
	{
		p=Group+level;
		startgame();
	}
	system("cls");
	Pos(32,14);
	printf("恭喜你已通过所有关卡！\n");
	system("pause");
}
void Pos(int x,int y)		//设置光标位置
{
	COORD pos;
	HANDLE hOutput;
	pos.X=x;pos.Y=y;
	hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput,pos);
}
void welcometogame()	//开始界面
{
	system("title C语言程序游戏之推箱子");
	Pos(31,13);
	printf("欢迎来到推箱子游戏！");
	Pos(55,25);
	printf("    	作者：薛奇\n");
	system("pause");
	system("cls");
	Pos(29,12);
	printf("用↑.↓.←.→控制人物的移动。");
	Pos(29,13);
	printf("按F2重新开始此局。");
	Pos(29,14);
	printf("按ESC结束游戏。\n");
	system("pause");
	system("cls");
}
void startgame()	//开始游戏
{
	while(1)
	{
		system("cls");
		Pos(25,5);
		printf("第%d关",level+1);
		Pos(40,2);
		printf("按F2重新开始此局。");
		Pos(40,3);
		printf("按ESC结束游戏。");
		loadmap();
		gamecircle();
		if(regame==0)		//regame为0不重玩
			break;
		else if(regame==1)		//regame为1重玩
			regame=0;
	}
	Sleep(300);
}
void loadmap()	//加载地图
{
	int i;
	Pos(2*(p->wall[0][0]+ofx),p->wall[0][1]+ofy);
	printf("■");
	for(i=1;p->wall[i][0]!=0 || p->wall[i][1]!=0;i++)//打印墙
	{
		Pos(2*(p->wall[i][0]+ofx),p->wall[i][1]+ofy);
		printf("■");
	}
	for(i=0;p->des[i][0]!=0 || p->des[i][1]!=0;i++)//打印终点
	{
		Pos(2*(p->des[i][0]+ofx),p->des[i][1]+ofy);
		printf("×");
	}
	for(i=0;p->box[i][0]!=0 || p->box[i][1]!=0;i++)//打印箱子
	{
		Pos(2*(p->box[i][0]+ofx),p->box[i][1]+ofy);
		printf("◇");
	}
	printfoverlap(p->overlap);		//打印重叠箱子位置
	Pos(2*(p->x+ofx),p->y+ofy);	//打印玩家位置
	printf(":)");
	Pos(79,29);
}
void printfoverlap(int b[9][2])	//打印重叠箱子位置
{
	int i;
	for(i=0;i<9;i++)
	{
		if(b[i][0]==0&&b[i][1]==0)
			continue;
		Pos(2*(b[i][0]+ofx),b[i][1]+ofy);
		printf("□");
	}
}
void gamecircle()	//控制游戏
{
	copybox(boxc,p->box);
	copybox(overlapc,p->overlap);
	player[0]=p->x;
	player[1]=p->y;
	nowbox=p->now;	//还原游戏初始状态
	while(1)
	{
		Sleep(80);
		if(GetAsyncKeyState(VK_ESCAPE))
		{
			system("cls");
			Pos(34,14);
			printf("欢迎下次再玩。");
			Sleep(1500);
			exit(0);
		}
		if(GetAsyncKeyState(VK_F2))
		{
			regame=1;
			break;
		}
		else if(GetAsyncKeyState(VK_UP))
		{
			status=U;
		}
		else if(GetAsyncKeyState(VK_DOWN))
		{
			status=D;
		}
		else if(GetAsyncKeyState(VK_LEFT))
		{
			status=L;
		}
		else if(GetAsyncKeyState(VK_RIGHT))
		{
			status=R;
		}
		if(nowbox==p->all)	//完成此局
			break;
		move();
	}
}
void copybox(int copy[9][2],int b[9][2])	//复制箱子信息
{
	int i;
	for(i=0;i<9;i++)
	{
		copy[i][0]=b[i][0];
		copy[i][1]=b[i][1];
	}
}
void move()		//玩家移动
{
	int i,j,k,con;//i用于遍历,j保存要移动的箱子编号,k标记重叠箱子
	//con：0下一步有墙重合，1下一步没有墙，2下一步没有墙没有箱子
	//，3下一步有箱子，4箱子下一步没有墙，5箱子下一步有墙
	//，6箱子下一步没有墙没有箱子，7箱子下一步有其他箱子
	if(status!=0)//status不等于0执行移动
	{
		trans(nextp,player);//更新下一步
		for(i=1,con=1;p->wall[i][0]!=0||p->wall[i][1]!=0;i++)//判断玩家下一步是否与墙重合
		{
			if(p->wall[i][0]==nextp[0]&&p->wall[i][1]==nextp[1])
			{
				con=0;//重合使con为0
				break;
			}
		}
		if(con==1)//1下一步没有墙
		{
			for(i=0,con=2;boxc[i][0]!=0||boxc[i][1]!=0;i++)//判断下一步是否有箱子
			{
				if(boxc[i][0]==nextp[0]&&boxc[i][1]==nextp[1])
				{
					con=3;//下一步有箱子使con为3
					j=i;//保存要移动的箱子
					break;
				}
			}
			if(con==3)//3下一步有箱子
			{
				trans(nextb,nextp);//更新盒子下一步
				for(i=1,con=4;p->wall[i][0]!=0||p->wall[i][1]!=0;i++)//判断箱子下一步是否与墙重合
				{
					if(p->wall[i][0]==nextb[0]&&p->wall[i][1]==nextb[1])
					{
						con=5;//箱子下一步与墙重合使con为5
						break;
					}
				}
				if(con==4)//4箱子下一步没有墙
				{
					for(i=0,con=6;boxc[i][0]!=0||boxc[i][1]!=0;i++)//判断箱子下一步是否有其他箱子
					{
						if(boxc[i][0]==nextb[0]&&boxc[i][1]==nextb[1])
						{
							con=7;//箱子下一步有其他箱子使con为7
							break;
						}
					}
					if(con==6)//6箱子下一步没有墙并且也没有其他箱子
					{
						Pos(2*(nextb[0]+ofx),nextb[1]+ofy);
						printf("◇");
						Pos(2*(nextp[0]+ofx),nextp[1]+ofy);
						printf(":)");
						Pos(2*(player[0]+ofx),player[1]+ofy);
						printf("  ");
						for(i=0;p->des[i][0]!=0||p->des[i][1]!=0;i++)//当前玩家位置是否有终点标记
						{
							if(p->des[i][0]==player[0]&&p->des[i][1]==player[1])
							{
								Pos(2*(player[0]+ofx),player[1]+ofy);
								printf("×");
								break;
							}
						}
						for(i=0;p->des[i][0]!=0||p->des[i][1];i++)//判断要移动的箱子是否在终点上
						{
							if(p->des[i][0]==boxc[j][0]&&p->des[i][1]==boxc[j][1])
							{
								nowbox--;	//当前到位的箱子数减一
								for(k=0;k<9;k++)	//从箱子重叠表中删除离开终点的箱子
								{
									if(overlapc[k][0]==p->des[i][0]&&overlapc[k][1]==p->des[i][1])
									{
										overlapc[k][0]=0;
										overlapc[k][1]=0;
										break;
									}
								}
								break;
							}
						}
						for(i=0;p->des[i][0]!=0||p->des[i][1];i++)//判断下一个箱子是否在终点上
						{
							if(p->des[i][0]==nextb[0]&&p->des[i][1]==nextb[1])
							{
								nowbox++;//当前到位的箱子数减加一
								for(k=0;k<9;k++)	//从箱子重叠表中加入移入终点的箱子
								{
									if(overlapc[k][0]==0&&overlapc[k][1]==0)
									{
										overlapc[k][0]=p->des[i][0];
										overlapc[k][1]=p->des[i][1];
										break;
									}
								}
								break;
							}
						}
						printfoverlap(overlapc);
						Pos(79,29);
						boxc[j][0]=nextb[0];
						boxc[j][1]=nextb[1];
						player[0]=nextp[0];
						player[1]=nextp[1];
					}
				}
			}
			else if(con==2)//2下一步没有墙没有箱子
			{
				Pos(2*(nextp[0]+ofx),nextp[1]+ofy);
				printf(":)");
				Pos(2*(player[0]+ofx),player[1]+ofy);
				printf("  ");
				for(i=0;p->des[i][0]!=0||p->des[i][1]!=0;i++)//当前玩家位置是否有终点标记
				{
					if(p->des[i][0]==player[0]&&p->des[i][1]==player[1])
					{
						Pos(2*(player[0]+ofx),player[1]+ofy);
						printf("×");
						break;
					}
				}
				Pos(79,29);
				player[0]=nextp[0];
				player[1]=nextp[1];
			}
		}
		status=0;
	}
}
void trans(int p[2],int q[2])		//状态传递
{
	if(status==U)
	{
		p[0]=q[0];
		p[1]=q[1]-1;
	}
	else if(status==D)
	{
		p[0]=q[0];
		p[1]=q[1]+1;
	}
	else if(status==L)
	{
		p[0]=q[0]-1;
		p[1]=q[1];
	}
	else if(status==R)
	{
		p[0]=q[0]+1;
		p[1]=q[1];
	}
}
void initgame()		//初始化游戏
{//[0]是玩家起始位置，[1]是箱子个数和到位个数，
	//之后是重叠箱子位置，终点位置，箱子位置，墙位置
	int a0[60][2]={{4,4},{4,0},{1,4},{3,1},{4,6},{6,3},{3,3},{3,4},{4,5},{5,3},
	{0,3},{0,4},{0,5},{1,3},{1,5},{2,0},{2,1},{2,2},{2,3},{2,5},
	{3,0},{3,5},{3,6},{3,7},{4,0},{4,1},{4,2},{4,7},
	{5,2},{5,4},{5,5},{5,6},{5,7},{6,2},{6,4},{7,2},{7,3},{7,4},{999,999}},
	a1[60][2]={{1,1},{3,0},{7,3},{7,4},{7,5},{2,2},{2,3},{3,2},
	{0,0},{0,1},{0,2},{0,3},{0,4},{1,0},{1,4},{1,5},{1,6},{1,7},{1,8},
	{2,0},{2,4},{2,5},{2,8},{3,0},{3,8},{4,0},{4,1},{4,2},{4,3},{4,4},{4,8},
	{5,4},{5,6},{5,7},{5,8},{6,2},{6,3},{6,4},{6,7},{7,2},{7,7},
	{8,2},{8,3},{8,4},{8,5},{8,6},{8,7},{999,999}},
	a2[60][2]={{2,3},{4,0},{2,4},{2,5},{3,4},{3,5},{2,2},{4,3},{6,4},{7,3},
	{0,2},{0,3},{0,4},{0,5},{1,0},{1,1},{1,2},{1,5},{1,6},{2,0},{2,6},
	{3,0},{3,2},{3,6},{4,0},{4,2},{4,4},{4,5},{4,6},{5,0},{5,2},{5,6},
	{6,0},{6,6},{7,0},{7,1},{7,6},{8,1},{8,4},{8,5},{8,6},
	{9,1},{9,2},{9,3},{9,4},{999,999}},
	a3[60][2]={{1,2},{5,1},{3,6},{1,5},{1,6},{2,6},{3,6},{4,6},
	{2,2},{2,3},{2,5},{3,4},{3,6},{0,1,},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},
	{1,0},{1,1},{1,3},{1,4},{1,7},{2,0},{2,7},{3,0},{3,7},
	{4,0},{4,1},{4,2},{4,3},{4,7},{5,3},{5,4},{5,5},{5,6},{5,7},{999,999}};
	Group=Pass;
	p=Group+0;
	inputmap(a0);
	p=Group+1;
	inputmap(a1);
	p=Group+2;
	inputmap(a2);
	p=Group+3;
	inputmap(a3);
}
void inputmap(int a[][2])	//输入地图数据
{
	int i,k;
	p->x=a[0][0];
	p->y=a[0][1];		//玩家坐标
	p->all=a[1][0];
	p->now=a[1][1];		//箱子状态
	for(k=0,i=2;i<2+p->now;i++,k++)		//输入重叠箱子信息
	{
		p->overlap[k][0]=a[i][0];
		p->overlap[k][1]=a[i][1];
	}
	for(k=0;i<2+p->now+p->all;i++,k++)	//输入终点信息
	{
		p->des[k][0]=a[i][0];
		p->des[k][1]=a[i][1];
	}
	for(k=0;i<2+p->now+2*p->all;i++,k++)		//输入箱子信息
	{
		p->box[k][0]=a[i][0];
		p->box[k][1]=a[i][1];
	}
	for(k=0;a[i][0]!=999;i++,k++)		//输入墙的信息
	{
		p->wall[k][0]=a[i][0];
		p->wall[k][1]=a[i][1];
	}
}