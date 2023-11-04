#pragma comment(lib,"msimg32.lib")

#include <windows.h>
#include <math.h>
#include <time.h>

// Բ����
#define PI 3.1415

// ��ͼ����
#define MAP_WIDTH 20
#define MAP_HEIGHT 20

// ��λ��������߳�
#define UNIT_CUBE_SIZE 75

// ǽ����ɫ
#define WALL_RED 1
#define WALL_GREEN 2
#define WALL_BLUE 3

// ��λ�ƶ��ٶ�
#define UNIT_SPEED 1.0

// ��������
#define BOX_NUM 3

// ������ɫ
#define BOX_RED 4
#define BOX_GREEN 5
#define BOX_BLUE 6

// ��ͼ����������Ϊǽ�ڣ��߳�Ϊ 100 �����ִ���ǽ����ɫ���ϱ����������Ҷ�
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

// ��Ļ����
INT cxClient, cyClient;

// �豸����
HDC hdc, hdcMem, hdcHand;

// ���ƽṹ
PAINTSTRUCT ps;

// λͼ���
HBITMAP hBitmap, hBmHand;

// λͼ
BITMAP bmHand;

// ���ھ���
RECT rt;

// ������꣬ԭ��λ�ڵ�ͼ���½ǣ� x �᷽��Ϊ������ y �᷽��Ϊ����
DOUBLE posX;
DOUBLE posY;

// ����ӽǷ�������
DOUBLE dirX;
DOUBLE dirY;

// ����ƶ��ٶ�
DOUBLE movSpeed = 10.0 * UNIT_SPEED;

// ����ӽ���ת�ٶȣ�������
DOUBLE rotSpeed = PI / 45;

// ���ƽ�淽������
DOUBLE planeX;
DOUBLE planeY;

// ���� x ����ͶӰ�����ƽ��� x ����
DOUBLE cameraX;

// ���߷���
DOUBLE rayDirX;
DOUBLE rayDirY;

// ���߳���
DOUBLE rayLength;

// �������Ͷ�䵽����ľ���
DOUBLE rayProjection;

// ��Ҫ���Ƶ�ǽ�ڵĸ߶�
INT wallHeight;

// ǽ����ɫ
INT wallColor;

// ����
struct BOX
{
	// �������ڵĵ�ͼ�±�
	INT mapX = 0;
	INT mapY = 0;

	// �Ƿ񵽴�Ŀ��
	BOOL goal = FALSE;
};
BOX box[BOX_NUM];
BOX boxTarget[BOX_NUM]; // ������Ŀ�ĵ�

// ����
HPEN hPen, hPenR, hPenG, hPenB, hPenBox[BOX_NUM], hPenBoxTarget;

// ��ˢ
HBRUSH hBr;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);					   // ��Ϣ����
VOID SetWndclass(HINSTANCE hInstance, WNDCLASS& wndclass);                 // ��ƴ�����
VOID DataInit();                                                           // ���ݳ�ʼ��
VOID SetDC(HWND hwnd);                                                     // �����豸����
VOID Render();                                                             // ��Ϸ������Ⱦ
VOID PlayerMove();                                                         // ����ƶ�
VOID RotateView();                                                         // �ӽ���ת
VOID BoxMove();                                                            // �����ƶ�
VOID Destroy(HWND hwnd);                                                   // ��Դ����
DOUBLE CountRayLength(DOUBLE posx, DOUBLE posy, DOUBLE dirx, DOUBLE diry); // ��ĳ�㷢�����ߵ������ϰ���ľ���

// �������
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	// ��ƴ�����
	SetWndclass(hInstance, wndclass);

	// ע�ᴰ��
	RegisterClass(&wndclass);

	// ��������
	hwnd = CreateWindow(TEXT("MyWin"),
		TEXT("2240622426 ��˼��"),
		NULL,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	// ��ʾʱ�������
	ShowCursor(FALSE);

	// ��ʾ���ڲ�����
	ShowWindow(hwnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hwnd);

	// ��Ϣѭ��
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

// ��Ϣ����
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		DataInit();                                                                         // ���ݳ�ʼ��

		SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2); // �����λ����Ļ����
		SetTimer(hwnd, 1, 32, NULL);														// ��ʱ��
		return 0;
	}

	case WM_SIZE:
	{
		// ���ڳ���
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

		// �������λ����Ļ����
		SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
		return 0;
	}

    case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE) // ESC���˳�����
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

// ��ƴ�����
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
	// ��ҳ�ʼλ�ã��������������ƽ��������ֱ�ҳ���Ϊ 1
	posX = 850.0;
	posY = 850.0;
	dirX = 0.0;
	dirY = 1.0;
	planeX = 1.0;
	planeY = 0.0;

    // ����λ�����
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

	hPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200)); // �ұ�
	hPenR = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));    // ���
	hPenG = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));    // �̱�
	hPenB = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));    // ����
	hBr = CreateSolidBrush(RGB(194, 252, 254));		   // ����ˢ

	// ��ͬ������ɫ����ʵ��
	hPenBox[0] = CreatePen(PS_DASH, 1, RGB(255, 0, 0));
	hPenBox[1] = CreatePen(PS_DASH, 1, RGB(0, 255, 0));
	hPenBox[2] = CreatePen(PS_DASH, 1, RGB(0, 0, 255));
	hPenBoxTarget = CreatePen(PS_DASH, 1, RGB(240, 240, 240));

	// λͼ��Դ
	hBmHand = (HBITMAP)LoadImage(NULL, TEXT("hand.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(hBmHand, sizeof(BITMAP), &bmHand);
}

// �����豸����
VOID SetDC(HWND hwnd)
{
	// ˫����
	hdc = GetDC(hwnd);
	hdcMem = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, rt.right - rt.left, rt.bottom - rt.top);
	SelectObject(hdcMem, hBitmap);
	SelectObject(hdcMem, hPen);
	SelectObject(hdcMem, hBr);

	// λͼDC
	hdcHand = CreateCompatibleDC(hdcMem);
	SelectObject(hdcHand, hBmHand);
}

// ��Ϸ������Ⱦ
VOID Render()
{
	// ����
	FillRect(hdcMem, &rt, hBr);

	// ���ӵ���Ŀ�������ʵ��
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
		// �Ȼ�ǽ��
		if (i == 0)
		{
			for (INT boxCount = 0; boxCount < BOX_NUM; boxCount++)
			{
				map[boxTarget[boxCount].mapY][boxTarget[boxCount].mapX] = 0;
				map[box[boxCount].mapY][box[boxCount].mapX] = 0;
			}
		}
		// �ٻ�����
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
			// ���� x ������Ͷ�䵽���ƽ��� x �����꣬��ΧΪ -1 �� 1
			cameraX = 2.0 * screenX / (DOUBLE)cxClient - 1.0;

			// ������߷���
			rayDirX = dirX + planeX * cameraX;
			rayDirY = dirY + planeY * cameraX;

			// ������߳���
			rayLength = CountRayLength(posX, posY, rayDirX, rayDirY);

			// ����������Ͷ�䵽����ľ���
			rayProjection = 1.0 / sqrt(pow(rayDirX, 2) + pow(rayDirY, 2)) * rayLength;

			// ������Ҫ���Ƶ�ǽ�ڵĸ߶�
			wallHeight = (INT)((DOUBLE)cyClient * UNIT_CUBE_SIZE * 2.0 / rayProjection);

			// ѡ�񻭱�
			switch (wallColor)
			{
			// ��ǽ��
			case WALL_RED:
				SelectObject(hdcMem, hPenR);
				break;

			case WALL_GREEN:
				SelectObject(hdcMem, hPenG);
				break;

			case WALL_BLUE:
				SelectObject(hdcMem, hPenB);
				break;

			// ������
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

			// ���ذ�
			SelectObject(hdcMem, hPen);
			MoveToEx(hdcMem, screenX, (cyClient + wallHeight) / 2, NULL);
			LineTo(hdcMem, screenX, cyClient);
		}
	}

 ;
	TextOut(hdcMem, 0, 0, TEXT("WSAD�����ƶ����������ӽǣ���������������ӣ�������ָ����ɫ����"), 34);

	// ˫��
	TransparentBlt(hdcMem, (cxClient - bmHand.bmWidth) / 2, cyClient / 2, bmHand.bmWidth, bmHand.bmHeight,
		hdcHand, 0, 0, bmHand.bmWidth, bmHand.bmHeight, GetPixel(hdcHand, 0, 0));
	
	// һ�����������
	BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
}

// ����ƶ�
VOID PlayerMove()
{
	if (GetAsyncKeyState('W') & 0x8000) // ��ǰ�ƶ�
	{
		if (CountRayLength(posX + dirX * movSpeed, posY + dirY * movSpeed, dirX, dirY) > UNIT_CUBE_SIZE) // ǰ���Ƿ����ϰ���
		{
			// �����������
			posX += dirX * movSpeed;
			posY += dirY * movSpeed;

			Render();
		}
		else if (wallColor == BOX_RED || wallColor == BOX_GREEN || wallColor == BOX_BLUE) // ǰ���Ƿ�������
		{
			// �����ƶ�
			BoxMove();
			Render();
		}
	}
	else if (GetAsyncKeyState('S') & 0x8000) // ����ƶ�
	{
		if (CountRayLength(posX - dirX * movSpeed, posY - dirY * movSpeed, -1.0 * dirX, -1.0 * dirY) > UNIT_CUBE_SIZE)
		{
			posX -= dirX * movSpeed;
			posY -= dirY * movSpeed;

			Render();
		}
	}
	else if (GetAsyncKeyState('A') & 0x8000) // �����ƶ�
	{
		// ���㷽����������ֱ����
		DOUBLE dirx = dirX, diry = dirY;
		DOUBLE oldDirx = dirx;
		dirx = dirx * cos(PI / 2.0) - diry * sin(PI / 2.0);
		diry = oldDirx * sin(PI / 2.0) + diry * cos(PI / 2.0);

		if (CountRayLength(posX + dirx * movSpeed, posY + diry * movSpeed, dirx, diry) > UNIT_CUBE_SIZE)
		{
			// �����������
			posX += dirx * movSpeed;
			posY += diry * movSpeed;

			Render();
		}
	}
	else if (GetAsyncKeyState('D') & 0x8000) // �����ƶ�
	{
		// ���㷽����������ֱ����
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

// �ӽ���ת
VOID RotateView()
{
	POINT curPos, preCurPos = { GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2 };
	GetCursorPos(&curPos);

	if (curPos.x < preCurPos.x) // ������ת�ӽ�
	{
		// ���㷽���������������������ʼ�մ�ֱ�ҳ��Ȳ���
		DOUBLE oldDirX = dirX;
		dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
		dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
		DOUBLE oldPlaneX = planeX;
		planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
		planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);

		Render();
	}
	else if (curPos.x > preCurPos.x) // ������ת�ӽ�
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

// �����ƶ�
VOID BoxMove()
{
	INT boxMapX = box[0].mapX, boxMapY = box[0].mapX, playerMapX, playerMapY;
	INT boxCount = 0;

	// ��¼���ӵĵ�ͼ�±�
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

	// ��¼��ҵĵ�ͼ�±�
	playerMapX = (INT)(posX / 100) + 1;
	playerMapY = MAP_HEIGHT - (INT)(posY / 100) - 2;

	// �����ƶ�
	if (boxMapX == playerMapX) // �����Ӻ������ͬ�� y ��ʱ
	{
		if (playerMapY < boxMapY) // �Ƚ����� y ������ȷ���ƶ�����
		{
			if (map[boxMapY + 1][boxMapX] == 0) // �����ƶ�λ���Ƿ����ϰ���
			{
				// ����λ��
				box[boxCount].mapY += 1;
				map[boxMapY][boxMapX] = 0;
				map[boxMapY + 1][boxMapX] = BOX_RED + boxCount;
			}
			else if (map[boxMapY + 1][boxMapX] == BOX_RED + BOX_NUM) // ���ӵ���Ŀ��
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

// ��Դ����
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

// ��ĳ�㷢�����ߵ������ϰ���ľ���
DOUBLE CountRayLength(DOUBLE posx, DOUBLE posy, DOUBLE dirx, DOUBLE diry)
{
	// ��������
	DOUBLE rayX = posx;
	DOUBLE rayY = posy;

	// ����λ��
	DOUBLE disX = 0.0;
	DOUBLE disY = 0.0;

	// ���������Ӧ�ĵ�ͼ�±�
	INT rayMapX;
	INT rayMapY;

	// ��������������
	do
	{
		// ������ߵ���һ�����������λ��
		if (dirx > 0)
			disX = (INT)(rayX / 100 + 1) * 100.0 - rayX;
		else if (dirx < 0)
			disX = (INT)((rayX - 1) / 100) * 100.0 - rayX;
		if (diry > 0)
			disY = (INT)(rayY / 100 + 1) * 100.0 - rayY;
		else if (diry < 0)
			disY = (INT)((rayY - 1) / 100) * 100.0 - rayY;

		// ������߷�����ٶ�����
		DOUBLE velX = fabs(dirx) / sqrt(pow(dirx, 2) + pow(diry, 2));
		DOUBLE velY = fabs(diry) / sqrt(pow(dirx, 2) + pow(diry, 2));

		// ���ߴ�������һ�����������ʱ������
		DOUBLE timeX = velX != 0 ? fabs(disX) / velX : DOUBLE(1.79769111186211570811527121711701157e+108L);
		DOUBLE timeY = velY != 0 ? fabs(disY) / velY : DOUBLE(1.79769111186211570811527121711701157e+108L);

		// �жϹ��߻��з��������λ�ã������ϱ����Ϊtrue�� ���ж������Ϊfalse
		BOOL side = timeX > timeY ? true : false;

		// �����������
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

		// ������������Ӧ�ĵ�ͼ�±�
		rayMapX = dirx != 0 ? floor((rayX + dirx * fabs(0.1 / dirx)) / 100) + 1 : (INT)(rayX / 100) + 1.0;
		rayMapY = diry != 0 ? MAP_HEIGHT - floor((rayY + diry * fabs(0.1 / diry)) / 100) - 2 : (DOUBLE)MAP_HEIGHT - (INT)(rayY / 100) - 2.0;
	} while (!map[rayMapY][rayMapX]); // �Ƿ������ϰ���

	// ����ǽ����ɫ
	wallColor = map[rayMapY][rayMapX];

	return sqrt(pow(posx - rayX, 2) + pow(posy - rayY, 2)); // ���߳�ʼλ�õ����ĳ���
}