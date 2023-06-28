#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <map>
#include <Windows.h>

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

//--------Ball Class-------------------------------------------

class QBall
{
public:
    QBall(QSEngine *engine);
    
    void DrawBall(HDC hdc); // Ball Drawing Function
    void MoveBall(QSEngine *engine); // Ball Moving Function

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
    RECT BallDirectionRect; // Ball Direction Position

    int BallScale = 4; // Ball Scale
    double BallSpeed = 3.0; // Ball Speed
    int BallDirectionNum = -2; // Ball Direction: <map>StartAngle
    double BallDirection; // Ball Direction: Radian Angle
    double PrewBallDirection; // Previous Ball Direction: Radian Angle

    int BallX; // Ball X-coordinate
    int BallY; // Ball Y-coordinate
};

//--------Engine Class-----------------------------------------

class QSEngine
{
    
public:
    QSEngine();
    
    void EngineInit(HWND hwnd); // Init Engine Function
    void DrawFrame(HDC hdc, RECT &area); // Window Drawing Function
    void GameControl(EKeyType key); // Control Game Function
    void OnTick();
    void MovePlatfom(); // Platform Redrawing Function
    
    static void SetPenBrushColor(HDC hdc, EColorScheme color, int pen_width = 0); // Set Pen And Brush Color Function

    //========Level Variables==================================

    static const int SCALE = 3; // Game Scale
    HWND Hwnd; // Window Handle

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
    
    //========Platform Variables===============================
    
    int PlatformInnerWidth = 21; // Platform Inner Width

    int PlatformWidth = PlatformInnerWidth + 12; // Platform Width
    int PlatformX = (12 * 16) / 2 - PlatformWidth / 2; // Platform X-coordinate
    static const int PlatformY = 150; // Platform Y-coordinate: CONST
    
private:
    void DrawInterface(HDC hdc); // Interface Drawing Function
    void DrawBrick(HDC hdc, EColorScheme color, int x, int y); // Brick Drawing Function
    void DrawLevel(HDC hdc); // Level Drawing Function
    void DrawPlatform(HDC hdc); // Platform Drawing Function
    
    //========Platform Variables===============================

    RECT PlatformRect, PrewPlatformRect; // Platform Position
    
    int PlatformCircleScale = 8; // Platform Circle Scale

    int PlatformStep = 3; // Platform Moving Step
    
};