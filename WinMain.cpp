#include <windows.h>
#include <math.h>

// Բ����
#define PI 3.1415926

// ��ͼ����
#define MAP_WIDTH 20
#define MAP_HEIGHT 20

// ��ͼ����������Ϊǽ�ڣ��߳�Ϊ 100 �����ִ�����ɫ���ϱ����������Ҷ�
INT map[MAP_HEIGHT][MAP_WIDTH] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
                                  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 0, 0, 1, 0, 0, 2, 2, 2, 0, 0, 0, 3, 3, 3, 0, 0, 2},
                                  {2, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 1, 1, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 2},
                                  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 2, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 2},
                                  {2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 2, 0, 0, 1, 1, 1, 1, 0, 3, 3, 3, 3, 3, 0, 0, 0, 2},
                                  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}};

// ��λ��������ı߳�
#define UNIT_CUBE_SIZE 50.0

// ��λ�ƶ��ٶ�
#define UNIT_SPEED 1.0

// ������꣬ԭ��λ�ڵ�ͼ���½ǣ� x �᷽��Ϊ������ y �᷽��Ϊ����
DOUBLE posX;
DOUBLE posY;

// ����ӽǷ�������
DOUBLE dirX;
DOUBLE dirY;

// ����ƶ��ٶ�
DOUBLE movSpeed = 5.0 * UNIT_SPEED;

// ����ӽ���ת�ٶȣ�������
DOUBLE rotSpeed = PI / 180;

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

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);                      // ��Ϣ����
DOUBLE CountRayLength(DOUBLE posx, DOUBLE posy, DOUBLE dirx, DOUBLE diry); // ��ĳ�㷢�����ߵ������ϰ���ľ���

// �������
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   PSTR szCmdLine,
                   int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("MyWin");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    // ��ƴ���
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    // ע�ᴰ��
    RegisterClass(&wndclass);

    // ��������
    hwnd = CreateWindow(szAppName,
                        TEXT("MyDemo"),
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
    static int cxClient, cyClient;
    static HDC hdc, hdcMem;
    static PAINTSTRUCT ps;
    static HBITMAP hBitmap;
    static HPEN hPen, hPenR, hPenG, hPenB, hPenOld;
    static HBRUSH hBr;
    static RECT rt;

    switch (message)
    {
    case WM_CREATE:
    {
        // ���ݳ�ʼ�����������������ƽ��������ֱ�ҳ���Ϊ 1
        posX = 900.0;
        posY = 900.0;
        dirX = 0.0;
        dirY = 1.0;
        planeX = 1.0;
        planeY = 0.0;

        hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));    // �ڱ�
        hPenR = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // ���
        hPenG = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // �̱�
        hPenB = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); // ����
        hBr = CreateSolidBrush(RGB(255, 255, 255));     // ��ˢ

        SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2); // �����λ����Ļ����
        SetTimer(hwnd, 1, 20, NULL);                                                        // ��ʱ��
        return 0;
    }

    case WM_SIZE:
    {
        // ���ڳ���
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        GetClientRect(hwnd, &rt);

        // ˫����
        hdc = GetDC(hwnd);
        hdcMem = CreateCompatibleDC(hdc);
        hBitmap = CreateCompatibleBitmap(hdc, rt.right - rt.left, rt.bottom - rt.top);
        hPenOld = (HPEN)SelectObject(hdcMem, hPen);
        SelectObject(hdcMem, hBitmap);
        SelectObject(hdcMem, hBr);
        return 0;
    }

    case WM_PAINT:
    {
        HDC hdcPaint = BeginPaint(hwnd, &ps);
        HDC hdcMemPaint = CreateCompatibleDC(hdcPaint);
        HBITMAP hBitmapPaint = CreateCompatibleBitmap(hdcPaint, rt.right - rt.left, rt.bottom - rt.top);
        HPEN hPenOldPaint = (HPEN)SelectObject(hdcMemPaint, hPen);
        SelectObject(hdcMemPaint, hBitmapPaint);
        SelectObject(hdcMemPaint, hBr);

        FillRect(hdcMemPaint, &rt, hBr);

        // ��Ϸ����
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
            wallHeight = (INT)(cyClient * UNIT_CUBE_SIZE * 2.0 / rayProjection);

            // ѡ��ǽ����ɫ
            switch (wallColor)
            {
            case 1:
                SelectObject(hdcMemPaint, hPenR);
                break;

            case 2:
                SelectObject(hdcMemPaint, hPenG);
                break;

            case 3:
                SelectObject(hdcMemPaint, hPenB);
                break;

            default:
                SelectObject(hdcMemPaint, hPen);
                break;
            }

            // ��ǽ��
            MoveToEx(hdcMemPaint, screenX, (cyClient - wallHeight) / 2, NULL);
            LineTo(hdcMemPaint, screenX, (cyClient + wallHeight) / 2);
        }

        BitBlt(hdcPaint, 0, 0, cxClient, cyClient, hdcMemPaint, 0, 0, SRCCOPY);

        SelectObject(hdcMemPaint, hPenOldPaint);
        DeleteObject(hBitmapPaint);
        ReleaseDC(hwnd, hdcMemPaint);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
    {
        // ��Դ����
        KillTimer(hwnd, 1);
        SelectObject(hdcMem, hPenOld);
        DeleteObject(hPen);
        DeleteObject(hPenR);
        DeleteObject(hPenG);
        DeleteObject(hPenB);
        DeleteObject(hBr);
        DeleteObject(hPenOld);
        DeleteObject(hBitmap);
        ReleaseDC(hwnd, hdcMem);
        ReleaseDC(hwnd, hdc);
        PostQuitMessage(0);
        return 0;
    }

    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE) // ESC���˳�����
        {
            // ��Դ����
            KillTimer(hwnd, 1);
            SelectObject(hdcMem, hPenOld);
            DeleteObject(hPen);
            DeleteObject(hPenR);
            DeleteObject(hPenG);
            DeleteObject(hPenB);
            DeleteObject(hBr);
            DeleteObject(hPenOld);
            DeleteObject(hBitmap);
            ReleaseDC(hwnd, hdcMem);
            ReleaseDC(hwnd, hdc);
            PostQuitMessage(0);
        }
        return 0;
    }

    case WM_TIMER:
    {
        if (GetAsyncKeyState('W') & 0x8000) // ��ǰ�ƶ�
        {
            if (CountRayLength(posX + dirX * movSpeed, posY + dirY * movSpeed, dirX, dirY) > UNIT_CUBE_SIZE) // ǰ���Ƿ����ϰ���
            {
                // �����������
                posX += dirX * movSpeed;
                posY += dirY * movSpeed;

                // ���»���
                FillRect(hdcMem, &rt, hBr);

                for (INT screenX = 0; screenX < cxClient; screenX++)
                {
                    cameraX = 2.0 * screenX / (DOUBLE)cxClient - 1.0;

                    rayDirX = dirX + planeX * cameraX;
                    rayDirY = dirY + planeY * cameraX;

                    rayLength = CountRayLength(posX, posY, rayDirX, rayDirY);

                    rayProjection = 1.0 / sqrt(pow(rayDirX, 2) + pow(rayDirY, 2)) * rayLength;

                    wallHeight = (INT)(cyClient * UNIT_CUBE_SIZE * 2.0 / rayProjection);

                    switch (wallColor)
                    {
                    case 1:
                        SelectObject(hdcMem, hPenR);
                        break;

                    case 2:
                        SelectObject(hdcMem, hPenG);
                        break;

                    case 3:
                        SelectObject(hdcMem, hPenB);
                        break;

                    default:
                        SelectObject(hdcMem, hPen);
                        break;
                    }

                    MoveToEx(hdcMem, screenX, (cyClient - wallHeight) / 2, NULL);
                    LineTo(hdcMem, screenX, (cyClient + wallHeight) / 2);
                }

                BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
            }
        }
        else if (GetAsyncKeyState('S') & 0x8000) // ����ƶ�
        {
            if (CountRayLength(posX - dirX * movSpeed, posY - dirY * movSpeed, -1.0 * dirX, -1.0 * dirY) > UNIT_CUBE_SIZE)
            {
                posX -= dirX * movSpeed;
                posY -= dirY * movSpeed;

                FillRect(hdcMem, &rt, hBr);

                for (INT screenX = 0; screenX < cxClient; screenX++)
                {
                    cameraX = 2.0 * screenX / (DOUBLE)cxClient - 1.0;

                    rayDirX = dirX + planeX * cameraX;
                    rayDirY = dirY + planeY * cameraX;

                    rayLength = CountRayLength(posX, posY, rayDirX, rayDirY);

                    rayProjection = 1.0 / sqrt(pow(rayDirX, 2) + pow(rayDirY, 2)) * rayLength;

                    wallHeight = (INT)(cyClient * UNIT_CUBE_SIZE * 2.0 / rayProjection);

                    switch (wallColor)
                    {
                    case 1:
                        SelectObject(hdcMem, hPenR);
                        break;

                    case 2:
                        SelectObject(hdcMem, hPenG);
                        break;

                    case 3:
                        SelectObject(hdcMem, hPenB);
                        break;

                    default:
                        SelectObject(hdcMem, hPen);
                        break;
                    }

                    MoveToEx(hdcMem, screenX, (cyClient - wallHeight) / 2, NULL);
                    LineTo(hdcMem, screenX, (cyClient + wallHeight) / 2);
                }

                BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
            }
        }
        if (GetAsyncKeyState('A') & 0x8000) // �����ƶ�
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

                FillRect(hdcMem, &rt, hBr);

                for (INT screenX = 0; screenX < cxClient; screenX++)
                {
                    cameraX = 2.0 * screenX / (DOUBLE)cxClient - 1.0;

                    rayDirX = dirX + planeX * cameraX;
                    rayDirY = dirY + planeY * cameraX;

                    rayLength = CountRayLength(posX, posY, rayDirX, rayDirY);

                    rayProjection = 1.0 / sqrt(pow(rayDirX, 2) + pow(rayDirY, 2)) * rayLength;

                    wallHeight = (INT)(cyClient * UNIT_CUBE_SIZE * 2.0 / rayProjection);

                    switch (wallColor)
                    {
                    case 1:
                        SelectObject(hdcMem, hPenR);
                        break;

                    case 2:
                        SelectObject(hdcMem, hPenG);
                        break;

                    case 3:
                        SelectObject(hdcMem, hPenB);
                        break;

                    default:
                        SelectObject(hdcMem, hPen);
                        break;
                    }

                    MoveToEx(hdcMem, screenX, (cyClient - wallHeight) / 2, NULL);
                    LineTo(hdcMem, screenX, (cyClient + wallHeight) / 2);
                }

                BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
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

                FillRect(hdcMem, &rt, hBr);

                for (INT screenX = 0; screenX < cxClient; screenX++)
                {
                    cameraX = 2.0 * screenX / (DOUBLE)cxClient - 1.0;

                    rayDirX = dirX + planeX * cameraX;
                    rayDirY = dirY + planeY * cameraX;

                    rayLength = CountRayLength(posX, posY, rayDirX, rayDirY);

                    rayProjection = 1.0 / sqrt(pow(rayDirX, 2) + pow(rayDirY, 2)) * rayLength;

                    wallHeight = (INT)(cyClient * UNIT_CUBE_SIZE * 2.0 / rayProjection);

                    switch (wallColor)
                    {
                    case 1:
                        SelectObject(hdcMem, hPenR);
                        break;

                    case 2:
                        SelectObject(hdcMem, hPenG);
                        break;

                    case 3:
                        SelectObject(hdcMem, hPenB);
                        break;

                    default:
                        SelectObject(hdcMem, hPen);
                        break;
                    }

                    MoveToEx(hdcMem, screenX, (cyClient - wallHeight) / 2, NULL);
                    LineTo(hdcMem, screenX, (cyClient + wallHeight) / 2);
                }

                BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
            }
        }
        return 0;
    }

    case WM_MOUSEMOVE:
    {
        POINT curPos, preCurPos = {GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2};
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

            FillRect(hdcMem, &rt, hBr);

            for (INT screenX = 0; screenX < cxClient; screenX++)
            {
                cameraX = 2.0 * screenX / (DOUBLE)cxClient - 1.0;

                rayDirX = dirX + planeX * cameraX;
                rayDirY = dirY + planeY * cameraX;

                rayLength = CountRayLength(posX, posY, rayDirX, rayDirY);

                rayProjection = 1.0 / sqrt(pow(rayDirX, 2) + pow(rayDirY, 2)) * rayLength;

                wallHeight = (INT)(cyClient * UNIT_CUBE_SIZE * 2.0 / rayProjection);

                switch (wallColor)
                {
                case 1:
                    SelectObject(hdcMem, hPenR);
                    break;

                case 2:
                    SelectObject(hdcMem, hPenG);
                    break;

                case 3:
                    SelectObject(hdcMem, hPenB);
                    break;

                default:
                    SelectObject(hdcMem, hPen);
                    break;
                }

                MoveToEx(hdcMem, screenX, (cyClient - wallHeight) / 2, NULL);
                LineTo(hdcMem, screenX, (cyClient + wallHeight) / 2);
            }

            BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
        }
        else if (curPos.x > preCurPos.x) // ������ת�ӽ�
        {
            DOUBLE oldDirX = dirX;
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            DOUBLE oldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);

            FillRect(hdcMem, &rt, hBr);

            for (INT screenX = 0; screenX < cxClient; screenX++)
            {
                cameraX = 2.0 * screenX / (DOUBLE)cxClient - 1.0;

                rayDirX = dirX + planeX * cameraX;
                rayDirY = dirY + planeY * cameraX;

                rayLength = CountRayLength(posX, posY, rayDirX, rayDirY);

                rayProjection = 1.0 / sqrt(pow(rayDirX, 2) + pow(rayDirY, 2)) * rayLength;

                wallHeight = (INT)(cyClient * UNIT_CUBE_SIZE * 2.0 / rayProjection);

                switch (wallColor)
                {
                case 1:
                    SelectObject(hdcMem, hPenR);
                    break;

                case 2:
                    SelectObject(hdcMem, hPenG);
                    break;

                case 3:
                    SelectObject(hdcMem, hPenB);
                    break;

                default:
                    SelectObject(hdcMem, hPen);
                    break;
                }

                MoveToEx(hdcMem, screenX, (cyClient - wallHeight) / 2, NULL);
                LineTo(hdcMem, screenX, (cyClient + wallHeight) / 2);
            }

            BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
        }

        SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
        return 0;
    }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
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
        rayMapX = dirx != 0 ? floor((rayX + dirx * fabs(0.1 / dirx)) / 100) + 1 : (INT)(rayX / 100) + 1;
        rayMapY = diry != 0 ? MAP_HEIGHT - floor((rayY + diry * fabs(0.1 / diry)) / 100) - 2 : MAP_HEIGHT - (INT)(rayY / 100) - 2;

    } while (!map[rayMapY][rayMapX]); // �Ƿ������ϰ���

    // ����ǽ����ɫ
    wallColor = map[rayMapY][rayMapX];

    return sqrt(pow(posx - rayX, 2) + pow(posy - rayY, 2)); // ���߳�ʼλ�õ����ĳ���
}