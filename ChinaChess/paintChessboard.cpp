#include<Windows.h>
#include"const.h"
#include <tchar.h>

void paintChessBoardMiddleText(HDC hdc);
void paintChessboardBossCrossLines(HDC hdc);
void paintChessboardColumns(HDC hdc);
void paintChessboardRows(HDC hdc);

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
        SetTextAlign(hdc, VTA_CENTER | VTA_BASELINE);
        TextOut(hdc, 2 * GRID_SIZE, ((CHESS_BOARD_ROW_COUNT / 2) - 1) * GRID_SIZE + (GRID_SIZE / 2), _T("³þºÓ"), 2);
        TextOut(hdc, ((CHESS_BOARD_ROW_COUNT / 2) + 1) * GRID_SIZE, ((CHESS_BOARD_ROW_COUNT / 2) - 1) * GRID_SIZE + (GRID_SIZE / 2), _T("ºº½ç"), 2);
}

