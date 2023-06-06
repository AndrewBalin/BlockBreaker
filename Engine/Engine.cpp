#include "Engine.h"
#include "Levels/LevelsData.h"
#include <iostream>

#define SCALE 3
HWND Hwnd;

bool GameStarted = false;

int FieldPadding = 8;

RECT PlatformRect, PrewPlatformRect; // Platform Position

int PlatformInnerWidth = 21; // Platform Inner Width
int PlatformWidth = PlatformInnerWidth + 12; // Platform Width
int PlatformX = (12 * 16) / 2 - PlatformWidth / 2; // Platform X-coordinate
const int PlatformY = 150; // Platform Y-coordinate: CONST
int PlatformStep = 3; // Platform Moving Step
int PlatformCircleScale = 8;

int BallX = PlatformX + PlatformWidth / 2 - 4;
int BallY = 148;
int BallScale = 4;
int BallXOffset = 0, BallYOffset = 0;

RECT BallRect, PrewBallRect;


//--------Init Engine Function---------------------------------

void EngineInit(HWND hwnd)
{
    Hwnd = hwnd;
    PlatformRect = {
        (PlatformX + FieldPadding) * SCALE,
        (PlatformY + FieldPadding) * SCALE,
        (PlatformX + FieldPadding + PlatformWidth) * SCALE,
        (PlatformY + FieldPadding + 8) * SCALE,
    };

    BallRect = {
        (BallX + FieldPadding) * SCALE,
        (BallY + FieldPadding) * SCALE,
        (BallX + FieldPadding + BallScale) * SCALE,
        (BallY + FieldPadding + BallScale) * SCALE,
    };

    SetTimer(Hwnd, ET_1, 50, 0);
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
    Rectangle(hdc, 6 * SCALE, 6 * SCALE, (12 * 16 + FieldPadding) * SCALE, 7 * SCALE); // Top Field Border
    Rectangle(hdc, (12 * 16 + FieldPadding) * SCALE, 6 * SCALE, (12 * 16 + FieldPadding + 1) * SCALE, (200 - 6) * SCALE); // Right Field Border
    
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
                        DrawBrick(hdc, BrickColor, (x * 16) + FieldPadding, (y * 8) + FieldPadding);
                }
    }
        
}

//--------Platform Drawing Function----------------------------

void DrawPlatform(HDC hdc)
{

    SetPenBrushColor(hdc, ECS_Background);
    Rectangle(hdc, PrewPlatformRect.left, PrewPlatformRect.top, PrewPlatformRect.right, PrewPlatformRect.bottom); // Clear Platform
    
    SetPenBrushColor(hdc, ECS_DefaultBreak_01);
    Ellipse(hdc, (PlatformX + FieldPadding) * SCALE, (PlatformY + FieldPadding) * SCALE,
        (PlatformX + FieldPadding + PlatformCircleScale) * SCALE, (PlatformY + FieldPadding + PlatformCircleScale) * SCALE); // Left Ball Drawing
    Ellipse(hdc, (PlatformX + PlatformInnerWidth + FieldPadding) * SCALE, (PlatformY + FieldPadding) * SCALE,
        (PlatformX + PlatformInnerWidth + FieldPadding + PlatformCircleScale) * SCALE, (PlatformY + FieldPadding + PlatformCircleScale) * SCALE); // Right Ball Drawing
    
    SetPenBrushColor(hdc, ECS_DefaultBreak_02);
    RoundRect(hdc, (PlatformX + FieldPadding + PlatformCircleScale / 2) * SCALE, (PlatformY + FieldPadding + 2) * SCALE, (PlatformX + PlatformWidth) * SCALE,
        (PlatformY + FieldPadding + 6) * SCALE, 5 * SCALE, 5 * SCALE); // Platform Rectangle Drawing
    
    SetPenBrushColor(hdc, ECS_White, 3);
    Arc(hdc, (PlatformX + FieldPadding + 2) * SCALE, (PlatformY + FieldPadding + 1) * SCALE, (PlatformX + FieldPadding + 7) * SCALE, (PlatformY + FieldPadding + 6) * SCALE,
        (PlatformX + FieldPadding + 2) * SCALE, (PlatformY + FieldPadding + 1) * SCALE, (PlatformX + FieldPadding + 1) * SCALE,
        (PlatformY + FieldPadding + 4) * SCALE); // Ball Highlight Drawing
    
    SetPenBrushColor(hdc, ECS_White, 0);
    Rectangle(hdc, (PlatformX + FieldPadding + 7) * SCALE, (PlatformY + FieldPadding + 3) * SCALE,                   
        (PlatformX + PlatformInnerWidth + FieldPadding - 5) * SCALE, (PlatformY + FieldPadding + PlatformCircleScale / 2) * SCALE);             
    Rectangle(hdc, (PlatformX + PlatformInnerWidth + FieldPadding - 3) * SCALE, (PlatformY + FieldPadding + 3) * SCALE,    
        (PlatformX + PlatformInnerWidth + FieldPadding - 1) * SCALE, (PlatformY + FieldPadding + PlatformCircleScale / 2) * SCALE); // Rectangle Highlight Drawing
    
}

void MovePlatfom()
{
    PrewPlatformRect = PlatformRect;
    
    PlatformRect = {
        (PlatformX + FieldPadding) * SCALE,
        (PlatformY + FieldPadding) * SCALE,
        (PlatformX + FieldPadding + PlatformWidth) * SCALE,
        (PlatformY + FieldPadding + 8) * SCALE,
    };

    if(!GameStarted)
    {
        BallX = PlatformX + PlatformWidth / 2 - 4;
    }
    
    InvalidateRect(Hwnd, &PrewPlatformRect, FALSE); // Clear Platform
    InvalidateRect(Hwnd, &PlatformRect, FALSE); // Redaraw Platform
}


//--------Ball Drawing Function--------------------------------

void DrawBall(HDC hdc)
{
    SetPenBrushColor(hdc, ECS_Background);
    Ellipse(hdc, PrewBallRect.left, PrewBallRect.top, PrewBallRect.right, PrewBallRect.bottom);
    
    SetPenBrushColor(hdc, ECS_White);
    Ellipse(hdc, BallRect.left, BallRect.top, BallRect.right, BallRect.bottom);
}

void MoveBall()
{
    PrewBallRect = BallRect;

    BallX += BallXOffset;
    BallY += BallYOffset;

    if(BallX < 0)
    {
        BallXOffset += 2;
    } else if (BallX > (16 * 12 - 5))
    {
        BallXOffset -= 2;
    }
    if(BallY < 0)
    {
        BallYOffset += 2;
    } else if((BallY == 148) and (PlatformX < BallX) and (BallX < (PlatformX + PlatformWidth)) and GameStarted)
    {
        BallYOffset -= 2;
    }
    
    
    BallRect = {
        (BallX + FieldPadding) * SCALE,
        (BallY + FieldPadding) * SCALE,
        (BallX + FieldPadding + BallScale) * SCALE,
        (BallY + FieldPadding + BallScale) * SCALE,
    };

    InvalidateRect(Hwnd, &PrewBallRect, FALSE); // Clear Platform
    InvalidateRect(Hwnd, &BallRect, FALSE); // Redaraw Platform
    MovePlatfom();
}

//

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
    DrawBall(hdc);
    //DrawBonus(hdc, TripleBall);
    
}

//--------Control Game Function--------------------------------

void GameControl(EKeyType key)
{
    switch(key)
    {
    case EKT_Left: // Move Left
        if(PlatformX > 0)
        {
            PlatformX -= PlatformStep;
            MovePlatfom();
        }
        break;
    case EKT_Right: // Move Right
        if((PlatformX + PlatformInnerWidth + FieldPadding + 8) * SCALE < (12 * 16 + FieldPadding - 1) * SCALE)
        {
            PlatformX += PlatformStep;
            MovePlatfom();
        }
        break;
    case EKT_Space:
        if(!GameStarted)
        {
            GameStarted = true;
            BallXOffset = 1, BallYOffset = -1;
        }
    }
}

//

void OnTick()
{
    MoveBall();
}