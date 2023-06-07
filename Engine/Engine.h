#pragma once

#include <Windows.h>

//

//--------Game Color Scheme------------------------------------

enum EColorScheme
{ 
    ECS_Background = RGB(0, 0, 0),
    ECS_White = RGB(255, 255, 255),
    ECS_DefaultBreak_01 = RGB(255, 80, 255),
    ECS_DefaultBreak_02 = RGB(0, 255, 255),
};

//--------Game Control Scheme----------------------------------

enum EKeyType
{
    EKT_Left,
    EKT_Right,
    EKT_Up,
    EKT_Dowun,
    EKT_Space,
};

//

enum ETimers
{
    ET_1 = WM_USER + 1,
};

//--------Engine Functions-------------------------------------

void EngineInit(HWND hwnd); // Init Engine Function
void DrawFrame(HDC hdc, RECT &area); // Window Drawing Function
void GameControl(EKeyType key); // Control Game Function
void OnTick();
