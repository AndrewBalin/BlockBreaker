#include "Engine.h"
#include "Levels/LevelsData.h"
#include <iostream>

#define SCALE 3
HWND Hwnd;

RECT PlatformRect, PrewPlatformRect; // Platform Position

int PlatformInnerWidth = 21; // Platform Inner Width
int PlatformWidth = PlatformInnerWidth + 12; // Platform Width
int PlatformX = (12 * 16) / 2 - PlatformWidth / 2; // Platform X-coordinate
const int PlatformY = 160; // Platform Y-coordinate: CONST
int PlatformStep = 3; // Platform Moving Step



//--------Init Engine Function---------------------------------

void EngineInit(HWND hwnd)
{
    Hwnd = hwnd;
    PlatformRect = {
        (PlatformX + 8) * SCALE,
        PlatformY * SCALE,
        (PlatformX + 8 + PlatformWidth) * SCALE,
        (PlatformY + 8) * SCALE,
    };
}

//--------Set Pen And Brush Drawing Function-------------------

HPEN Pen;
HBRUSH Brush;

void SetPenBrushColor(HDC hdc, EColorScheme color, int pen_width = 0)
{
    DeleteObject(Pen); // Clear Pen
    DeleteObject(Brush); // Clear Brush
    Pen = CreatePen(PS_SOLID, pen_width, color);
    Brush =  CreateSolidBrush(color);
    SelectObject(hdc, Pen);
    SelectObject(hdc, Brush);
}

//--------Interface Drawing Function---------------------------

void DrawInterface(HDC hdc)
{
    SetPenBrushColor(hdc, ECS_White);
    Rectangle(hdc, 6 * SCALE, 6 * SCALE, 7 * SCALE, (200 - 6) * SCALE); // Left Field Border 
    Rectangle(hdc, 6 * SCALE, 6 * SCALE, (12 * 16 + 8) * SCALE, 7 * SCALE); // Top Field Border
    Rectangle(hdc, (12 * 16 + 8) * SCALE, 6 * SCALE, (12 * 16 + 9) * SCALE, (200 - 6) * SCALE); // Right Field Border
    
}

//--------Brick Drawing Function-------------------------------

void DrawBrick(HDC hdc, EColorScheme color, int x, int y)
{
    SetPenBrushColor(hdc, color);
    RoundRect(hdc, x * SCALE, y * SCALE, (x + 15) * SCALE, (y + 7) * SCALE, 5 * SCALE, 5 * SCALE);
}

//--------Level Drawing Function-------------------------------

EColorScheme BrickColor;

void DrawLevel(HDC hdc)
{
    for(int x = 0; x < 12; x++)
    {
        std::cout << x << std::endl;
                for(int y = 0; y < 14; y++)
                {            
                    switch(Level_01[y][x])
                    {
                    case 1:
                        BrickColor = ECS_DefaultBreak_01;
                        break;
                    case 2:
                        BrickColor = ECS_DefaultBreak_02;
                        break;
                    }
                    if(Level_01[y][x])
                        DrawBrick(hdc, BrickColor, (x * 16) + 8, (y * 8) + 8);
                }
    }
        
}

//--------Platform Drawing Function----------------------------

void DrawPlatform(HDC hdc)
{

    SetPenBrushColor(hdc, ECS_Background);
    Rectangle(hdc, PrewPlatformRect.left, PrewPlatformRect.top, PrewPlatformRect.right, PrewPlatformRect.bottom); // Clear Platform
    
    int Circle = 8;
    
    SetPenBrushColor(hdc, ECS_DefaultBreak_01);
    Ellipse(hdc, (PlatformX + 8) * SCALE, PlatformY * SCALE,
        (PlatformX + 8 + Circle) * SCALE, (PlatformY + Circle) * SCALE); // Left Ball Drawing
    Ellipse(hdc, (PlatformX + PlatformInnerWidth + 8) * SCALE, PlatformY * SCALE,
        (PlatformX + PlatformInnerWidth + 8 + Circle) * SCALE, (PlatformY + Circle) * SCALE); // Right Ball Drawing
    
    SetPenBrushColor(hdc, ECS_DefaultBreak_02);
    RoundRect(hdc, (PlatformX + 8 + 4) * SCALE, (PlatformY + 2) * SCALE, (PlatformX + PlatformWidth) * SCALE, (PlatformY + 6) * SCALE,
        5 * SCALE, 5 * SCALE); // Platform Rectangle Drawing
    
    SetPenBrushColor(hdc, ECS_White, 3);
    Arc(hdc, (PlatformX + 8 + 2) * SCALE, (PlatformY + 1) * SCALE, (PlatformX + 8 + 7) * SCALE, (PlatformY + 6) * SCALE, (PlatformX + 8 + 2) * SCALE,
        (PlatformY + 1) * SCALE, (PlatformX + 8 + 1) * SCALE, (PlatformY + 4) * SCALE); // Ball Highlight Drawing
    
    SetPenBrushColor(hdc, ECS_White, 0);
    Rectangle(hdc, (PlatformX + 8 + 7) * SCALE, (PlatformY + 3) * SCALE,                   
        (PlatformX + PlatformInnerWidth + 8 - 5) * SCALE, (PlatformY + 4) * SCALE);             
    Rectangle(hdc, (PlatformX + PlatformInnerWidth + 8 - 3) * SCALE, (PlatformY + 3) * SCALE,    
        (PlatformX + PlatformInnerWidth + 8 - 1) * SCALE, (PlatformY + 4) * SCALE); // Rectangle Highlight Drawing
    
}

void MovePlatfom()
{
    PrewPlatformRect = PlatformRect;
    
    PlatformRect = {
        (PlatformX + 8) * SCALE,
        PlatformY * SCALE,
        (PlatformX + 4 + PlatformWidth) * SCALE,
        (PlatformY + 8) * SCALE,
    };
    
    InvalidateRect(Hwnd, &PrewPlatformRect, FALSE); // Clean Platform
    InvalidateRect(Hwnd, &PlatformRect, FALSE); // Redaraw Platform
}


//--------Bonus Drawing Function-------------------------------

BITMAP BmImage; // BitMap Image
HBITMAP HmImage; // HBitMap Image

enum EBonusType // Nums To Bonus Types
{
    EBT_TripleBall,
    EBT_xplosionBall,
    EBT_PhantomBall,
};

void DrawBonus(HDC hdc, EBonusType type)
{
    std::string link;
    switch (type)
    {
    case EBT_TripleBall:
        link = "TripleBall";
        break;
    }
    link = "H:\\Projects\\Game\\Game\\Engine\\Assets\\Images\\" + link + ".bmp";
    HmImage = (HBITMAP)LoadImage(0, (LPCWSTR)link.c_str(), IMAGE_BITMAP, 1000, 1000, LR_LOADFROMFILE);
    HDC hdcMem = (hdc);

    SelectObject(hdcMem, HmImage);
    GetObject((HGDIOBJ)HmImage, sizeof(BmImage), &BmImage);
    BitBlt(hdc, 10, 10, BmImage.bmWidth, BmImage.bmHeight, hdcMem, 0, 0, SRCCOPY);
}

//--------Window Drawing Function------------------------------

void DrawFrame(HDC hdc, RECT &area)
{

    RECT IntersectionRect;
    
    if(IntersectRect(&IntersectionRect, &area, &PlatformRect))
        DrawPlatform(hdc);
    
    DrawInterface(hdc);
    DrawLevel(hdc);
    //DrawBonus(hdc, TripleBall);
    
}

//--------Control Game Function--------------------------------

void GameControl(EKeyType key)
{
    switch(key)
    {
    case EKT_Left: // Move Left
        if(PlatformX > 3)
        {
            PlatformX -= PlatformStep;
            MovePlatfom();
        }
        break;
    case EKT_Right: // Move Right
        if((PlatformX + PlatformInnerWidth + 8 + 8) * SCALE < (12 * 16 + 7 - 1) * SCALE)
        {
            PlatformX += PlatformStep;
            MovePlatfom();
        }
        break;
    }
}
