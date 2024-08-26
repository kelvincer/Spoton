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
static int numCircles = 3;
static const int circleRadius = 25;
static const int numAngles = 4;
static int score = 0;
static float alpha = 1.0f;
static int frameCounter = 0;
static int interval = 1;
static Vector2 speed = {3 / 2, 4 / 2};
static int levels = 1;
static int removeSpotIndex = -1;

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
void SetupInitialGameState();
void DrawGameState();
void ResizeArrays();
Vector2 livesPosition[numLives];
Vector2 *spotsPositions = NULL;
int *sign = NULL;
int *ballSide = NULL;
float direction[numAngles] = {PI / 4, 37 * PI / 180, 16 * PI / 180, 15 * PI / 180};
Color ballsColor[] = {BLUE, ORANGE, RED};

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
        speed.x += 5;
        speed.y += 5;
    }

    if (IsKeyPressed('S'))
    {
        speed.x -= 3;
        speed.y -= 3;
    }

    for (int i = 0; i < numCircles; i++)
    {

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            puts("click buttton");
            Vector2 clickPosition = GetMousePosition();

            if (CheckCollisionPointCircle(clickPosition, (Vector2){spotsPositions[i].x, spotsPositions[i].y}, circleRadius))
            {
                removeSpotIndex = i;
            }
        }

        if (frameCounter == 0)
        {
            ballSide[i] = GetRandomValue(1, 4);
        }

        // top side
        if (CheckCollisionCircleRec((Vector2){spotsPositions[i].x, spotsPositions[i].y}, circleRadius, (Rectangle){50, 50, 700, 20}))
        {
            sign[i] = GetRandomValue(0, 1);
            switch (sign[i])
            {
            case 0:
                ballSide[i] = 1;
                break;
            case 1:
                ballSide[i] = 2;
                break;
            }
        } // right side
        else if (CheckCollisionCircleRec((Vector2){spotsPositions[i].x, spotsPositions[i].y}, circleRadius, (Rectangle){730, 70, 20, 310}))
        {
            sign[i] = GetRandomValue(0, 1);
            switch (sign[i])
            {
            case 0:
                ballSide[i] = 4;
                break;
            case 1:
                ballSide[i] = 1;
                break;
            }
        } // bottom side
        else if (CheckCollisionCircleRec((Vector2){spotsPositions[i].x, spotsPositions[i].y}, circleRadius, (Rectangle){50, 380, 700, 20}))
        {
            sign[i] = GetRandomValue(0, 1);
            switch (sign[i])
            {
            case 0:
                ballSide[i] = 4;
                break;
            case 1:
                ballSide[i] = 3;
                break;
            }
        } // left side
        else if (CheckCollisionCircleRec((Vector2){spotsPositions[i].x, spotsPositions[i].y}, circleRadius, (Rectangle){50, 70, 20, 310}))
        {
            sign[i] = GetRandomValue(0, 1);
            switch (sign[i])
            {
            case 0:
                ballSide[i] = 3;
                break;
            case 1:
                ballSide[i] = 2;
                break;
            }
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

    ResizeArrays();
    removeSpotIndex = -1;
    frameCounter++;
}

void DrawGame(void)
{
    BeginDrawing();

    {
        ClearBackground(RAYWHITE);
        DrawGameState();
    }

    EndDrawing();
}

void UnloadGame(void)
{
    // free(sign);
    // free(spotsPositions);
    // free(ballSide);
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

void SetupInitialGameState()
{
    sign = calloc(numCircles, sizeof(int));
    spotsPositions = calloc(numCircles, sizeof(Vector2));
    ballSide = calloc(numCircles, sizeof(int));
    SetPlayerLives();
    for (int i = 0; i < numCircles; i++)
    {
        spotsPositions[i] = (Vector2){GetRandomValue(70, 730), GetRandomValue(70, 380)};
    }
}

void DrawGameState()
{
    DrawPlayerLives();
    // DrawRectangle(50, 50, 700, 350, GRAY);
    DrawRectangle(50, 50, 700, 20, YELLOW);  // top side
    DrawRectangle(50, 380, 700, 20, YELLOW); // bottom side
    DrawRectangle(50, 70, 20, 310, YELLOW);  // left side
    DrawRectangle(730, 70, 20, 310, YELLOW); // right side
    DrawText(gameName, 10, 10, 10, DARKGRAY);
    DrawText(TextFormat("Level: %i", levels), 15, 30, 16, BLACK);
    DrawText(TextFormat("Score: %i", score), 15, 60, 16, BLACK);
    for (int i = 0; i < numCircles; i++)
    {
        DrawCircle(spotsPositions[i].x, spotsPositions[i].y, 25, ballsColor[i]);
    }
}

void ResizeArrays()
{
    if (removeSpotIndex != -1)
    {
        numCircles -= 1;

        score += 10;

        if (numCircles == 0)
        {
            return;
        }

        int *localSign = calloc(numCircles, sizeof(int));
        Vector2 *localSpotsPositions = calloc(numCircles, sizeof(Vector2));
        int *localBallSide = calloc(numCircles, sizeof(int));
        int j = 0;
        for (int i = 0; i < numCircles + 1; i++)
        {
            if (removeSpotIndex != i)
            {
                localSpotsPositions[j] = spotsPositions[i];
                j++;
            }
        }
        j = 0;
        for (int i = 0; i < numCircles + 1; i++)
        {
            if (removeSpotIndex != i)
            {
                localSign[j] = sign[i];
                j++;
            }
        }
        j = 0;
        for (int i = 0; i < numCircles + 1; i++)
        {
            if (removeSpotIndex != i)
            {
                localBallSide[j] = ballSide[i];
                j++;
            }
        }
        
        free(sign);
        free(spotsPositions);
        free(ballSide);

        sign = localSign;
        spotsPositions = localSpotsPositions;
        ballSide = localBallSide;
    }
}
