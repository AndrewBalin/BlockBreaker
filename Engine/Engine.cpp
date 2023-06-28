#include "Engine.h"
#include "Levels/LevelsData.h"

//--------Level Init Function----------------------------------

QSLevel::QSLevel()
:LevelRect({
    QSEngine::FieldPadding * QSEngine::SCALE,
    QSEngine::FieldPadding * QSEngine::SCALE,
    (QSEngine::FieldPadding + (QSEngine::DefaultBrickWidth * 12)) * QSEngine::SCALE,
    (QSEngine::FieldPadding + (QSEngine::DefaultBrickHeight * 14)) * QSEngine::SCALE,
}) {}

void QSLevel::Init()
{
    
}

//--------Brick Drawing Function-------------------------------

void QSLevel::DrawBrick(HDC hdc, EColorScheme color, int x, int y)
{
    QSEngine::SetPenBrushColor(hdc, color);
    RoundRect(hdc,
        x * QSEngine::SCALE,
        y * QSEngine::SCALE,
        (x + QSEngine::DefaultBrickWidth - 1) * QSEngine::SCALE,
        (y + QSEngine::DefaultBrickHeight - 1) * QSEngine::SCALE,
        5 * QSEngine::SCALE, 5 * QSEngine::SCALE);
}

//--------Level Drawing Function-------------------------------

void QSLevel::Draw(HDC hdc, RECT &area)
{

    RECT IntersectionRect;
    EColorScheme BrickColor;
    
    if(!IntersectRect(&IntersectionRect, &area, &LevelRect))
        return;
    
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
            
            DrawBrick(hdc, BrickColor, (x * 16) + QSEngine::FieldPadding, (y * 8) + QSEngine::FieldPadding);
        }
}

//-------------------------------------------------------------




//--------Platform Init Function-------------------------------

QSPlatform::QSPlatform()
{}

void QSPlatform::Init()
{}

//--------Platform Draw Function-------------------------------

void QSPlatform::Draw(const HDC hdc, const RECT &area) const
{

    RECT IntersectionRect;
    
    if(!IntersectRect(&IntersectionRect, &area, &PlatformRect))
        return;
    
    QSEngine::SetPenBrushColor(hdc, ECS_Background);
    Rectangle(hdc, PrewPlatformRect.left, PrewPlatformRect.top, PrewPlatformRect.right, PrewPlatformRect.bottom); // Clear Platform
    
    QSEngine::SetPenBrushColor(hdc, ECS_DefaultBreak_01);
    Ellipse(hdc, PlatformRect.left,
        PlatformRect.top,
        (PlatformX + QSEngine::FieldPadding + PlatformCircleScale) * QSEngine::SCALE,
        (PlatformY + QSEngine::FieldPadding + PlatformCircleScale) * QSEngine::SCALE); // Left Ball Drawing
    Ellipse(hdc,
        (PlatformX + PlatformInnerWidth + QSEngine::FieldPadding) * QSEngine::SCALE, (PlatformY + QSEngine::FieldPadding) * QSEngine::SCALE,
        (PlatformX + PlatformInnerWidth + QSEngine::FieldPadding + PlatformCircleScale) * QSEngine::SCALE,
        (PlatformY + QSEngine::FieldPadding + PlatformCircleScale) * QSEngine::SCALE); // Right Ball Drawing
    
    QSEngine::SetPenBrushColor(hdc, ECS_DefaultBreak_02);
    RoundRect(hdc,
        (PlatformX + QSEngine::FieldPadding + PlatformCircleScale / 2) * QSEngine::SCALE,
        (PlatformY + QSEngine::FieldPadding + 2) * QSEngine::SCALE, (PlatformX + PlatformWidth) * QSEngine::SCALE,
        (PlatformY + QSEngine::FieldPadding + 6) * QSEngine::SCALE, 5 * QSEngine::SCALE, 5 * QSEngine::SCALE); // Platform Rectangle Drawing
    
    QSEngine::SetPenBrushColor(hdc, ECS_White, 3);
    Arc(hdc,
        (PlatformX + QSEngine::FieldPadding + 2) * QSEngine::SCALE, (PlatformY + QSEngine::FieldPadding + 1) * QSEngine::SCALE,
        (PlatformX + QSEngine::FieldPadding + 7) * QSEngine::SCALE, (PlatformY + QSEngine::FieldPadding + 6) * QSEngine::SCALE,
        (PlatformX + QSEngine::FieldPadding + 2) * QSEngine::SCALE, (PlatformY + QSEngine::FieldPadding + 1) * QSEngine::SCALE,
        (PlatformX + QSEngine::FieldPadding + 1) * QSEngine::SCALE,
        (PlatformY + QSEngine::FieldPadding + 4) * QSEngine::SCALE); // Ball Highlight Drawing
    
    QSEngine::SetPenBrushColor(hdc, ECS_White);
    Rectangle(hdc,
        (PlatformX + QSEngine::FieldPadding + 7) * QSEngine::SCALE,
        (PlatformY + QSEngine::FieldPadding + 3) * QSEngine::SCALE,                   
        (PlatformX + PlatformInnerWidth + QSEngine::FieldPadding - 5) * QSEngine::SCALE,
        (PlatformY + QSEngine::FieldPadding + PlatformCircleScale / 2) * QSEngine::SCALE);             
    Rectangle(hdc,
        (PlatformX + PlatformInnerWidth + QSEngine::FieldPadding - 3) * QSEngine::SCALE,
        (PlatformY + QSEngine::FieldPadding + 3) * QSEngine::SCALE,    
        (PlatformX + PlatformInnerWidth + QSEngine::FieldPadding - 1) * QSEngine::SCALE,
        (PlatformY + QSEngine::FieldPadding + PlatformCircleScale / 2) * QSEngine::SCALE); // Rectangle Highlight Drawing

}

//--------Platform Move Function-------------------------------

void QSPlatform::Move(QSEngine *engine, QBall *ball)
{
    PrewPlatformRect = PlatformRect;
    
    PlatformRect = {
        (PlatformX + QSEngine::FieldPadding) * QSEngine::SCALE,
        (PlatformY + QSEngine::FieldPadding) * QSEngine::SCALE,
        (PlatformX + QSEngine::FieldPadding + PlatformWidth) * QSEngine::SCALE,
        (PlatformY + QSEngine::FieldPadding + 8) * QSEngine::SCALE,
    };

    if(!engine->GameStarted)
    {
        ball->BallX = PlatformX + PlatformWidth / 2 - 4;
    }
    
    InvalidateRect(engine->Hwnd, &PrewPlatformRect, FALSE); // Clear Platform
    InvalidateRect(engine->Hwnd, &PlatformRect, FALSE); // Redaraw Platform
}

//-------------------------------------------------------------




//--------Ball Init Function-----------------------------------

QBall::QBall(QSEngine *engine, QSPlatform *platform)
: BallX(platform->PlatformX + platform->PlatformWidth / 2 - 4),
  BallY(148), BallDirectionRect({
      8 * QSEngine::SCALE,
      426,
      (16 * 12 - 8) * QSEngine::SCALE,
      471,

  }){}

void QBall::Init()
{
    BallRect = {
        (BallX + QSEngine::FieldPadding) * QSEngine::SCALE,
        (BallY + QSEngine::FieldPadding) * QSEngine::SCALE,
        (BallX + QSEngine::FieldPadding + BallScale) * QSEngine::SCALE,
        (BallY + QSEngine::FieldPadding + BallScale) * QSEngine::SCALE,
    };

    BallDirection = StartAngle[BallDirectionNum];
    PrewBallDirection = BallDirection;
}

//--------Ball Drawing Function--------------------------------

void QBall::Draw(HDC hdc, RECT &area)
{
    RECT IntersectionRect;

    if(!IntersectRect(&IntersectionRect, &area, &BallRect))
        return;
    
    QSEngine::SetPenBrushColor(hdc, ECS_Background);
    Ellipse(hdc, PrewBallRect.left, PrewBallRect.top, PrewBallRect.right, PrewBallRect.bottom); // Clear Ball
    
    QSEngine::SetPenBrushColor(hdc, ECS_White);
    Ellipse(hdc, BallRect.left, BallRect.top, BallRect.right, BallRect.bottom); // Ball Drawing
}

//--------Ball Moving Function---------------------------------

void QBall::Move(QSEngine *engine, QSPlatform *platform)
{
    PrewBallRect = BallRect;    

    if(engine->GameStarted)
    {
        int NextBallX = BallX + static_cast<int>(cos(BallDirection) * BallSpeed);
        int NextBallY = BallY - static_cast<int>(sin(BallDirection) * BallSpeed);
        
        BallX += static_cast<int>(cos(BallDirection) * BallSpeed);
        BallY -= static_cast<int>(sin(BallDirection) * BallSpeed);

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
        } else if(NextBallY >= 148 and NextBallY <= 149  and NextBallX >= platform->PlatformX and NextBallX <= (platform->PlatformX + platform->PlatformWidth))
        {
            NextBallY = 148 - (NextBallY - 148);
            BallDirection = -BallDirection;
        } else
            for(int x = 0; x <= 11; x++)
                for(int y = 13; y >= 0; y--)
                {
                    if(!Level_01[y][x])
                        continue;
                    
                    if(NextBallY <= ((y + 1) * QSEngine::DefaultBrickHeight) and (NextBallX + BallScale / 2) >= (x + 1)
                        * QSEngine::DefaultBrickWidth and (NextBallX + BallScale / 2) <= (x + 2) * QSEngine::DefaultBrickWidth)
                    {    
                        NextBallY = ((y + 1) * QSEngine::DefaultBrickHeight) - (NextBallY - ((y + 1) * QSEngine::DefaultBrickHeight));
                        BallDirection = -BallDirection;
                        Level_01[y][x+1] = 0;
                        
                        cout << (x + 1) * QSEngine::DefaultBrickWidth << ' ' << NextBallX << endl;
                        InvalidateRect(engine->Hwnd, &engine->LevelRect, FALSE);
                    }
                }
        BallX = NextBallX;
        BallY = NextBallY;
    }
    
    BallRect = {
        (BallX + QSEngine::FieldPadding) * QSEngine::SCALE,
        (BallY + QSEngine::FieldPadding) * QSEngine::SCALE,
        (BallX + QSEngine::FieldPadding + BallScale) * QSEngine::SCALE,
        (BallY + QSEngine::FieldPadding + BallScale) * QSEngine::SCALE,
    };

    InvalidateRect(engine->Hwnd, &PrewBallRect, FALSE); // Clear Ball
    InvalidateRect(engine->Hwnd, &BallRect, FALSE); // Redraw Ball
    platform->Move(engine, this); // Update Platform
}
//-------------------------------------------------------------




//--------Engine Init Function---------------------------------

QSEngine::QSEngine(): Ball(this, &Platform)
{
    //pass
}

void QSEngine::Init(HWND hwnd)
{
    Hwnd = hwnd;
    Platform.PlatformRect = {
        (Platform.PlatformX + FieldPadding) * SCALE,
        (QSPlatform::PlatformY + FieldPadding) * SCALE,
        (Platform.PlatformX + FieldPadding + Platform.PlatformWidth) * SCALE,
        (QSPlatform::PlatformY + FieldPadding + 8) * SCALE,
    };

    Ball.Init(); // Init Ball
    SetTimer(Hwnd, ET_1, 50, nullptr); // Set Tick Timer
}

//--------Set Pen And Brush Color Function---------------------

HPEN Pen;
HBRUSH Brush;

void QSEngine::SetPenBrushColor(HDC hdc, EColorScheme color, int pen_width)
{
    DeleteObject(Pen); // Clear Pen
    DeleteObject(Brush); // Clear Brush
    
    Pen = CreatePen(PS_SOLID, pen_width, color); // Set Pen
    Brush =  CreateSolidBrush(color); // Set Brush
    
    SelectObject(hdc, Pen); // Select Pen
    SelectObject(hdc, Brush); // Select Brush
}

//--------Interface Drawing Function---------------------------

void QSEngine::DrawInterface(HDC hdc)
{
    SetPenBrushColor(hdc, ECS_White);
    Rectangle(hdc, 6 * SCALE, 6 * SCALE, 7 * SCALE, (200 - 6) * SCALE); // Left Field Border 
    Rectangle(hdc, 6 * SCALE, 6 * SCALE, (12 * 16 + FieldPadding) * SCALE, 7 * SCALE); // Top Field Border
    Rectangle(hdc, (12 * 16 + FieldPadding) * SCALE, 6 * SCALE, (12 * 16 + FieldPadding + 1) * SCALE, (200 - 6) * SCALE); // Right Field Border
}

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
    Level.Draw(hdc, area);
    Platform.Draw(hdc, area);
    Ball.Draw(hdc, area);
    
    DrawInterface(hdc);
}

//--------Control Game Function--------------------------------

void QSEngine::GameControl(EKeyType key)
{
    switch(key)
    {
    case EKT_Left: // Move Left
        if(Platform.PlatformX > 0)
        {
            Platform.PlatformX -= Platform.PlatformStep;
            Platform.Move(this, &Ball);
        }
        break;
    case EKT_Right: // Move Right
        if((Platform.PlatformX + Platform.PlatformInnerWidth + FieldPadding + 8) * SCALE < (12 * 16 + FieldPadding - 1) * SCALE)
        {
            Platform.PlatformX += Platform.PlatformStep;
            Platform.Move(this, &Ball);
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

//--------On Engine Tick Function------------------------------

void QSEngine::OnTick()
{
    Ball.Move(this, &Platform);
}