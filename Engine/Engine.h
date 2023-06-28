#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <map>
#include <Windows.h>

class QBall;
class QSEngine;
using namespace std;

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

enum ETimers
{
    ET_1 = WM_USER + 1,
};

//--------Brick Class------------------------------------------

class QBrick
{
public:
    void Draw();
};

//--------Level Class------------------------------------------

class QSLevel
{
public:
    QSLevel();

    void Init(); // Level Init Function
    void DrawBrick(HDC hdc, EColorScheme color, int x, int y); // Brick Drawing Function
    void Draw(HDC hdc, RECT &area); // Level Drawing Function

    RECT LevelRect{}; // Bricks Position
};

//--------Platform Class---------------------------------------

class QSPlatform
{
public:
    QSPlatform();

    void Init(); // Platform Init Function
    void Draw(HDC hdc, const RECT &area) const; // Platform Drawing Function
    void Move(QSEngine *engine, QBall *ball); // Platform Redrawing Function

    RECT PlatformRect{};
    
    int PlatformInnerWidth = 21; // Platform Inner Width

    int PlatformWidth = PlatformInnerWidth + 12; // Platform Width
    int PlatformX = (12 * 16) / 2 - PlatformWidth / 2; // Platform X-coordinate
    static const int PlatformY = 150; // Platform Y-coordinate: CONST

    int PlatformStep = 3; // Platform Moving Step

private:
    RECT PrewPlatformRect{}; // Platform Position
    int PlatformCircleScale = 8; // Platform Circle Scale
};

//--------Ball Class-------------------------------------------

class QBall
{
public:
    QBall(QSEngine *engine, QSPlatform *platform);

    RECT BallRect{};
    
    void Init();
    void Draw(HDC hdc, RECT &area); // Ball Drawing Function
    void Move(QSEngine *engine, QSPlatform *platform); // Ball Moving Function
    
    int BallDirectionNum = -2; // Ball Direction: <map>StartAngle
    
    int BallX; // Ball X-coordinate
    int BallY; // Ball Y-coordinate
    
private:
    std::map<int, double> StartAngle = {
        {-3, 5 * M_PI / 6},
        {-2, 3 * M_PI / 4},
        {-1, 2 * M_PI / 3},
        {0, M_PI / 2},
        {1, M_PI / 3},
        {2, M_PI / 4},
        {3, M_PI / 6},
    };
    double BallDirection{}; // Ball Direction: Radian Angle
    double PrewBallDirection{}; // Previous Ball Direction: Radian Angle

    RECT BallDirectionRect; // Ball Direction Position
    RECT PrewBallRect{}; // Ball Position

    int BallScale = 4; // Ball Scale
    double BallSpeed = 3.0; // Ball Speed
    
};

//--------Engine Class-----------------------------------------

class QSEngine
{
    
public:
    QSEngine();
    
    void Init(HWND hwnd); // Init Engine Function
    void DrawFrame(HDC hdc, RECT &area); // Window Drawing Function
    void GameControl(EKeyType key); // Control Game Function
    void OnTick();
    
    static void SetPenBrushColor(HDC hdc, EColorScheme color, int pen_width = 0); // Set Pen And Brush Color Function

    //========Level Variables==================================

    static const int SCALE = 3; // Game Scale
    HWND Hwnd{}; // Window Handle

    QSLevel Level; // Level
    QSPlatform Platform; // Platform
    QBall Ball; // Ball
    
    bool GameStarted = false; // Level State

    static const int FieldPadding = 8; // Padding Behind Window And Field
    static const int DefaultBrickWidth = 16; // Default Single Brick Width
    static const int DefaultBrickHeight = 8; // Default Single Brick Height
    
    RECT LevelRect = {
        FieldPadding * SCALE,
        FieldPadding * SCALE,
        (FieldPadding + (DefaultBrickWidth * 12)) * SCALE,
        (FieldPadding + (DefaultBrickHeight * 14)) * SCALE,
    
    }; // Bricks Position
    
    
private:
    static void DrawInterface(HDC hdc); // Interface Drawing Function
    static void DrawLevel(HDC hdc); // Level Drawing Function
        
};