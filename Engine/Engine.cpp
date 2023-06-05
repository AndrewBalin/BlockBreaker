#include "Engine.h"
#include "Levels/LevelsData.h"
#include <iostream>

#define SCALE 3

void Init()
{
    
}

HPEN BrickPen;
HBRUSH BrickBrush;
COLORREF color;

void DrawBrick(HDC hdc, COLORREF color, int x, int y)
{
    BrickPen = CreatePen(PS_SOLID, 0, color);
    BrickBrush =  CreateSolidBrush(color);
    SelectObject(hdc, BrickPen);
    SelectObject(hdc, BrickBrush); 
    RoundRect(hdc, x * SCALE, y * SCALE, (x + 15) * SCALE, (y + 7) * SCALE, 5 * SCALE, 5 * SCALE);
}

void DrawLevel(HDC hdc)
{
    for(int x = 0; x < 12; x++)
        for(int y = 0; y < 14; y++)
        {
            std::cout << y << ' ' << x << ' ' << Level_01[y][x] << std::endl;
            
            switch(Level_01[y][x])
            {
            case 1:
                color = RGB(255, 80, 255);
                break;
            case 2:
                color = RGB(0, 255, 255);
                break;
            }
            if(Level_01[y][x])
                DrawBrick(hdc, color, (x * 16) + 8, (y * 8) + 6);
        }
}

void DrawFrame(HDC hdc)
{
    DrawLevel(hdc);
}
