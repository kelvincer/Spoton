#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "constants.h"
#include "spoton.h"

static const int numLives = 5;
const int circleRadius = 25;
const int levelPlayingTime = 8;
int waitPeriodNextLevel = WAIT_PERIOD;
int remainingLevelTime = 0;
bool waitNextLevel = false;
int removeSpotIndex = -1;
int level = 1;
bool gameOver = false;
int numCircles = 3;
Vector2 speed = INITIAL_SPEED;
int frameCounter = 0;
static int score = 0;
static Vector2 livesPosition[numLives];

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

    endLevelTime = (int)GetTime() + levelPlayingTime;
}

void DrawGameState()
{
    DrawPlayerLives();
    DrawRectangle(50, 50, 700, 20, YELLOW);  // top side
    DrawRectangle(50, 380, 700, 20, YELLOW); // bottom side
    DrawRectangle(50, 70, 20, 310, YELLOW);  // left side
    DrawRectangle(730, 70, 20, 310, YELLOW); // right side
    DrawText(GAME_NAME, 10, 10, 15, DARKGRAY);
    DrawScoreAndLevel();
    DrawGameTime();

    if (gameOver)
    {
        DrawGameOver();
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
    waitPeriodNextLevel = WAIT_PERIOD;
    speed = (Vector2)INITIAL_SPEED;
    SetupNewLevelState();
}

void DrawWaitMessage()
{
    char textWait[50];
    sprintf(textWait, "NEXT LEVEL IN %d MINUTES", waitPeriodNextLevel);
    int fontSize = 30;
    DrawText(textWait,
             SCREEN_WIDTH / 2 - MeasureText(textWait, fontSize) / 2,
             SCREEN_HEIGHT / 2 - fontSize / 2, fontSize, GRAY);
}

void DrawGameTime()
{
    int fontSize = 25;
    DrawText(gameTimeText, SCREEN_WIDTH - MeasureText(gameTimeText, fontSize) - 15, 15, fontSize, GRAY);
}

void DrawScoreAndLevel()
{
    int fontSize = 16;
    DrawText(TextFormat("Level: %i", level), 10, 30, fontSize, BLACK);
    DrawText(TextFormat("Score: %i", score), 25 + MeasureText("Level: 0", fontSize), 30, fontSize, BLACK);
}

void DrawGameOver()
{
    char scoreText[20];
    int scoreTextSize = 30;
    int playAgainTextSize = 16;
    sprintf(scoreText, "TOTAL SCORE: %d", score);
    DrawText(scoreText,
             SCREEN_WIDTH / 2 - MeasureText(scoreText, scoreTextSize) / 2,
             (SCREEN_HEIGHT - scoreTextSize - playAgainTextSize - 15) / 2, scoreTextSize, GREEN);
    char *gameOverText = "PRESS ENTER TO PLAY AGAIN";
    DrawText(gameOverText,
             SCREEN_WIDTH / 2 - MeasureText(gameOverText, playAgainTextSize) / 2,
             (SCREEN_HEIGHT - scoreTextSize - playAgainTextSize - 15) / 2 + scoreTextSize + 15 , playAgainTextSize, GRAY);
}

void startNewLevel()
{
    waitNextLevel = false;
    level++;
    waitPeriodNextLevel = WAIT_PERIOD;
    speed.x += 1.5;
    speed.y += 1.5;
    SetupNewLevelState();
}
