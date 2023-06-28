#include "Engine.h"
#include "Levels/LevelsData.h"

//--------Init Engine Function---------------------------------

void QSEngine::EngineInit(HWND hwnd)
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

//--------Set Pen And Brush Color Function---------------------

HPEN Pen;
HBRUSH Brush;

void QSEngine::SetPenBrushColor(HDC hdc, EColorScheme color, int pen_width)
{
    DeleteObject(Pen); // Clear Pen
    DeleteObject(Brush); // Clear Brush
    Pen = CreatePen(PS_SOLID, pen_width, color);
    Brush =  CreateSolidBrush(color);
    SelectObject(hdc, Pen);
    SelectObject(hdc, Brush);
}

//--------Interface Drawing Function---------------------------

void QSEngine::DrawInterface(HDC hdc)
{
    SetPenBrushColor(hdc, ECS_White);
    Rectangle(hdc, 6 * SCALE, 6 * SCALE, 7 * SCALE, (200 - 6) * SCALE); // Left Field Border 
    Rectangle(hdc, 6 * SCALE, 6 * SCALE, (12 * 16 + FieldPadding) * SCALE, 7 * SCALE); // Top Field Border
    Rectangle(hdc, (12 * 16 + FieldPadding) * SCALE, 6 * SCALE, (12 * 16 + FieldPadding + 1) * SCALE, (200 - 6) * SCALE); // Right Field Border
}

//--------Brick Drawing Function-------------------------------

void QSEngine::DrawBrick(HDC hdc, EColorScheme color, int x, int y)
{
    SetPenBrushColor(hdc, color);
    RoundRect(hdc, x * SCALE, y * SCALE, (x + DefaultBrickWidth - 1) * SCALE, (y + DefaultBrickHeight - 1) * SCALE, 5 * SCALE, 5 * SCALE);
}

//--------Level Drawing Function-------------------------------

EColorScheme BrickColor;

void QSEngine::DrawLevel(HDC hdc)
{
    for(int x = 0; x < 12; x++)
        for(int y = 0; y < 14; y++)
        {            
            switch(Level_01[y][x])
            {
            case 0:
                BrickColor = ECS_Background;
                break;
            case 1:
                BrickColor = ECS_DefaultBreak_01;
                break;
            case 2:
                BrickColor = ECS_DefaultBreak_02;
                break;
            }
            
            DrawBrick(hdc, BrickColor, (x * 16) + FieldPadding, (y * 8) + FieldPadding);
        }
}

//--------Platform Drawing Function----------------------------

void QSEngine::DrawPlatform(HDC hdc)
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

void QSEngine::MovePlatfom()
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

/*void DrawDirection(HDC hdc, int direction)
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
    
}*/

void QSEngine::DrawBall(HDC hdc)
{
    SetPenBrushColor(hdc, ECS_Background);
    Ellipse(hdc, PrewBallRect.left, PrewBallRect.top, PrewBallRect.right, PrewBallRect.bottom); // Clear Ball
    
    SetPenBrushColor(hdc, ECS_White);
    Ellipse(hdc, BallRect.left, BallRect.top, BallRect.right, BallRect.bottom); // Ball Drawing
}

void QSEngine::MoveBall()
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
        } else
            for(int x = 0; x <= 11; x++)
                for(int y = 13; y >= 0; y--)
                {

                	
                    if(!Level_01[y][x])
                        continue;
                    
                    if(NextBallY <= ((y + 1) * DefaultBrickHeight) and (NextBallX + BallScale / 2) >= (x + 1) * DefaultBrickWidth and (NextBallX + BallScale / 2) <= (x + 2) * DefaultBrickWidth)
                    {    
                        NextBallY = ((y + 1) * DefaultBrickHeight) - (NextBallY - ((y + 1) * DefaultBrickHeight));
                        BallDirection = -BallDirection;
                        Level_01[y][x+1] = 0;
                        
                        cout << (x + 1) * DefaultBrickWidth << ' ' << NextBallX << endl;
                        InvalidateRect(Hwnd, &LevelRect, FALSE);
                    }
                    
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

/*BITMAP BmImage; // BitMap Image
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
}*/

//--------Window Drawing Function------------------------------

void QSEngine::DrawFrame(HDC hdc, RECT &area)
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

void QSEngine::GameControl(EKeyType key)
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

void QSEngine::OnTick()
{
    MoveBall();
}