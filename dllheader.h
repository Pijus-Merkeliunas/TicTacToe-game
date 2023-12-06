#ifndef dllheader_H
#define dllheader_H
#endif

#include <windows.h>

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

extern "C" __declspec(dllexport) BOOL GetGameBoardRect(HWND hwnd, RECT * pRect);
extern "C" __declspec(dllexport) int GetCellNumberFromPoint(HWND hwnd, int x, int y);
extern "C" __declspec(dllexport) BOOL GetCellRect(HWND hwnd, int index, RECT * pRect);
extern "C" __declspec(dllexport) void WriteResults(char FileName[],int PlayerWins[]);
extern "C" __declspec(dllexport) int GetWinner();
extern "C" __declspec(dllexport) void DrawIconCentered(HDC hdc, RECT * pRect, HICON hIcon);
