#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "raylib.h"

typedef struct Spot
{
    Vector2 position;
    Color color;
    int sign;
    int ballSide;
} Spot;

static const int screenWidth = 800;
static const int screenHeight = 450;

static const int fps = 60;
static const int numLives = 5;
static const char *gameName = "SpotOn";
static int numCircles = 3;
static const int circleRadius = 25;
static const int numAngles = 4;
static int score = 0;
static float alpha = 1.0f;
static int frameCounter = 0;
static int interval = 1;
static Vector2 speed = {3 / 2, 4 / 2};
static int level = 1;
static int removeSpotIndex = -1;
bool gameOver = false;
bool waitNextLevel = false;
static int waitPeriodNextLevel = 5;
static char gameTime[100];
int currentTime;
int endTime;
int levelPlayingTime = 5;
int remainingTime = 0;
const int MAX_LEVEL = 3;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);   // Initialize game
static void UpdateGame(void); // Update game (one frame)
static void DrawGame(void);   // Draw game (one frame)
static void UnloadGame(void); // Unload game
// static void UpdateDrawFrame(void); // Update and Draw (one frame)

// -----------------------------------------------------------------------------------
void DrawPlayerLives();
void SetPlayerLives();
void SetupNewLevelState();
void DrawGameState();
void UpdateSpots();
void ResetGame();
void DrawWaitMessage();
void DrawGameTime();
Vector2 livesPosition[numLives];
float direction[numAngles] = {PI / 4, 37 * PI / 180, 16 * PI / 180, 15 * PI / 180};
Spot *spots = NULL;

int main(void)
{
    // initialization
    InitWindow(screenWidth, screenHeight, "SPOTON");
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
            sprintf(gameTime, "Time: %02d", remainingTime);
        }

        UpdateSpots();
        removeSpotIndex = -1;

        if ((numCircles == 0 && level == MAX_LEVEL) || (level == MAX_LEVEL && remainingTime == 0))
        {
            gameOver = true;
            sprintf(gameTime, "Time: %02d", 0);
        }

        if ((numCircles == 0 && level <= 2) || remainingTime == 0)
        {
            free(spots);
            spots = NULL;
            waitNextLevel = true;
            sprintf(gameTime, "Time: %02d", 0);
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

void DrawPlayerLives()
{
    for (int i = 0; i < numLives; i++)
    {
        DrawCircle(livesPosition[i].x, livesPosition[i].y, 10, RED);
    }
}

void SetPlayerLives()
{
    int space = 0;
    for (int i = 0; i < numLives; i++)
    {
        livesPosition[i].x = 20 + space;
        livesPosition[i].y = 420;
        space += 30;
    }
}

void SetupNewLevelState()
{
    numCircles = 3;
    SetPlayerLives();
    Color colors[] = {BLUE, ORANGE, RED};

    spots = calloc(numCircles, sizeof(Spot));

    for (int i = 0; i < numCircles; i++)
    {
        spots[i].position = (Vector2){GetRandomValue(70, 730), GetRandomValue(70, 380)};
        spots[i].color = colors[i];
        spots[i].ballSide = GetRandomValue(1, 4);
    }

    endTime = GetTime() + levelPlayingTime;
}

void DrawGameState()
{
    DrawPlayerLives();
    // DrawRectangle(50, 50, 700, 350, GRAY);
    DrawRectangle(50, 50, 700, 20, YELLOW);  // top side
    DrawRectangle(50, 380, 700, 20, YELLOW); // bottom side
    DrawRectangle(50, 70, 20, 310, YELLOW);  // left side
    DrawRectangle(730, 70, 20, 310, YELLOW); // right side
    DrawText(gameName, 10, 10, 15, DARKGRAY);
    DrawText(TextFormat("Level: %i", level), 15, 30, 16, BLACK);
    DrawText(TextFormat("Score: %i", score), 15, 60, 16, BLACK);
    DrawGameTime();

    if (gameOver)
    {
        char *textGameOver = "PRESS ENTER TO PLAY AGAIN";
        int fontSize = 30;
        DrawText(textGameOver,
                 screenWidth / 2 - MeasureText(textGameOver, fontSize) / 2,
                 screenHeight / 2 - fontSize / 2, fontSize, GRAY);
    }
    else
    {
        if (waitNextLevel)
        {
            DrawWaitMessage();
        }
        else
        {
            for (int i = 0; i < numCircles; i++)
            {
                DrawCircle(spots[i].position.x, spots[i].position.y, 25, spots[i].color);
            }
        }
    }
}

void UpdateSpots()
{
    if (removeSpotIndex != -1)
    {
        numCircles -= 1;

        score += 10;

        if (numCircles == 0)
        {
            return;
        }

        Spot *localSpots = calloc(numCircles, sizeof(Spot));
        int j = 0;
        for (int i = 0; i < numCircles + 1; i++)
        {
            if (removeSpotIndex != i)
            {
                localSpots[j] = spots[i];
                j++;
            }
        }

        free(spots);
        spots = localSpots;
    }
}

void ResetGame()
{
    gameOver = false;
    level = 1;
    score = 0;
    waitNextLevel = false;
    SetupNewLevelState();
}

void DrawWaitMessage()
{
    char textWait[50];
    sprintf(textWait, "NEXT LEVEL IN %d MINUTES", waitPeriodNextLevel);
    int fontSize = 30;
    DrawText(textWait,
             screenWidth / 2 - MeasureText(textWait, fontSize) / 2,
             screenHeight / 2 - fontSize / 2, fontSize, GRAY);
}

void DrawGameTime()
{
    int fontSize = 25;
    DrawText(gameTime, screenWidth - MeasureText(gameTime, fontSize) - 15, 15, fontSize, GRAY);
}
