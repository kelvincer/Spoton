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
    InitWindow(screenWidth, screenHeight, windowTitle);
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
    SetupNewLevelState();
}

void UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('A'))
        {
            speed.x += 5;
            speed.y += 5;
        }

        if (IsKeyPressed('S'))
        {
            speed.x -= 3;
            speed.y -= 3;
        }

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
                printf("ballside: %d\n", spots[i].ballSide);
            }

            currentTime = GetTime();
            remainingTime = endTime - currentTime;
            sprintf(gameTimeText, "Time: %02d", remainingTime);
        }

        UpdateSpots();
        removeSpotIndex = -1;

        if ((numCircles == 0 && level == MAX_LEVEL) || (level == MAX_LEVEL && remainingTime == 0))
        {
            gameOver = true;
            sprintf(gameTimeText, "Time: %02d", 0);
        }

        if ((numCircles == 0 && level <= 2) || remainingTime == 0)
        {
            free(spots);
            spots = NULL;
            waitNextLevel = true;
            sprintf(gameTimeText, "Time: %02d", 0);
        }

        if (frameCounter % fps == 0 && waitNextLevel)
        {
            waitPeriodNextLevel--;
        }

        if (waitPeriodNextLevel == -1)
        {
            waitNextLevel = false;
            level++;
            waitPeriodNextLevel = 5;
            SetupNewLevelState();
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
