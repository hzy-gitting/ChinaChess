// ChinaChess.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "ChinaChess.h"
#include<Windowsx.h>
#include"const.h"
#include"paintChessboard.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void changeCurCamp();
void paintPiecies(HDC hdc);
void initPieciesName();
void initPieciesCamp();
void initPiecies();
void initPieciesLocation();

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
	if (!InitInstance(hInstance, nCmdShow))
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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHINACHESS));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CHINACHESS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

struct Piece {
	const char* name;
	int camp;
	int x;
	int y;
	int selected;
};
#define PIECE_COUNT (32)
struct Piece piecies[PIECE_COUNT];

void initPiecies() {
	initPieciesName();
	initPieciesCamp();
	initPieciesLocation();
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

void initPieciesCamp()
{
	for (int i = 0; i < 16; i++) {
		piecies[i].camp = 0;
	}
	for (int i = 16; i < 32; i++) {
		piecies[i].camp = 1;
	}
}

void initPieciesSelected()
{
	for (int i = 0; i < 32; i++) {
		piecies[i].selected = 0;
	}
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
void paintPiecies(HDC hdc)
{
	for (int i = 0; i < 32; i++) {
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		SelectObject(hdc, hPen);
		if (piecies[i].selected == 1) {
			SetTextColor(hdc, RGB(255, 255, 0));
		}
		else if (piecies[i].camp == 0) {
			SetTextColor(hdc, RGB(255, 0, 0));
		}
		else {
			SetTextColor(hdc, RGB(0, 0, 0));
		}
		TextOutA(hdc, piecies[i].x * GRID_SIZE, piecies[i].y * GRID_SIZE, piecies[i].name, 2);
	}
}

int getPieceIndexByChessCoordinate(POINT cc) {
	for (int i = 0; i < PIECE_COUNT; i++) {
		if (piecies[i].x == cc.x && piecies[i].y == cc.y) {
			return i;
		}
	}
	return -1;
}
void movePiece(int index, POINT target) {
	piecies[index].x = target.x;
	piecies[index].y = target.y;
}

void selectPiece(int index) {
	piecies[index].selected = 1;
}

void unselectPiece(int index) {
	piecies[index].selected = 0;
}
POINT pixelCoordToChessCoord(POINT pxc) {
	POINT cc;
	cc.x = pxc.x / GRID_SIZE + (abs(pxc.x % GRID_SIZE) > (GRID_SIZE / 2.0) ? (pxc.x > 0 ? 1 : -1) : 0);
	cc.y = pxc.y / GRID_SIZE + (abs(pxc.y % GRID_SIZE) > (GRID_SIZE / 2.0) ? (pxc.y > 0 ? 1 : -1) : 0);
	return cc;
}

int status = 0;
int pieceSelectedIndex = -1;
int curCamp = 0;

void changeCurCamp()
{
	curCamp = ((curCamp == 0) ? 1 : 0);
}
enum Status {
	SELECT_NONE,
	SELECT_ONE,
};
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
	case WM_LBUTTONDOWN:
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		x = x - 50;
		y = y - 50;
		POINT pxc;
		pxc.x = x;
		pxc.y = y;
		POINT cc = pixelCoordToChessCoord(pxc);
		if (cc.x < 0 || cc.y < 0 || cc.x >= CHESS_BOARD_COLUMN_COUNT || cc.y >= CHESS_BOARD_ROW_COUNT) {
			MessageBox(hWnd, _T("out of chessboard!"), _T("error"), MB_OKCANCEL);
			break;
		}
		int newPieceSelectedIndex = getPieceIndexByChessCoordinate(cc);
		
		if (status == SELECT_NONE) {
			if (newPieceSelectedIndex == -1) {
				MessageBox(hWnd, _T("out of any chess piece!"), _T("error"), MB_OKCANCEL);
				break;
			}
			if (curCamp != piecies[newPieceSelectedIndex].camp) {
				MessageBox(hWnd, _T("not cur camp!"), _T("error"), MB_OKCANCEL);
				break;
			}
			pieceSelectedIndex = newPieceSelectedIndex;
			selectPiece(pieceSelectedIndex);
			status = SELECT_ONE;
		}
		else if (status == SELECT_ONE) {
			unselectPiece(pieceSelectedIndex);
			if (newPieceSelectedIndex == pieceSelectedIndex) {
				status = SELECT_NONE;
			}
			else if (newPieceSelectedIndex != -1 && curCamp == piecies[newPieceSelectedIndex].camp) {
				selectPiece(newPieceSelectedIndex);
				pieceSelectedIndex = newPieceSelectedIndex;
			}
			else  {
				movePiece(pieceSelectedIndex, cc);
				status = SELECT_NONE;
				changeCurCamp();
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}
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
		paintPiecies(hdc);
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
