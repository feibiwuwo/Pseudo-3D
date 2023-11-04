#include <windows.h>
#include <math.h>

// 圆周率
#define PI 3.1415926

// 地图长宽
#define MAP_WIDTH 20
#define MAP_HEIGHT 20

// 地图，非零区域为墙壁，边长为 100 ，数字代表颜色，上北下南左西右东
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

// 单位方形区域的边长
#define UNIT_CUBE_SIZE 50.0

// 单位移动速度
#define UNIT_SPEED 1.0

// 玩家坐标，原点位于地图左下角， x 轴方向为正北， y 轴方向为正东
DOUBLE posX;
DOUBLE posY;

// 玩家视角方向向量
DOUBLE dirX;
DOUBLE dirY;

// 玩家移动速度
DOUBLE movSpeed = 5.0 * UNIT_SPEED;

// 玩家视角旋转速度，弧度制
DOUBLE rotSpeed = PI / 180;

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

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);                      // 消息处理
DOUBLE CountRayLength(DOUBLE posx, DOUBLE posy, DOUBLE dirx, DOUBLE diry); // 求某点发出射线到击中障碍物的距离

// 程序入口
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   PSTR szCmdLine,
                   int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("MyWin");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    // 设计窗口
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

    // 注册窗口
    RegisterClass(&wndclass);

    // 创建窗口
    hwnd = CreateWindow(szAppName,
                        TEXT("MyDemo"),
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
        // 数据初始化，方向向量和相机平面向量垂直且长度为 1
        posX = 900.0;
        posY = 900.0;
        dirX = 0.0;
        dirY = 1.0;
        planeX = 1.0;
        planeY = 0.0;

        hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));    // 黑笔
        hPenR = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // 红笔
        hPenG = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // 绿笔
        hPenB = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); // 蓝笔
        hBr = CreateSolidBrush(RGB(255, 255, 255));     // 白刷

        SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2); // 鼠标光标位于屏幕中心
        SetTimer(hwnd, 1, 20, NULL);                                                        // 计时器
        return 0;
    }

    case WM_SIZE:
    {
        // 窗口长宽
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        GetClientRect(hwnd, &rt);

        // 双缓冲
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

        // 游戏画面
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
            wallHeight = (INT)(cyClient * UNIT_CUBE_SIZE * 2.0 / rayProjection);

            // 选择墙壁颜色
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

            // 画墙壁
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
        // 资源销毁
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
        if (wParam == VK_ESCAPE) // ESC键退出程序
        {
            // 资源销毁
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
        if (GetAsyncKeyState('W') & 0x8000) // 向前移动
        {
            if (CountRayLength(posX + dirX * movSpeed, posY + dirY * movSpeed, dirX, dirY) > UNIT_CUBE_SIZE) // 前方是否有障碍物
            {
                // 计算玩家坐标
                posX += dirX * movSpeed;
                posY += dirY * movSpeed;

                // 重新绘制
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
        else if (GetAsyncKeyState('S') & 0x8000) // 向后移动
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
        if (GetAsyncKeyState('A') & 0x8000) // 向左移动
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

        if (curPos.x < preCurPos.x) // 向左旋转视角
        {
            // 计算方向向量和相机向量，两者始终垂直且长度不变
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
        else if (curPos.x > preCurPos.x) // 向右旋转视角
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
        rayMapX = dirx != 0 ? floor((rayX + dirx * fabs(0.1 / dirx)) / 100) + 1 : (INT)(rayX / 100) + 1;
        rayMapY = diry != 0 ? MAP_HEIGHT - floor((rayY + diry * fabs(0.1 / diry)) / 100) - 2 : MAP_HEIGHT - (INT)(rayY / 100) - 2;

    } while (!map[rayMapY][rayMapX]); // 是否命中障碍物

    // 设置墙壁颜色
    wallColor = map[rayMapY][rayMapX];

    return sqrt(pow(posx - rayX, 2) + pow(posy - rayY, 2)); // 光线初始位置到落点的长度
}
