#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "raylib.h"
#include "constants.h"
#include "spoton.h"

#define MAX_LEVEL 3

static void InitGame(void);   // Initialize game
static void UpdateGame(void); // Update game (one frame)
static void DrawGame(void);   // Draw game (one frame)
static void UnloadGame(void); // Unload game

int main(void)
{
    // initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    InitGame();
    SetTargetFPS(FPS);

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
    SetupNewLevelState();
}

void UpdateGame(void)
{
    if (!gameOver)
    {
        if (!waitNextLevel)
        {
            for (int i = 0; i < numCircles; i++)
            {

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    puts("click buttton");
                    Vector2 clickPosition = GetMousePosition();

                    if (CheckCollisionPointCircle(clickPosition, spots[i].position, circleRadius))
                    {
                        removeSpotIndex = i;
                    }
                }

                // top side
                if (CheckCollisionCircleRec(spots[i].position, circleRadius, (Rectangle){50, 50, 700, 20}))
                {
                    spots[i].sign = GetRandomValue(0, 1);
                    switch (spots[i].sign)
                    {
                    case 0:
                        spots[i].ballSide = 1;
                        break;
                    case 1:
                        spots[i].ballSide = 2;
                        break;
                    }
                } // right side
                else if (CheckCollisionCircleRec(spots[i].position, circleRadius, (Rectangle){730, 70, 20, 310}))
                {
                    spots[i].sign = GetRandomValue(0, 1);
                    switch (spots[i].sign)
                    {
                    case 0:
                        spots[i].ballSide = 4;
                        break;
                    case 1:
                        spots[i].ballSide = 1;
                        break;
                    }
                } // bottom side
                else if (CheckCollisionCircleRec(spots[i].position, circleRadius, (Rectangle){50, 380, 700, 20}))
                {
                    spots[i].sign = GetRandomValue(0, 1);
                    switch (spots[i].sign)
                    {
                    case 0:
                        spots[i].ballSide = 4;
                        break;
                    case 1:
                        spots[i].ballSide = 3;
                        break;
                    }
                } // left side
                else if (CheckCollisionCircleRec(spots[i].position, circleRadius, (Rectangle){50, 70, 20, 310}))
                {
                    spots[i].sign = GetRandomValue(0, 1);
                    switch (spots[i].sign)
                    {
                    case 0:
                        spots[i].ballSide = 3;
                        break;
                    case 1:
                        spots[i].ballSide = 2;
                        break;
                    }
                }

                switch (spots[i].ballSide)
                {
                case 1:
                    spots[i].position = (Vector2){spots[i].position.x - speed.x, spots[i].position.y + speed.y};
                    break;
                case 2:
                    spots[i].position = (Vector2){spots[i].position.x + speed.x, spots[i].position.y + speed.y};
                    break;
                case 3:
                    spots[i].position = (Vector2){spots[i].position.x + speed.x, spots[i].position.y - speed.y};
                    break;
                case 4:
                    spots[i].position = (Vector2){spots[i].position.x - speed.x, spots[i].position.y - speed.y};
                    break;
                }
                //printf("ballside: %d\n", spots[i].ballSide);
            }

            remainingLevelTime = endLevelTime - (int)GetTime();
            sprintf(gameTimeText, "Time: %02d", remainingLevelTime);
        }

        UpdateSpots();
        removeSpotIndex = -1;

        if ((numCircles == 0 && level == MAX_LEVEL) || (level == MAX_LEVEL && remainingLevelTime == 0))
        {
            gameOver = true;
            sprintf(gameTimeText, "Time: %02d", 0);
        }

        if (((numCircles == 0 && level <= 2) || remainingLevelTime == 0) && !waitNextLevel)
        {
            free(spots);
            spots = NULL;
            waitNextLevel = true;
            sprintf(gameTimeText, "Time: %02d", 0);
            endWaitTime = (int)GetTime() + waitPeriodNextLevel;
        }

        if (waitNextLevel)
        {
            //printf("endtime: %d - time: %d\n", endWaitTime, (int)GetTime());
            waitPeriodNextLevel = endWaitTime - (int)GetTime();
        }

        if (waitPeriodNextLevel == 0 && waitNextLevel)
        {
            startNewLevel();
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            free(spots);
            ResetGame();
        }
    }

    frameCounter++;
}

void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawGameState();

    EndDrawing();
}

void UnloadGame(void)
{
}
