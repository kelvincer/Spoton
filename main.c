#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "raylib.h"

static const int screenWidth = 800;
static const int screenHeight = 450;

static const int fps = 60;
static const int numLives = 5;
static const char *gameName = "SpotOn";
static const int numCircles = 1;
static const int numAngles = 4;
static int score = 100020;
static int hiscore = 200450;
static float alpha = 1.0f;
static int frameCounter = 0;
static int interval = 1;
static Vector2 speed = {3, 4};
static int numCounterChanges = 0;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);   // Initialize game
static void UpdateGame(void); // Update game (one frame)
static void DrawGame(void);   // Draw game (one frame)
static void UnloadGame(void); // Unload game
// static void UpdateDrawFrame(void); // Update and Draw (one frame)

// -----------------------------------------------------------------------------------
void drawLives();
void setLives();
void SetupInitialGameState();
void drawGameState();
int getNextDirection(int matrix[numCircles][3], int index, int i);
Vector2 livesPosition[numLives];
Vector2 spotsPositions[numCircles];
int xSign[numCircles];
int ySign[numCircles];
int xNewPosition[numCircles];
int yNewPosition[numCircles];
float xSpeed[numCircles];
float ySpeed[numCircles];
int ballSide[numCircles] = {0};
int flag[numCircles] = {0};
float direction[numAngles] = {PI / 4, 37 * PI / 180, 16 * PI / 180, 15 * PI / 180};

int main(void)
{
    // initialization
    InitWindow(screenWidth, screenHeight, "Window Title");
    InitGame();
    SetTargetFPS(fps);

    // game loop
    while (!WindowShouldClose())
    {
        UpdateGame(); // update game elements
        DrawGame();   // draw next animation frame
    }

    // cleanup
    UnloadGame();  // release game resources
    CloseWindow(); // close game window

    return 0;
}

void InitGame(void)
{
    SetupInitialGameState();
}

void UpdateGame(void)
{
    if (IsKeyPressed('A'))
    {
        speed.x += 10;
        speed.y += 10;
    }

    if (IsKeyPressed('S'))
    {
        speed.x -= 10;
        speed.y -= 10;
    }

    // int onAdditionX = GetRandomValue(0, 1);
    // int onAdditionY = GetRandomValue(0, 1);

    // if (frameCounter % 120 == 0)
    // {
    //     switch (onAdditionX)
    //     {
    //     case 0:
    //         switch (onAdditionY)
    //         {
    //         case 0:
    //             speed.x -= 0.56f;
    //             speed.y -= 0.56f;
    //             break;
    //         case 1:
    //             speed.x -= 0.56f;
    //             speed.y += 0.56f;
    //             break;
    //         }
    //         break;
    //     case 1:
    //         switch (onAdditionY)
    //         {
    //         case 0:
    //             speed.x += 0.56f;
    //             speed.y -= 0.56f;
    //             break;

    //         case 1:
    //             speed.x += 0.56f;
    //             speed.y += 0.56f;
    //             break;
    //         }
    //         break;
    //     }
    // }

    for (int i = 0; i < numCircles; i++)
    {
        if (frameCounter == 0)
        {
            ballSide[i] = GetRandomValue(1, 4);
        }

        // top side
        if (CheckCollisionCircleRec((Vector2){spotsPositions[i].x, spotsPositions[i].y}, 25, (Rectangle){50, 50, 700, 20}))
        {
            xSign[i] = GetRandomValue(0, 1);
            switch (xSign[i])
            {
            case 0:
                ballSide[i] = 1;
                break;
            case 1:
                ballSide[i] = 2;
                break;
            }
        } // right side
        else if (CheckCollisionCircleRec((Vector2){spotsPositions[i].x, spotsPositions[i].y}, 25, (Rectangle){730, 70, 20, 310}))
        {
            xSign[i] = GetRandomValue(0, 1);
            switch (xSign[i])
            {
            case 0:
                ballSide[i] = 4;
                break;
            case 1:
                ballSide[i] = 1;
                break;
            }
        } // bottom side
        else if (CheckCollisionCircleRec((Vector2){spotsPositions[i].x, spotsPositions[i].y}, 25, (Rectangle){50, 380, 700, 20}))
        {
            xSign[i] = GetRandomValue(0, 1);
            switch (xSign[i])
            {
            case 0:
                ballSide[i] = 4;
                break;
            case 1:
                ballSide[i] = 3;
                break;
            }
        } // left side
        else if (CheckCollisionCircleRec((Vector2){spotsPositions[i].x, spotsPositions[i].y}, 25, (Rectangle){50, 70, 20, 310}))
        {
            xSign[i] = GetRandomValue(0, 1);
            switch (xSign[i])
            {
            case 0:
                ballSide[i] = 3;
                break;
            case 1:
                ballSide[i] = 2;
                break;
            }
        }
        else
        {
            flag[i] = 5;
        }

        switch (ballSide[i])
        {
        case 1:
            spotsPositions[i] = (Vector2){spotsPositions[i].x - speed.x, spotsPositions[i].y + speed.y};
            break;
        case 2:
            spotsPositions[i] = (Vector2){spotsPositions[i].x + speed.x, spotsPositions[i].y + speed.y};
            break;
        case 3:
            spotsPositions[i] = (Vector2){spotsPositions[i].x + speed.x, spotsPositions[i].y - speed.y};
            break;
        case 4:
            spotsPositions[i] = (Vector2){spotsPositions[i].x - speed.x, spotsPositions[i].y - speed.y};
            break;
        }
        printf("ballside: %d\n", ballSide[i]);
    }

    frameCounter++;
}

void DrawGame(void)
{
    BeginDrawing();

    {
        ClearBackground(RAYWHITE);
        drawGameState();
    }

    EndDrawing();
}

void UnloadGame(void)
{
}

void drawLives()
{
    for (int i = 0; i < numLives; i++)
    {
        DrawCircle(livesPosition[i].x, livesPosition[i].y, 10, RED);
    }
}

void setLives()
{
    int space = 0;
    for (int i = 0; i < numLives; i++)
    {
        livesPosition[i].x = 20 + space;
        livesPosition[i].y = 420;
        space += 30;
    }
}

void SetupInitialGameState()
{
    setLives();
    for (int i = 0; i < numCircles; i++)
    {
        spotsPositions[i] = (Vector2){GetRandomValue(70, 730), GetRandomValue(70, 430)};
    }
}

void drawGameState()
{
    drawLives();
    DrawRectangle(50, 50, 700, 350, GRAY);
    DrawRectangle(50, 50, 700, 20, YELLOW);    // top side
    DrawRectangle(50, 380, 700, 20, YELLOW);   // bottom side
    DrawRectangle(50, 70, 20, 310, YELLOW);    // left side
    DrawRectangle(730, 70, 20, 310, YELLOW); // right side
    DrawText(gameName, 10, 10, 10, DARKGRAY);
    DrawText(TextFormat("Level: %08i", score), 15, 30, 20, RED);
    DrawText(TextFormat("Score: %08i", hiscore), 15, 60, 20, GREEN);
    for (int i = 0; i < numCircles; i++)
    {
        if (alpha >= 0)
        {
            DrawCircle(spotsPositions[i].x, spotsPositions[i].y, 25, Fade(BLACK, alpha));
        }
    }
}
