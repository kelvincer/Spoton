#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

static const int screenWidth = 800;
static const int screenHeight = 450;

static const int fps = 60;
static const int numLives = 5;
static const char *gameName = "SpotOn";
static const int numCircles = 3;
static int score = 100020;
static int hiscore = 200450;
static float alpha = 1.0f;
static int frameCounter = 0;
static int interval = 1;
static Vector2 speed = {3, 4};

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
Vector2 livesPosition[numLives];
Vector2 spotsPositions[numCircles];
int xSign[numCircles];
int ySign[numCircles];
int xNewPosition[numCircles];
int yNewPosition[numCircles];
float xSpeed[numCircles];
float ySpeed[numCircles];
int ballSide[numCircles] = {0};

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

static const int movementTime = 8;

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
        if (frameCounter % 120 == 0)
        {
            xSign[i] = GetRandomValue(0, 1);
        }

        if (CheckCollisionCircleRec((Vector2){spotsPositions[i].x, spotsPositions[i].y}, 25, (Rectangle){50.0, 50.0, 50.0, 50.0}))
        {
            spotsPositions[i] = (Vector2){spotsPositions[i].x + speed.x, spotsPositions[i].y + speed.y};
            ballSide[i] = 4;
        }

        if (CheckCollisionCircleRec((Vector2){spotsPositions[i].x, spotsPositions[i].y}, 25, (Rectangle){700.0, 50.0, 50.0, 50.0}))
        {
            spotsPositions[i] = (Vector2){spotsPositions[i].x - speed.x, spotsPositions[i].y + speed.y};
            ballSide[i] = 8;
        }

        if (CheckCollisionCircleRec((Vector2){spotsPositions[i].x, spotsPositions[i].y}, 25, (Rectangle){50.0, 350.0, 50.0, 50.0}))
        {
            spotsPositions[i] = (Vector2){spotsPositions[i].x + speed.x, spotsPositions[i].y - speed.y};
            ballSide[i] = 6;
        }

        if (CheckCollisionCircleRec((Vector2){spotsPositions[i].x, spotsPositions[i].y}, 25, (Rectangle){700.0, 350.0, 50.0, 50.0}))
        {
            spotsPositions[i] = (Vector2){spotsPositions[i].x - speed.x, spotsPositions[i].y - speed.y};
            ballSide[i] = 5;
        }

        if (spotsPositions[i].y <= 50)
        {
            switch (xSign[i])
            {
            case 0:
                spotsPositions[i] = (Vector2){spotsPositions[i].x - speed.x, spotsPositions[i].y + speed.y};
                ballSide[i] = 1;
                break;
            case 1:
                spotsPositions[i] = (Vector2){spotsPositions[i].x + speed.x, spotsPositions[i].y + speed.y};
                ballSide[i] = 2;
                break;
            }
        }
        else if (spotsPositions[i].x <= 50)
        {
            switch (xSign[i])
            {
            case 0:
                spotsPositions[i] = (Vector2){spotsPositions[i].x + speed.x, spotsPositions[i].y - speed.y};
                ballSide[i] = 3;
                break;
            case 1:
                spotsPositions[i] = (Vector2){spotsPositions[i].x + speed.x, spotsPositions[i].y + speed.y};
                ballSide[i] = 4;
                break;
            }
        }
        else if (spotsPositions[i].y >= 400)
        {
            switch (xSign[i])
            {
            case 0:
                spotsPositions[i] = (Vector2){spotsPositions[i].x - speed.x, spotsPositions[i].y - speed.y};
                ballSide[i] = 5;
                break;
            case 1:
                spotsPositions[i] = (Vector2){spotsPositions[i].x + speed.x, spotsPositions[i].y - speed.y};
                ballSide[i] = 6;
                break;
            }
        }
        else if (spotsPositions[i].x >= 750)
        {
            switch (xSign[i])
            {
            case 0:
                spotsPositions[i] = (Vector2){spotsPositions[i].x - speed.x, spotsPositions[i].y - speed.y};
                ballSide[i] = 7;
                break;
            case 1:
                spotsPositions[i] = (Vector2){spotsPositions[i].x - speed.x, spotsPositions[i].y + speed.y};
                ballSide[i] = 8;
                break;
            }
        }
        else
        {
            if (frameCounter % 60 == 0)
            {
                // ballSide[i] = GetRandomValue(0, 8);
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
                spotsPositions[i] = (Vector2){spotsPositions[i].x + speed.x, spotsPositions[i].y + speed.y};
                break;
            case 5:
                spotsPositions[i] = (Vector2){spotsPositions[i].x - speed.x, spotsPositions[i].y - speed.y};
                break;
            case 6:
                spotsPositions[i] = (Vector2){spotsPositions[i].x + speed.x, spotsPositions[i].y - speed.y};
                break;
            case 7:
                spotsPositions[i] = (Vector2){spotsPositions[i].x - speed.x, spotsPositions[i].y - speed.y};
                break;
            case 8:
                spotsPositions[i] = (Vector2){spotsPositions[i].x - speed.x, spotsPositions[i].y + speed.y};
                break;
            default:
                spotsPositions[i] = (Vector2){spotsPositions[i].x + speed.x, spotsPositions[i].y + speed.y};
            }

            printf("ballside: %d\n", ballSide[i]);

            /* switch (xSign[i])
            {
            case 0:
                switch (ySign[i])
                {
                case 0:
                    spotsPositions[i] = (Vector2){spotsPositions[i].x - 2, spotsPositions[i].y - 2};
                    break;
                case 1:
                    spotsPositions[i] = (Vector2){spotsPositions[i].x - 2, spotsPositions[i].y + 2};
                    break;
                }
                break;
            case 1:
                switch (ySign[i])
                {
                case 0:
                    spotsPositions[i] = (Vector2){spotsPositions[i].x + 2, spotsPositions[i].y - 2};
                    break;
                case 1:
                    spotsPositions[i] = (Vector2){spotsPositions[i].x + 2, spotsPositions[i].y + 2};
                    break;
                }
                break;
            } */
        }
    }
    frameCounter++;
}

/* void UpdateGame(void)
{
    if (frameCounter % 1 == 0)
    {
        // alpha -= 1.0f / (60 * movementTime);
    }

    float t = 150.0f; // time of the game (in seconds)
    if (frameCounter <= fps * t)
    {
        for (int i = 0; i < numCircles; i++)
        {
            // spotsPositions[i] = (Vector2){spotsPositions[i].x + distance / (60.0 * t), spotsPositions[i].y + distance / (60.0 * t)};
            if ((frameCounter) % (fps * interval) == 0)
            {
                xSign[i] = GetRandomValue(0, 1); // 0: negative  1: positive
                ySign[i] = GetRandomValue(0, 1);
                // xNewPosition[i] = GetRandomValue(50, 50 + (spotsPositions[i].x - 50) > (750 - spotsPositions[i].x) ? (750 - spotsPositions[i].x) : (spotsPositions[i].x - 50));
                xNewPosition[i] = GetRandomValue(50, 750);
                xSpeed[i] = (xNewPosition[i] - spotsPositions[i].x) / (float)interval;
                // yNewMov[i] = GetRandomValue(10, 80);
                // yNewPosition[i] = GetRandomValue(50, 50 + (spotsPositions[i].y - 50) > (400 - spotsPositions[i].y) ? (400 - spotsPositions[i].y) : (spotsPositions[i].y - 50));
                yNewPosition[i] = GetRandomValue(50, 400);
                ySpeed[i] = (yNewPosition[i] - spotsPositions[i].y) / (float)interval;
                printf("fc= %d, x= %d, y= %d\n", frameCounter, xNewPosition[i], yNewPosition[i]);
            }

            // printf("\nspeed: %f - %f", xSpeed[i], ySpeed[i]);

            spotsPositions[i] = (Vector2){spotsPositions[i].x + xSpeed[i] / fps, spotsPositions[i].y + ySpeed[i] / fps};

             switch (xSign[i])
            {
            case 0:
                switch (ySign[i])
                {
                case 0:
                    spotsPositions[i] = (Vector2){spotsPositions[i].x - xSpeed[i] / fps, spotsPositions[i].y - ySpeed[i] / fps};
                    // spotsPositions[i] = (Vector2){50 + xSpeed[i] / fps, 50 + ySpeed[i] / fps};
                    break;
                case 1:
                    spotsPositions[i] = (Vector2){spotsPositions[i].x - xSpeed[i] / fps, spotsPositions[i].y + ySpeed[i] / fps};
                    break;
                }
                break;
            case 1:
                switch (ySign[i])
                {
                case 0:
                    spotsPositions[i] = (Vector2){spotsPositions[i].x + xSpeed[i] / fps, spotsPositions[i].y - ySpeed[i] / fps};
                    break;
                case 1:
                    spotsPositions[i] = (Vector2){spotsPositions[i].x + xSpeed[i] / fps, spotsPositions[i].y + ySpeed[i] / fps};
                    break;
                }
                break;
            }
        }
    }
    // printf("\nalpha=%.3f - counter=%d", alpha, frameCounter);

    frameCounter++;
}*/

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
        spotsPositions[i] = (Vector2){GetRandomValue(50, 750), GetRandomValue(50, 400)};
    }
}

void drawGameState()
{
    drawLives();
    DrawRectangle(50, 50, 700, 350, GRAY);
    DrawRectangle(50, 50, 700, 20, RED);
    DrawRectangle(50, 380, 700, 20, RED);
    DrawRectangle(50, 70, 20, 310, RED);
    DrawRectangle(730, 70, 20, 310, GREEN);
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
