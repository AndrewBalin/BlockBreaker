#include "Engine.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Levels/LevelsData.h"
#include <iostream>
#include <map>

//========Level Variables======================================

#define SCALE 3 // Game Scale
HWND Hwnd; // Window Handle

bool GameStarted = false; // Level State

int FieldPadding = 8; // Padding Behind Window And Field
int DefaultBrickWidth = 16; // Default Single Brick Width
int DefaultBrickHeight = 8; // Default Single Brick Height

RECT LevelRect = {
    FieldPadding * SCALE,
    FieldPadding * SCALE,
    (FieldPadding + (DefaultBrickWidth * 12)) * SCALE,
    (FieldPadding + (DefaultBrickHeight * 14)) * SCALE,
    
}; // Bricks Position 

//========Platform Variables===================================

RECT PlatformRect, PrewPlatformRect; // Platform Position

int PlatformInnerWidth = 21; // Platform Inner Width
int PlatformCircleScale = 8; // Platform Circle Scale

int PlatformWidth = PlatformInnerWidth + 12; // Platform Width
int PlatformX = (12 * 16) / 2 - PlatformWidth / 2; // Platform X-coordinate
const int PlatformY = 150; // Platform Y-coordinate: CONST

int PlatformStep = 3; // Platform Moving Step

//========Ball Variables=======================================

std::map<int, double> StartAngle = {
    {-3, 5 * M_PI / 6},
    {-2, 3 * M_PI / 4},
    {-1, 2 * M_PI / 3},
    {0, M_PI / 2},
    {1, M_PI / 3},
    {2, M_PI / 4},
    {3, M_PI / 6},
};

RECT BallRect, PrewBallRect; // Ball Position
RECT BallDirectionRect = {
    8 * SCALE,
    426,
    (16 * 12 - 8) * SCALE,
    471,
    
}; // Ball Direction Position

int BallScale = 4; // Ball Scale
double BallSpeed = 3.0; // Ball Speed
int BallDirectionNum = -3; // Ball Direction: <map>StartAngle
double BallDirection; // Ball Direction: Radian Angle
double PrewBallDirection; // Previous Ball Direction: Radian Angle

int BallX = PlatformX + PlatformWidth / 2 - 4; // Ball X-coordinate
int BallY = 148; // Ball Y-coordinate

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

    BallDirection = StartAngle[BallDirectionNum];
    PrewBallDirection = BallDirection;

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
    RoundRect(hdc, x * SCALE, y * SCALE, (x + DefaultBrickWidth - 1) * SCALE, (y + DefaultBrickHeight - 1) * SCALE, 5 * SCALE, 5 * SCALE);
}

//--------Level Drawing Function-------------------------------

EColorScheme BrickColor;

void DrawLevel(HDC hdc)
{
    for(int x = 0; x < 12; x++)
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

//--------Platform Drawing Function----------------------------

void DrawPlatform(HDC hdc)
{

    SetPenBrushColor(hdc, ECS_Background);
    Rectangle(hdc, PrewPlatformRect.left, PrewPlatformRect.top, PrewPlatformRect.right, PrewPlatformRect.bottom); // Clear Platform
    
    SetPenBrushColor(hdc, ECS_DefaultBreak_01);
    Ellipse(hdc, PlatformRect.left, PlatformRect.top,
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
    
    SetPenBrushColor(hdc, ECS_White);
    Rectangle(hdc, (PlatformX + FieldPadding + 7) * SCALE, (PlatformY + FieldPadding + 3) * SCALE,                   
        (PlatformX + PlatformInnerWidth + FieldPadding - 5) * SCALE, (PlatformY + FieldPadding + PlatformCircleScale / 2) * SCALE);             
    Rectangle(hdc, (PlatformX + PlatformInnerWidth + FieldPadding - 3) * SCALE, (PlatformY + FieldPadding + 3) * SCALE,    
        (PlatformX + PlatformInnerWidth + FieldPadding - 1) * SCALE, (PlatformY + FieldPadding + PlatformCircleScale / 2) * SCALE); // Rectangle Highlight Drawing
    
}

void MovePlatfom()
{

    std::cout << BallX << ' ' << BallY << std::endl;
    
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

void DrawDirection(HDC hdc, int direction)
{
    if(!GameStarted)
    {
        int BallCenter[2] = {BallX + BallScale / 2, BallY + BallScale / 2};
        int FromX = (int)(BallCenter[0] + FieldPadding + 4 * cos(StartAngle[direction])) * SCALE;
        int FromY = (int)(BallCenter[1] + FieldPadding - 4 * sin(StartAngle[direction])) * SCALE;

        int ToX = (int)(BallCenter[0] + FieldPadding + 15 * cos(StartAngle[direction])) * SCALE;
        int ToY = (int)(BallCenter[1] + FieldPadding - 15 * sin(StartAngle[direction])) * SCALE;
        
        SetPenBrushColor(hdc, ECS_White, 5);
        MoveToEx(hdc, FromX, FromY, NULL);
        LineTo(hdc, ToX, ToY);
    }
}

void ClearDirection(HDC hdc, int direction)
{
    int BallCenter[2] = {BallX + BallScale / 2, BallY + BallScale / 2};
    int FromX = (int)(BallCenter[0] + FieldPadding + 4 * cos(StartAngle[direction])) * SCALE;
    int FromY = (int)(BallCenter[1] + FieldPadding - 4 * sin(StartAngle[direction])) * SCALE;

    int ToX = (int)(BallCenter[0] + FieldPadding + 15 * cos(StartAngle[direction])) * SCALE;
    int ToY = (int)(BallCenter[1] + FieldPadding - 15 * sin(StartAngle[direction])) * SCALE;
    
    SetPenBrushColor(hdc, ECS_Background, 6);
    MoveToEx(hdc, FromX, FromY, NULL);
    LineTo(hdc, ToX, ToY);
}

void ChangeBallDirection()
{
    
}

void DrawBall(HDC hdc)
{
    SetPenBrushColor(hdc, ECS_Background);
    Ellipse(hdc, PrewBallRect.left, PrewBallRect.top, PrewBallRect.right, PrewBallRect.bottom); // Clear Ball
    
    SetPenBrushColor(hdc, ECS_White);
    Ellipse(hdc, BallRect.left, BallRect.top, BallRect.right, BallRect.bottom); // Ball Drawing
}



void MoveBall()
{
    PrewBallRect = BallRect;    

    if(GameStarted)
    {
        int NextBallX = BallX + (int)(cos(BallDirection) * BallSpeed);
        int NextBallY = BallY - (int)(sin(BallDirection) * BallSpeed);
        BallX += (int)(cos(BallDirection) * BallSpeed);
        BallY -= (int)(sin(BallDirection) * BallSpeed);

        if(NextBallX < 0)
        {
            NextBallX = -NextBallX;
            BallDirection = M_PI - BallDirection;
        } else if(NextBallX > (16 * 12 - 4))
        {
            NextBallX = (16 * 12 - 4) - (NextBallX - (16 * 12 - 4));
            BallDirection = M_PI - BallDirection;
        }
        
        if(NextBallY < 2)
        {
            NextBallY = -NextBallY;
            BallDirection = -BallDirection;
        } else if(NextBallY >= 148 and NextBallY <= 149  and NextBallX >= PlatformX and NextBallX <= (PlatformX + PlatformWidth))
        {
            NextBallY = 148 - (NextBallY - 148);
            BallDirection = -BallDirection;
        }
        

        BallX = NextBallX, BallY = NextBallY;
    }
    
    
    BallRect = {
        (BallX + FieldPadding) * SCALE,
        (BallY + FieldPadding) * SCALE,
        (BallX + FieldPadding + BallScale) * SCALE,
        (BallY + FieldPadding + BallScale) * SCALE,
    };

    InvalidateRect(Hwnd, &PrewBallRect, FALSE); // Clear Ball
    InvalidateRect(Hwnd, &BallRect, FALSE); // Redaraw Ball
    MovePlatfom(); // Update Platform
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

    if(IntersectRect(&IntersectionRect, &area, &LevelRect))
        DrawLevel(hdc);

    if(IntersectRect(&IntersectionRect, &area, &BallRect))
        DrawBall(hdc);
    
    DrawInterface(hdc);
    //DrawDirection(hdc, BallDirectionNum);
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
    case EKT_Up:
        
        break;
    case EKT_Space:
        if(!GameStarted)
        {
            GameStarted = true;
           
        }
    }
}

//

void OnTick()
{
    MoveBall();
}