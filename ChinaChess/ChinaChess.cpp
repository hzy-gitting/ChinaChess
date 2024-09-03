﻿// ChinaChess.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "ChinaChess.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void initPieciesName();
void initPieciesCamp();
void initPiecies();
void initPieciesLocation();
void paintChessboard(HDC hdc);
void paintChessBoardMiddleText(HDC hdc);
void paintChessboardBossCrossLines(HDC hdc);
void paintChessboardColumns(HDC hdc);
void paintChessboardRows(HDC hdc);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    initPiecies();
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CHINACHESS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHINACHESS));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHINACHESS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CHINACHESS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#define GRID_SIZE (50)
#define CHESS_BOARD_ROW_COUNT (10)
#define CHESS_BOARD_COLUMN_COUNT (9)
#define CHESS_BOARD_HIGHT ((CHESS_BOARD_ROW_COUNT - 1) * GRID_SIZE)
#define CHESS_BOARD_WIDTH ((CHESS_BOARD_COLUMN_COUNT - 1) * GRID_SIZE)
struct Piece {
        const char* name;
        int camp;
        int x;
        int y;
};
#define PIECIE_COUNT (32)
struct Piece piecies[PIECIE_COUNT];

void initPiecies() {
        initPieciesName();
        initPieciesCamp();
        initPieciesLocation();
}
void initPieciesLocation()
{
        for (int i = 0; i < 9; i++) {
                piecies[i].x = i;
                piecies[i].y = 0;
        }
        piecies[9].x = 1;
        piecies[9].y = 2;
        piecies[10].x = CHESS_BOARD_COLUMN_COUNT - 1 - 1;
        piecies[10].y = 2;
        for (int i = 11; i < 16; i++) {
                piecies[i].y = 3;
                piecies[i].x = (i - 11) * 2;
        }
        for (int i = 16; i < 32; i++) {
                piecies[i].x = piecies[i - 16].x;
                piecies[i].y = CHESS_BOARD_ROW_COUNT - 1 - piecies[i - 16].y;
        }
}
void initPieciesCamp()
{
        for (int i = 0; i < 16; i++) {
                piecies[i].camp = 0;
        }
        for (int i = 16; i < 32; i++) {
                piecies[i].camp = 1;
        }
}
void initPieciesName()
{
        piecies[0].name = "车";
        piecies[1].name = "马";
        piecies[2].name = "象";
        piecies[3].name = "士";
        piecies[4].name = "将";
        piecies[5].name = "士";
        piecies[6].name = "象";
        piecies[7].name = "马";
        piecies[8].name = "车";
        piecies[9].name = "炮";
        piecies[10].name = "炮";
        for (int i = 11; i < 16; i++) {
                piecies[i].name = "兵";
        }
        for (int i = 16; i < 32; i++) {
                piecies[i].name = piecies[i - 16].name;
        }
}
//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            int offsetX = 50;
            int offsetY = 50;
            BOOL ret = SetWindowOrgEx(hdc, -offsetX, -offsetY, NULL);
            if (!ret) {
                  MessageBox(hWnd, _T("SetWindowOrgEx fail"), _T("error"), MB_OKCANCEL);
            }
            paintChessboard(hdc);
            for (int i = 0; i < 32; i++) {
                    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
                    SelectObject(hdc, hPen);
                    if (piecies[i].camp == 0) {
                            SetTextColor(hdc, RGB(255, 0, 0));
                    }
                    else {
                            SetTextColor(hdc, RGB(0, 0, 0));
                    }
                    TextOutA(hdc, piecies[i].x * GRID_SIZE, piecies[i].y * GRID_SIZE, piecies[i].name, 2);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void paintChessboard(HDC hdc)
{
        paintChessboardRows(hdc);
        paintChessboardColumns(hdc);
        paintChessboardBossCrossLines(hdc);
        paintChessBoardMiddleText(hdc);
}

void paintChessboardRows(HDC hdc)
{
        for (int i = 0; i < CHESS_BOARD_ROW_COUNT; i++) {
                int y = i * GRID_SIZE;
                MoveToEx(hdc, 0, y, NULL);
                LineTo(hdc, CHESS_BOARD_WIDTH, y);
        }
}
void paintChessboardColumns(HDC hdc)
{
        MoveToEx(hdc, 0, 0, NULL);
        LineTo(hdc, 0, CHESS_BOARD_HIGHT);
        for (int i = 1; i < CHESS_BOARD_COLUMN_COUNT - 1; i++) {
                int x = i * GRID_SIZE;
                MoveToEx(hdc, x, 0, NULL);
                LineTo(hdc, x, ((CHESS_BOARD_ROW_COUNT / 2) - 1) * GRID_SIZE);
                MoveToEx(hdc, x, (CHESS_BOARD_ROW_COUNT / 2) * GRID_SIZE, NULL);
                LineTo(hdc, x, CHESS_BOARD_HIGHT);
        }
        MoveToEx(hdc, CHESS_BOARD_WIDTH, 0, NULL);
        LineTo(hdc, CHESS_BOARD_WIDTH, CHESS_BOARD_HIGHT);
}

void paintChessboardBossCrossLines(HDC hdc)
{

        MoveToEx(hdc, ((CHESS_BOARD_COLUMN_COUNT / 2) - 1) * GRID_SIZE, 0, NULL);
        LineTo(hdc, ((CHESS_BOARD_COLUMN_COUNT / 2) + 1) * GRID_SIZE, 2 * GRID_SIZE);

        MoveToEx(hdc, ((CHESS_BOARD_COLUMN_COUNT / 2) + 1) * GRID_SIZE, 0, NULL);
        LineTo(hdc, ((CHESS_BOARD_COLUMN_COUNT / 2) - 1) * GRID_SIZE, 2 * GRID_SIZE);

        MoveToEx(hdc, ((CHESS_BOARD_COLUMN_COUNT / 2) - 1) * GRID_SIZE, 7 * GRID_SIZE, NULL);
        LineTo(hdc, ((CHESS_BOARD_COLUMN_COUNT / 2) + 1) * GRID_SIZE, 9 * GRID_SIZE);

        MoveToEx(hdc, ((CHESS_BOARD_COLUMN_COUNT / 2) + 1) * GRID_SIZE, 7 * GRID_SIZE, NULL);
        LineTo(hdc, ((CHESS_BOARD_COLUMN_COUNT / 2) - 1) * GRID_SIZE, 9 * GRID_SIZE);
}

void paintChessBoardMiddleText(HDC hdc)
{
        SetTextAlign(hdc, VTA_CENTER |VTA_BASELINE);
        TextOut(hdc, 2 * GRID_SIZE, ((CHESS_BOARD_ROW_COUNT / 2) - 1) * GRID_SIZE + (GRID_SIZE / 2), _T("楚河"), 2);
        TextOut(hdc, ((CHESS_BOARD_ROW_COUNT / 2) + 1) * GRID_SIZE, ((CHESS_BOARD_ROW_COUNT / 2) - 1) * GRID_SIZE + (GRID_SIZE / 2), _T("汉界"), 2);
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
