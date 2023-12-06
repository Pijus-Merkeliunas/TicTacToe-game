#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "TicTacToe.h"
#include <windowsx.h>
#include "dllheader.h"
typedef int (__cdecl *MYPROC)(LPWSTR);

const int CELL_SIZE=100;
HBRUSH blue,red;
HICON iconx,icono;
int turn = 1;
int gameBoard[9]={0,0,0,0,0,0,0,0,0};
int winner;
int PlayerWins[3]={0}; //Sitas reikalingas rezultatu skaiciavimui
HANDLE hFile;
char WriteToFile[1024];

/*BOOL GetGameBoardRect(HWND hwnd, RECT * pRect)
{
    RECT rc;
    if(GetClientRect(hwnd, &rc)){
      int width = rc.right - rc.left;
      int height = rc.bottom - rc.top;

      pRect->left = (width - CELL_SIZE * 3) / 2;
      pRect->top = (height - CELL_SIZE * 3) / 2;

      pRect->right = pRect->left + CELL_SIZE * 3;
      pRect->bottom = pRect->top + CELL_SIZE * 3;
      return TRUE;
      //Rectangle(hdc, left, top, right, bottom);
    }
    SetRectEmpty(pRect) ;
    return FALSE;
}*/

void DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
    MoveToEx(hdc,x1,y1,NULL);
    LineTo(hdc,x2,y2);
}
/*
int GetCellNumberFromPoint(HWND hwnd, int x, int y)
{
    POINT pt= {x,y};
    RECT rc;

    if(GetGameBoardRect(hwnd, &rc)){

        if(PtInRect(&rc, pt)){

            x = pt.x - rc.left;
            y = pt.y - rc.top;

            int column = x / CELL_SIZE;
            int row = y / CELL_SIZE;

            return column + row * 3;
        }
    }
    return -1; //outside the board

}

BOOL GetCellRect(HWND hwnd, int index, RECT * pRect)
{
    RECT rcBoard;

    SetRectEmpty(pRect);
    if(index < 0 || index > 8){

        return FALSE;
    }

    if(GetGameBoardRect(hwnd,&rcBoard)){

        int y = index / 3; //row
        int x = index % 3; //collumn

        pRect->left = rcBoard.left + x * CELL_SIZE+1; //+1 ir -1 skirti kad nenubrauktu liniju kai paspaudi langelius
        pRect->top = rcBoard.top + y * CELL_SIZE+1;
        pRect->right = pRect->left + CELL_SIZE-1;
        pRect->bottom = pRect->top + CELL_SIZE-1;

        return TRUE;
    }

return FALSE;
}
*//*
void WriteResults(char FileName[],int PlayerWins[])
{
    hFile=CreateFile(FileName,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    snprintf(WriteToFile,sizeof(WriteToFile),"Player 1: %d wins, Player 2: %d wins\n",PlayerWins[1],PlayerWins[2]);
    WriteFile(hFile,WriteToFile,strlen(WriteToFile),NULL,NULL);
    CloseHandle(hFile);
}*/
/*
0 - no winner
1 - player 1 wins
2 - player 2 wins
3 - draw
*/
int GetWinner()
{
    int cells[]={0,1,2, 3,4,5, 6,7,8 ,0,3,6, 1,4,7, 2,5,8, 0,4,8, 2,4,6};

    for(long long unsigned int i=0; i<ARRAYSIZE(cells);i+=3)
    {
        if(gameBoard[cells[i]]!=0 && gameBoard[cells[i]]==gameBoard[cells[i+1]]&&gameBoard[cells[i]]==gameBoard[cells[i+2]]){
            PlayerWins[gameBoard[cells[i]]]+=1;

            WriteResults("Rezultatai.txt",PlayerWins);

            return gameBoard[cells[i]];
        }
    }
    for(long long unsigned int i=0; i<ARRAYSIZE(gameBoard);i++)
    {
        if (gameBoard[i]==0)
            return 0;
    }
    return 3;
}

void ShowTurn(HWND hwnd, HDC hdc)
{
    RECT rc;
    static const char turn1[] = "Turn: Player 1";
    static const char turn2[] = "Turn: Player 2";

    const char * turnText = NULL;

    switch(winner)
    {
    case 0:
        turnText = turn==1 ? turn1 : turn2;
        break;
    case 1:
        turnText = "Player 1 is the winner!";
        break;
    case 2:
        turnText = "Player 2 is the winner!";
        break;
    case 3:
        turnText = "It's a draw!";
        break;
    }

    if(turnText != NULL && GetClientRect(hwnd,&rc)){

        rc.top = rc.bottom - 48;
        FillRect(hdc,&rc, (HBRUSH)GetStockObject(GRAY_BRUSH));
        SetTextColor(hdc,RGB(255,255,255));
        SetBkMode(hdc,TRANSPARENT);
        DrawText(hdc, turnText, lstrlen(turnText), &rc, DT_CENTER);
    }
}

/*void DrawIconCentered(HDC hdc, RECT * pRect, HICON hIcon)
{
    if(pRect != NULL)
    {
        int left = pRect->left + ((pRect->right - pRect->left) - 32)/2;
        int top = pRect->top + ((pRect->bottom - pRect->top) - 32)/2;
        DrawIcon(hdc,left,top,hIcon);
    }
}*/
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    //wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIcon = (HICON)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON3),IMAGE_ICON,
            128, 128,LR_DEFAULTCOLOR);
    wincl.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON3),IMAGE_ICON,
            128, 128,LR_DEFAULTCOLOR);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    //wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    wincl.hbrBackground = (HBRUSH)(GetStockObject(GRAY_BRUSH));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("TicTacToe"),       /* Title Text */
           (WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU), /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           800,                 /* The programs width */
           500,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           LoadMenu(hThisInstance, MAKEINTRESOURCE(IDR_MYMENU)),              /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (GetConsoleWindow(), SW_HIDE);  //SLEPIA CMD WINDOW
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_LBUTTONDOWN:
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            if (turn==0)
                break;

            int index = GetCellNumberFromPoint(hwnd,xPos,yPos);

            HDC hdc = GetDC(hwnd);

            if(hdc!=NULL){
                if(index != -1){
                    RECT rcCell;
                    if(gameBoard[index]==0 && GetCellRect(hwnd, index, &rcCell)){

                        gameBoard[index] = turn;

                        DrawIconCentered(hdc,&rcCell,turn==1 ? iconx:icono);

                        winner = GetWinner();
                        if (winner == 1 || winner == 2){

                            MessageBox(hwnd,
                                       (winner==1)? "Player 1 is the winner!" : "Player 2 is the winner!",
                                       "You Win!",
                                       MB_OK | MB_ICONINFORMATION);
                            turn=0;
                        }
                        else if(winner == 0){
                        turn= turn==2 ? 1 : 2;
                        }
                        else if(winner == 3){
                            MessageBox(hwnd, "It's a draw!","Good luck next time!",
                                       MB_OK | MB_ICONINFORMATION);
                            turn=0;
                        }

                        ShowTurn(hwnd,hdc);
                    }
                }
                // Peles paspaudimu testavimas
                /*char temp[100];
                sprintf(temp, "%d, %d",PlayerWins[1],PlayerWins[2]);
                TextOutA(hdc,xPos,yPos,temp,lstrlen(temp));
                */
                ReleaseDC(hwnd,hdc);
                }

            }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc;

            HBRUSH hBrush;
            HFONT hFont;

            if(GetGameBoardRect(hwnd, &rc)){

                RECT rcClient;

                ShowTurn(hwnd,hdc);

                SetBkMode(hdc,TRANSPARENT);
                if(GetClientRect(hwnd,&rcClient)){
                    SetTextColor(hdc,RGB(0,0,255));
                    TextOut(hdc, 16, 16, "Player 1", 8);
                    DrawIcon(hdc,24,40,iconx);
                    SetTextColor(hdc,RGB(255,0,0));
                    TextOut(hdc, rcClient.right - 72, 16, "Player 2", 8);
                    DrawIcon(hdc,rcClient.right-64,40,icono);
                }
                Rectangle(hdc,rc.left,rc.top,rc.right,rc.bottom);
            }
            //HORIZONTAL
            DrawLine(hdc,rc.left+CELL_SIZE,rc.top,rc.left+CELL_SIZE,rc.bottom);
            DrawLine(hdc,rc.left+CELL_SIZE*2,rc.top,rc.left+CELL_SIZE*2,rc.bottom);
            //VERTICAL
            DrawLine(hdc,rc.left,rc.top+CELL_SIZE,rc.right,rc.top+CELL_SIZE);
            DrawLine(hdc,rc.left,rc.top+CELL_SIZE*2,rc.right,rc.top+CELL_SIZE*2);


            EndPaint(hwnd, &ps);
        }
        case WM_CREATE:
        {
            blue = CreateSolidBrush(RGB(0,0,255));
            red = CreateSolidBrush(RGB(255,0,0));

            iconx = (HICON)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,
            128, 128,LR_DEFAULTCOLOR);
            icono = (HICON)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON2),IMAGE_ICON,
            128, 128,LR_DEFAULTCOLOR);
        }
        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case ID_FILE_EXIT:
                    PostQuitMessage(0);
                break;

                case ID_RESTART:
                    {
                        if(MessageBox(hwnd, "Are you sure you want to restart?", "New Game", MB_YESNO | MB_ICONQUESTION)==IDYES){
                            turn = 1;
                            ZeroMemory(gameBoard,sizeof(gameBoard));
                            winner=NULL;

                            InvalidateRect(hwnd, NULL, TRUE);
                            UpdateWindow(hwnd);
                        }
                    }
                    break;
            }
            break;
        case WM_DESTROY:
            DeleteObject(red);
            DeleteObject(blue);
            DestroyIcon(iconx);
            DestroyIcon(icono);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
BOOL CALLBACK DialogProc(HWND hDLG, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            return TRUE;
        case WM_CLOSE:
            EndDialog(hDLG, 0);
            return TRUE;
        case WM_DESTROY:
            EndDialog(hDLG, 0);
            return TRUE;
    }
    return FALSE;
}
