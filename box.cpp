#pragma comment(lib,"msimg32.lib")

#include <windows.h>
#include <math.h>
#include <time.h>

// 圆周率
#define PI 3.1415

// 地图长宽
#define MAP_WIDTH 20
#define MAP_HEIGHT 20

// 单位方形区域边长
#define UNIT_CUBE_SIZE 75

// 墙壁颜色
#define WALL_RED 1
#define WALL_GREEN 2
#define WALL_BLUE 3

// 单位移动速度
#define UNIT_SPEED 1.0

// 箱子数量
#define BOX_NUM 3

// 箱子颜色
#define BOX_RED 4
#define BOX_GREEN 5
#define BOX_BLUE 6

// 地图，非零区域为墙壁，边长为 100 ，数字代表墙壁颜色，上北下南左西右东
INT map[MAP_HEIGHT][MAP_WIDTH] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
								  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
								  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
								  {2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2},
								  {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
								  {2, 0, 0, 1, 1, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
								  {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 2},
								  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2},
								  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2},
								  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 2},
								  {2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
								  {2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2},
								  {2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2},
								  {2, 0, 0, 2, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2},
								  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
								  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
								  {2, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 2},
								  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
								  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
								  {2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3} };

// 屏幕长宽
INT cxClient, cyClient;

// 设备环境
HDC hdc, hdcMem, hdcHand;

// 绘制结构
PAINTSTRUCT ps;

// 位图句柄
HBITMAP hBitmap, hBmHand;

// 位图
BITMAP bmHand;

// 窗口矩形
RECT rt;

// 玩家坐标，原点位于地图左下角， x 轴方向为正北， y 轴方向为正东
DOUBLE posX;
DOUBLE posY;

// 玩家视角方向向量
DOUBLE dirX;
DOUBLE dirY;

// 玩家移动速度
DOUBLE movSpeed = 10.0 * UNIT_SPEED;

// 玩家视角旋转速度，弧度制
DOUBLE rotSpeed = PI / 45;

// 相机平面方向向量
DOUBLE planeX;
DOUBLE planeY;

// 窗口 x 坐标投影到相机平面的 x 坐标
DOUBLE cameraX;

// 光线方向
DOUBLE rayDirX;
DOUBLE rayDirY;

// 光线长度
DOUBLE rayLength;

// 光线落点投射到相机的距离
DOUBLE rayProjection;

// 需要绘制的墙壁的高度
INT wallHeight;

// 墙壁颜色
INT wallColor;

// 箱子
struct BOX
{
	// 箱子所在的地图下标
	INT mapX = 0;
	INT mapY = 0;

	// 是否到达目标
	BOOL goal = FALSE;
};
BOX box[BOX_NUM];
BOX boxTarget[BOX_NUM]; // 推箱子目的地

// 画笔
HPEN hPen, hPenR, hPenG, hPenB, hPenBox[BOX_NUM], hPenBoxTarget;

// 画刷
HBRUSH hBr;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);					   // 消息处理
VOID SetWndclass(HINSTANCE hInstance, WNDCLASS& wndclass);                 // 设计窗口类
VOID DataInit();                                                           // 数据初始化
VOID SetDC(HWND hwnd);                                                     // 设置设备环境
VOID Render();                                                             // 游戏画面渲染
VOID PlayerMove();                                                         // 玩家移动
VOID RotateView();                                                         // 视角旋转
VOID BoxMove();                                                            // 箱子移动
VOID Destroy(HWND hwnd);                                                   // 资源销毁
DOUBLE CountRayLength(DOUBLE posx, DOUBLE posy, DOUBLE dirx, DOUBLE diry); // 求某点发出射线到击中障碍物的距离

// 程序入口
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	// 设计窗口类
	SetWndclass(hInstance, wndclass);

	// 注册窗口
	RegisterClass(&wndclass);

	// 创建窗口
	hwnd = CreateWindow(TEXT("MyWin"),
		TEXT("2240622426 徐思哲"),
		NULL,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	// 显示时隐藏鼠标
	ShowCursor(FALSE);

	// 显示窗口并更新
	ShowWindow(hwnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hwnd);

	// 消息循环
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

// 消息处理
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		DataInit();                                                                         // 数据初始化

		SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2); // 鼠标光标位于屏幕中心
		SetTimer(hwnd, 1, 32, NULL);														// 计时器
		return 0;
	}

	case WM_SIZE:
	{
		// 窗口长宽
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		GetClientRect(hwnd, &rt);

		SetDC(hwnd);
		return 0;
	}

	case WM_PAINT:
	{
		HDC hdcPaint = BeginPaint(hwnd, &ps);
		Render();
		EndPaint(hwnd, &ps);
        return 0;
    }

	case WM_TIMER:
	{
		PlayerMove();
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		RotateView();

		// 重置鼠标位于屏幕中心
		SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
		return 0;
	}

    case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE) // ESC键退出程序
		{
			KillTimer(hwnd, 1);
			Destroy(hwnd);
			PostQuitMessage(0);
		}
		return 0;
	}

	case WM_DESTROY:
	{
		KillTimer(hwnd, 1);
		Destroy(hwnd);
		PostQuitMessage(0);
		return 0;
	}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

// 设计窗口类
VOID SetWndclass(HINSTANCE hInstance, WNDCLASS& wndclass)
{
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("MyWin");
}

VOID DataInit()
{
	// 玩家初始位置，方向向量和相机平面向量垂直且长度为 1
	posX = 850.0;
	posY = 850.0;
	dirX = 0.0;
	dirY = 1.0;
	planeX = 1.0;
	planeY = 0.0;

    // 箱子位置随机
	srand((unsigned int)time(NULL));
	for (INT boxCount = 0; boxCount < BOX_NUM; boxCount++)
	{
		while (map[box[boxCount].mapY][box[boxCount].mapX] != 0
			|| (box[boxCount].mapX == (INT)(posX / 100) + 1 && box[boxCount].mapY == MAP_HEIGHT - (INT)(posY / 100) - 2))
		{
			box[boxCount].mapX = rand() % (MAP_WIDTH - 2) + 1;
			box[boxCount].mapY = rand() % (MAP_HEIGHT - 2) + 1;
		}
		map[box[boxCount].mapY][box[boxCount].mapX] = BOX_RED + boxCount;

		while (map[boxTarget[boxCount].mapY][boxTarget[boxCount].mapX] != 0
			|| (boxTarget[boxCount].mapX == (INT)(posX / 100) + 1 && boxTarget[boxCount].mapY == MAP_HEIGHT - (INT)(posY / 100) - 2))
		{
			boxTarget[boxCount].mapX = rand() % (MAP_WIDTH - 2) + 1;
			boxTarget[boxCount].mapY = rand() % (MAP_HEIGHT - 2) + 1;
		}
		map[boxTarget[boxCount].mapY][boxTarget[boxCount].mapX] = BOX_RED + BOX_NUM;
	}

	hPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200)); // 灰笔
	hPenR = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));    // 红笔
	hPenG = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));    // 绿笔
	hPenB = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));    // 蓝笔
	hBr = CreateSolidBrush(RGB(194, 252, 254));		   // 淡蓝刷

	// 不同箱子颜色，非实心
	hPenBox[0] = CreatePen(PS_DASH, 1, RGB(255, 0, 0));
	hPenBox[1] = CreatePen(PS_DASH, 1, RGB(0, 255, 0));
	hPenBox[2] = CreatePen(PS_DASH, 1, RGB(0, 0, 255));
	hPenBoxTarget = CreatePen(PS_DASH, 1, RGB(240, 240, 240));

	// 位图资源
	hBmHand = (HBITMAP)LoadImage(NULL, TEXT("hand.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(hBmHand, sizeof(BITMAP), &bmHand);
}

// 设置设备环境
VOID SetDC(HWND hwnd)
{
	// 双缓冲
	hdc = GetDC(hwnd);
	hdcMem = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, rt.right - rt.left, rt.bottom - rt.top);
	SelectObject(hdcMem, hBitmap);
	SelectObject(hdcMem, hPen);
	SelectObject(hdcMem, hBr);

	// 位图DC
	hdcHand = CreateCompatibleDC(hdcMem);
	SelectObject(hdcHand, hBmHand);
}

// 游戏画面渲染
VOID Render()
{
	// 背景
	FillRect(hdcMem, &rt, hBr);

	// 箱子到达目标则变做实心
	if (box[0].goal)
	{
		hPenBox[0] = hPenR;
	}
	if (box[1].goal)
	{
		hPenBox[1] = hPenG;
	}
	if (box[2].goal)
	{
		hPenBox[2] = hPenB;
	}
	
	for (INT i = 0; i < 2; i++)
	{
		// 先画墙壁
		if (i == 0)
		{
			for (INT boxCount = 0; boxCount < BOX_NUM; boxCount++)
			{
				map[boxTarget[boxCount].mapY][boxTarget[boxCount].mapX] = 0;
				map[box[boxCount].mapY][box[boxCount].mapX] = 0;
			}
		}
		// 再画箱子
		if (i == 1)
		{
			for (INT boxCount = 0; boxCount < BOX_NUM; boxCount++)
			{
				if (boxTarget[boxCount].mapY == box[boxCount].mapY && boxTarget[boxCount].mapX == box[boxCount].mapX)
				{
					map[boxTarget[boxCount].mapY][boxTarget[boxCount].mapX] = 0;
					map[box[boxCount].mapY][box[boxCount].mapX] = BOX_RED + boxCount;
				}
				else
				{
					map[boxTarget[boxCount].mapY][boxTarget[boxCount].mapX] = BOX_RED + BOX_NUM;
					map[box[boxCount].mapY][box[boxCount].mapX] = BOX_RED + boxCount;
				}
			}
		}

		for (INT screenX = 0; screenX < cxClient; screenX++)
		{
			// 窗口 x 轴坐标投射到相机平面的 x 轴坐标，范围为 -1 到 1
			cameraX = 2.0 * screenX / (DOUBLE)cxClient - 1.0;

			// 计算光线方向
			rayDirX = dirX + planeX * cameraX;
			rayDirY = dirY + planeY * cameraX;

			// 计算光线长度
			rayLength = CountRayLength(posX, posY, rayDirX, rayDirY);

			// 计算光线落点投射到相机的距离
			rayProjection = 1.0 / sqrt(pow(rayDirX, 2) + pow(rayDirY, 2)) * rayLength;

			// 计算需要绘制的墙壁的高度
			wallHeight = (INT)((DOUBLE)cyClient * UNIT_CUBE_SIZE * 2.0 / rayProjection);

			// 选择画笔
			switch (wallColor)
			{
			// 画墙壁
			case WALL_RED:
				SelectObject(hdcMem, hPenR);
				break;

			case WALL_GREEN:
				SelectObject(hdcMem, hPenG);
				break;

			case WALL_BLUE:
				SelectObject(hdcMem, hPenB);
				break;

			// 画箱子
			case BOX_RED:
				SelectObject(hdcMem, hPenBox[0]);
				MoveToEx(hdcMem, screenX, cyClient / 2, NULL);
				LineTo(hdcMem, screenX, (cyClient + wallHeight) / 2);

				SelectObject(hdcMem, hPen);
				MoveToEx(hdcMem, screenX, (cyClient + wallHeight) / 2, NULL);
				LineTo(hdcMem, screenX, cyClient);
				continue;

			case BOX_GREEN:
				SelectObject(hdcMem, hPenBox[1]);
				MoveToEx(hdcMem, screenX, cyClient / 2, NULL);
				LineTo(hdcMem, screenX, (cyClient + wallHeight) / 2);

				SelectObject(hdcMem, hPen);
				MoveToEx(hdcMem, screenX, (cyClient + wallHeight) / 2, NULL);
				LineTo(hdcMem, screenX, cyClient);
				continue;

			case BOX_BLUE:
				SelectObject(hdcMem, hPenBox[2]);
				MoveToEx(hdcMem, screenX, cyClient / 2, NULL);
				LineTo(hdcMem, screenX, (cyClient + wallHeight) / 2);

				SelectObject(hdcMem, hPen);
				MoveToEx(hdcMem, screenX, (cyClient + wallHeight) / 2, NULL);
				LineTo(hdcMem, screenX, cyClient);
				continue;

			case BOX_RED + BOX_NUM:
				SelectObject(hdcMem, hPenBoxTarget);
				MoveToEx(hdcMem, screenX, cyClient / 2, NULL);
				LineTo(hdcMem, screenX, (cyClient + wallHeight) / 2);

				SelectObject(hdcMem, hPen);
				MoveToEx(hdcMem, screenX, (cyClient + wallHeight) / 2, NULL);
				LineTo(hdcMem, screenX, cyClient);
				continue;

			default:
				SelectObject(hdcMem, hPen);
				break;
			}

			MoveToEx(hdcMem, screenX, (cyClient - wallHeight) / 2, NULL);
			LineTo(hdcMem, screenX, (cyClient + wallHeight) / 2);

			// 画地板
			SelectObject(hdcMem, hPen);
			MoveToEx(hdcMem, screenX, (cyClient + wallHeight) / 2, NULL);
			LineTo(hdcMem, screenX, cyClient);
		}
	}

 ;
	TextOut(hdcMem, 0, 0, TEXT("WSAD控制移动，鼠标控制视角，随机生成三个箱子，可推至指定白色区域"), 34);

	// 双手
	TransparentBlt(hdcMem, (cxClient - bmHand.bmWidth) / 2, cyClient / 2, bmHand.bmWidth, bmHand.bmHeight,
		hdcHand, 0, 0, bmHand.bmWidth, bmHand.bmHeight, GetPixel(hdcHand, 0, 0));
	
	// 一次输出到窗口
	BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
}

// 玩家移动
VOID PlayerMove()
{
	if (GetAsyncKeyState('W') & 0x8000) // 向前移动
	{
		if (CountRayLength(posX + dirX * movSpeed, posY + dirY * movSpeed, dirX, dirY) > UNIT_CUBE_SIZE) // 前方是否有障碍物
		{
			// 计算玩家坐标
			posX += dirX * movSpeed;
			posY += dirY * movSpeed;

			Render();
		}
		else if (wallColor == BOX_RED || wallColor == BOX_GREEN || wallColor == BOX_BLUE) // 前方是否有箱子
		{
			// 箱子移动
			BoxMove();
			Render();
		}
	}
	else if (GetAsyncKeyState('S') & 0x8000) // 向后移动
	{
		if (CountRayLength(posX - dirX * movSpeed, posY - dirY * movSpeed, -1.0 * dirX, -1.0 * dirY) > UNIT_CUBE_SIZE)
		{
			posX -= dirX * movSpeed;
			posY -= dirY * movSpeed;

			Render();
		}
	}
	else if (GetAsyncKeyState('A') & 0x8000) // 向左移动
	{
		// 计算方向向量，垂直向左
		DOUBLE dirx = dirX, diry = dirY;
		DOUBLE oldDirx = dirx;
		dirx = dirx * cos(PI / 2.0) - diry * sin(PI / 2.0);
		diry = oldDirx * sin(PI / 2.0) + diry * cos(PI / 2.0);

		if (CountRayLength(posX + dirx * movSpeed, posY + diry * movSpeed, dirx, diry) > UNIT_CUBE_SIZE)
		{
			// 计算玩家坐标
			posX += dirx * movSpeed;
			posY += diry * movSpeed;

			Render();
		}
	}
	else if (GetAsyncKeyState('D') & 0x8000) // 向右移动
	{
		// 计算方向向量，垂直向右
		DOUBLE dirx = dirX, diry = dirY;
		DOUBLE oldDirx = dirx;
		dirx = dirx * cos(-PI / 2.0) - diry * sin(-PI / 2.0);
		diry = oldDirx * sin(-PI / 2.0) + diry * cos(-PI / 2.0);

		if (CountRayLength(posX + dirx * movSpeed, posY + diry * movSpeed, dirx, diry) > UNIT_CUBE_SIZE)
		{
			posX += dirx * movSpeed;
			posY += diry * movSpeed;

			Render();
		}
	}
}

// 视角旋转
VOID RotateView()
{
	POINT curPos, preCurPos = { GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2 };
	GetCursorPos(&curPos);

	if (curPos.x < preCurPos.x) // 向左旋转视角
	{
		// 计算方向向量和相机向量，两者始终垂直且长度不变
		DOUBLE oldDirX = dirX;
		dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
		dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
		DOUBLE oldPlaneX = planeX;
		planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
		planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);

		Render();
	}
	else if (curPos.x > preCurPos.x) // 向右旋转视角
	{
		DOUBLE oldDirX = dirX;
		dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
		dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
		DOUBLE oldPlaneX = planeX;
		planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
		planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);

		Render();
	}
}

// 箱子移动
VOID BoxMove()
{
	INT boxMapX = box[0].mapX, boxMapY = box[0].mapX, playerMapX, playerMapY;
	INT boxCount = 0;

	// 记录箱子的地图下标
	while(boxCount < BOX_NUM)
	{
		if (wallColor == BOX_RED + boxCount)
		{
			boxMapX = box[boxCount].mapX;
			boxMapY = box[boxCount].mapY;
			break;
		}
		boxCount++;
	}
	if (box[boxCount].goal)
	{
		return;
	}

	// 记录玩家的地图下标
	playerMapX = (INT)(posX / 100) + 1;
	playerMapY = MAP_HEIGHT - (INT)(posY / 100) - 2;

	// 箱子移动
	if (boxMapX == playerMapX) // 当箱子和玩家在同个 y 轴时
	{
		if (playerMapY < boxMapY) // 比较两者 y 坐标以确定移动方向
		{
			if (map[boxMapY + 1][boxMapX] == 0) // 箱子移动位置是否有障碍物
			{
				// 箱子位移
				box[boxCount].mapY += 1;
				map[boxMapY][boxMapX] = 0;
				map[boxMapY + 1][boxMapX] = BOX_RED + boxCount;
			}
			else if (map[boxMapY + 1][boxMapX] == BOX_RED + BOX_NUM) // 箱子到达目标
			{
				box[boxCount].mapY += 1;
				map[boxMapY][boxMapX] = 0;
				map[boxMapY + 1][boxMapX] = BOX_RED + boxCount;
				box[boxCount].goal = TRUE;
			}
		}
		else
		{
			if (map[boxMapY - 1][boxMapX] == 0)
			{
				box[boxCount].mapY -= 1;
				map[boxMapY][boxMapX] = 0;
				map[boxMapY - 1][boxMapX] = BOX_RED + boxCount;
			}
			else if (map[boxMapY - 1][boxMapX] == BOX_RED + BOX_NUM)
			{
				box[boxCount].mapY -= 1;
				map[boxMapY][boxMapX] = 0;
				map[boxMapY - 1][boxMapX] = BOX_RED + boxCount;
				box[boxCount].goal = TRUE;
			}
		}
	}
	else
	{
		if (playerMapX < boxMapX)
		{
			if (map[boxMapY][boxMapX + 1] == 0)
			{
				box[boxCount].mapX += 1;
				map[boxMapY][boxMapX] = 0;
				map[boxMapY][boxMapX + 1] = BOX_RED + boxCount;
			}
			else if (map[boxMapY][boxMapX + 1] == BOX_RED + BOX_NUM)
			{
				box[boxCount].mapX += 1;
				map[boxMapY][boxMapX] = 0;
				map[boxMapY][boxMapX + 1] = BOX_RED + boxCount;
				box[boxCount].goal = TRUE;
			}
		}
		else
		{
			if (map[boxMapY][boxMapX - 1] == 0)
			{
				box[boxCount].mapX -= 1;
				map[boxMapY][boxMapX] = 0;
				map[boxMapY][boxMapX - 1] = BOX_RED + boxCount;
			}
			else if (map[boxMapY][boxMapX - 1] == BOX_RED + BOX_NUM)
			{
				box[boxCount].mapX -= 1;
				map[boxMapY][boxMapX] = 0;
				map[boxMapY][boxMapX - 1] = BOX_RED + boxCount;
				box[boxCount].goal = TRUE;
			}
		}
	}
}

// 资源销毁
VOID Destroy(HWND hwnd)
{
	DeleteObject(hPen);
	DeleteObject(hPenR);
	DeleteObject(hPenG);
	DeleteObject(hPenB);
	for (int i = 0; i < BOX_NUM; i++)
	{
		DeleteObject(hPenBox[i]);
	}
	DeleteObject(hPenBoxTarget);
	DeleteObject(hBr);
	DeleteObject(hBmHand);
	DeleteObject(hBitmap);
	ReleaseDC(hwnd, hdcHand);
	ReleaseDC(hwnd, hdcMem);
	ReleaseDC(hwnd, hdc);
}

// 求某点发出射线到击中障碍物的距离
DOUBLE CountRayLength(DOUBLE posx, DOUBLE posy, DOUBLE dirx, DOUBLE diry)
{
	// 光线坐标
	DOUBLE rayX = posx;
	DOUBLE rayY = posy;

	// 光线位移
	DOUBLE disX = 0.0;
	DOUBLE disY = 0.0;

	// 光线坐标对应的地图下标
	INT rayMapX;
	INT rayMapY;

	// 计算光线落点坐标
	do
	{
		// 计算光线到下一个方形区域的位移
		if (dirx > 0)
			disX = (INT)(rayX / 100 + 1) * 100.0 - rayX;
		else if (dirx < 0)
			disX = (INT)((rayX - 1) / 100) * 100.0 - rayX;
		if (diry > 0)
			disY = (INT)(rayY / 100 + 1) * 100.0 - rayY;
		else if (diry < 0)
			disY = (INT)((rayY - 1) / 100) * 100.0 - rayY;

		// 计算光线发射的速度向量
		DOUBLE velX = fabs(dirx) / sqrt(pow(dirx, 2) + pow(diry, 2));
		DOUBLE velY = fabs(diry) / sqrt(pow(dirx, 2) + pow(diry, 2));

		// 光线传播到下一个方形区域的时间向量
		DOUBLE timeX = velX != 0 ? fabs(disX) / velX : DOUBLE(1.79769111186211570811527121711701157e+108L);
		DOUBLE timeY = velY != 0 ? fabs(disY) / velY : DOUBLE(1.79769111186211570811527121711701157e+108L);

		// 判断光线击中方形区域的位置，命中南北侧边为true， 命中东西侧边为false
		BOOL side = timeX > timeY ? true : false;

		// 计算光线坐标
		if (side)
		{
			rayX += dirx != 0 ? dirx * fabs(1.0 / dirx) * fabs(disY * fabs(dirx / diry)) : 0;
			rayY += disY;
		}
		else
		{
			rayX += disX;
			rayY += diry != 0 ? diry * fabs(1.0 / diry) * fabs(disX * fabs(diry / dirx)) : 0;
		}

		// 计算光线坐标对应的地图下标
		rayMapX = dirx != 0 ? floor((rayX + dirx * fabs(0.1 / dirx)) / 100) + 1 : (INT)(rayX / 100) + 1.0;
		rayMapY = diry != 0 ? MAP_HEIGHT - floor((rayY + diry * fabs(0.1 / diry)) / 100) - 2 : (DOUBLE)MAP_HEIGHT - (INT)(rayY / 100) - 2.0;
	} while (!map[rayMapY][rayMapX]); // 是否命中障碍物

	// 设置墙壁颜色
	wallColor = map[rayMapY][rayMapX];

	return sqrt(pow(posx - rayX, 2) + pow(posy - rayY, 2)); // 光线初始位置到落点的长度
}
