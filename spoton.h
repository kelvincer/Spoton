#ifndef SPOTON_H
#define SPOTON_H

typedef struct Spot
{
    Vector2 position;
    Color color;
    int sign;
    int ballSide;
} Spot;

int numLives;
Vector2 speed;
const int circleRadius;
int frameCounter;
int level;
int removeSpotIndex;
bool gameOver;
int waitPeriodNextLevel;
bool waitNextLevel;
int numCircles;
Spot *spots;
char gameTimeText[100];
int endLevelTime;
int remainingLevelTime;
int endWaitTime;
int remainingWaitTime;
bool isClickOverRectangle;
int score;

void DrawPlayerLives();
void SetPlayerLives();
void SetupNewLevelState();
void DrawGameState();
void UpdateSpots();
void ResetGame();
void DrawWaitMessage();
void DrawGameTime();
void DrawScoreAndLevel();
void DrawGameOver();
void startNewLevel();
bool isClickInRectangle(Vector2 click);

#endif