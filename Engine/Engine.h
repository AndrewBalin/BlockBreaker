#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <map>
#include <Windows.h>

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

//

enum ETimers
{
    ET_1 = WM_USER + 1,
};

//--------Engine Class-----------------------------------------


class QSEngine
{
    
public:
    void EngineInit(HWND hwnd); // Init Engine Function
    void DrawFrame(HDC hdc, RECT &area); // Window Drawing Function
    void GameControl(EKeyType key); // Control Game Function
    void OnTick();
    
private:
    void SetPenBrushColor(HDC hdc, EColorScheme color, int pen_width = 0); // Set Pen And Brush Color Function
    void DrawInterface(HDC hdc); // Interface Drawing Function
    void DrawBrick(HDC hdc, EColorScheme color, int x, int y); // Brick Drawing Function
    void DrawLevel(HDC hdc); // Level Drawing Function
    void DrawPlatform(HDC hdc); // Platform Drawing Function
    void MovePlatfom(); // Platform Redrawing Function
    void DrawBall(HDC hdc); // Ball Drawing Function
    void MoveBall(); // Ball Moving Function

    //========Level Variables==================================

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
    
    //========Platform Variables===============================

    RECT PlatformRect, PrewPlatformRect; // Platform Position

    int PlatformInnerWidth = 21; // Platform Inner Width
    int PlatformCircleScale = 8; // Platform Circle Scale

    int PlatformWidth = PlatformInnerWidth + 12; // Platform Width
    int PlatformX = (12 * 16) / 2 - PlatformWidth / 2; // Platform X-coordinate
    const int PlatformY = 150; // Platform Y-coordinate: CONST

    int PlatformStep = 3; // Platform Moving Step
    
    //========Ball Variables===================================

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
    int BallDirectionNum = -2; // Ball Direction: <map>StartAngle
    double BallDirection; // Ball Direction: Radian Angle
    double PrewBallDirection; // Previous Ball Direction: Radian Angle

    int BallX = PlatformX + PlatformWidth / 2 - 4; // Ball X-coordinate
    int BallY = 148; // Ball Y-coordinate
    
};

