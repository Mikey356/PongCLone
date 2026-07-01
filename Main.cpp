#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <raylib.h>
#include <string>
#include <cmath>

struct Paddle
{
	Vector2 position = { 0, 0 };
	Vector2 size{ 0, 0 };
	float speed = 0;
	Color color;
};

struct Ball
{
	Vector2 position = { 0, 0 };
	float radius = 0;
	float velocityX = 0;
	float velocityY = 0;
	Color color;
};
int32_t g_screenWidth = 1920 ;
int32_t g_screenHeight = 1080;

Color g_backgroundColor = { 0 };


// Paddles properties
int32_t g_paddleWidth = 15;
int32_t g_paddleHeight = 120;

float speed = 600;
Color col = WHITE;
int counter = 0;
Vector2 position = { g_paddleHeight * 0.4 , g_screenHeight / 2 - g_paddleHeight };
float offset = (float)g_screenWidth * 0.94;
Vector2 position2 = { position.x + offset, position.y };
Vector2 size = { g_paddleWidth,g_paddleHeight };
Paddle g_paddle1 = { position, size, speed, col};
Paddle g_paddle2 = {position2, size, speed, col};

// Ball properties
float velocityX = 1000;
float velocityY = -60;
int centerX = g_screenWidth / 2;
int centerY = g_screenHeight / 2;
float radius = 10;
Color color = RAYWHITE;
Ball g_ball = { centerX, centerY, radius, velocityX, velocityY, color};
int scoreCounter1 = 0;
int scoreCounter2 = 0;
// Title
Color coler = { 135, 60, 190, 255 };

Color alternColor = {coler};

std::ostream& operator<<(std::ostream& out, const Vector2& vec);

void DrawDivider();
void DrawPaddles();
void MovePaddles(float deltaTime);
void MoveBall(float deltaTime);
void HandlePaddleCollisions(Paddle& paddle, Ball& ball, bool& wasColliding);
void InitScore();
void RenderScore(Ball& ball);
void EndGame();
bool gameOver = false;
float gameOverTimer = 9.0f;
// collision related variables
bool wasCollidingPad1 = false;
bool wasCollidingPad2 = false;

int main()
{
	std::cout << "What" << '\n';

	// Initialize window
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(g_screenWidth, g_screenHeight, "90's Pong");
	if (IsWindowReady()) std::cout << "Window loaded successfully!" << '\n';

	// Game FPS
	SetTargetFPS(60);


	// Window Paramters
	SetWindowPosition(0, 30);

	HideCursor();
	InitAudioDevice(); // Initialize audio device

	// Game Loop
	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();;
		if (IsKeyPressed(KEY_F)) {
			ToggleFullscreen();
		}
		if (!gameOver)
		{
			MoveBall(deltaTime);
			MovePaddles(deltaTime);
			HandlePaddleCollisions(g_paddle1, g_ball, wasCollidingPad1);
			HandlePaddleCollisions(g_paddle2, g_ball, wasCollidingPad2);
			
		}
		BeginDrawing();
			ClearBackground(BLACK);
			// Render
			DrawDivider();
			DrawPaddles();

			InitScore();
		

			EndGame();
			if (gameOver)
			{
				gameOverTimer -= deltaTime;

				if (gameOverTimer <= 0.0f)
				{
					break;   // exit the game loop
				}
			}
		
		EndDrawing();
	}
	
	// Cleanup
	CloseWindow();
	return 0;
}

void DrawDivider()
{
	Vector2 startPos = { g_screenWidth / 2, 0.0 };
	Vector2 endPos = { startPos.x, g_screenHeight };
	DrawLineDashed(startPos, endPos, 15, 10, GRAY);
}
void DrawPaddles()
{
	// Draw paddle 1
	DrawRectangleV(g_paddle1.position, g_paddle1.size, g_paddle1.color);

	// Draw paddle 2
	DrawRectangleV(g_paddle2.position, g_paddle2.size, g_paddle2.color);
}

void MovePaddles(float deltaTime)
{
	
	// paddle 1 movement
	if (IsKeyDown(KEY_W) || IsKeyPressed(KEY_W))
	{
		if (g_paddle1.position.y >= 20.0f)
			g_paddle1.position.y -= speed * deltaTime;
	}

	if (IsKeyDown(KEY_S) || IsKeyPressed(KEY_S))
	{
		if (g_paddle1.position.y <= g_screenHeight - g_paddle1.size.y - 18.0f)
			g_paddle1.position.y += speed * deltaTime;
	}

	// Paddle 2 movement
	if (IsKeyDown(KEY_UP) || IsKeyPressed(KEY_UP))
	{

		if (g_paddle2.position.y >= 20.0f)
			g_paddle2.position.y -= speed * deltaTime;
	}

	if (IsKeyDown(KEY_DOWN) || IsKeyPressed(KEY_DOWN))
	{
		if (g_paddle2.position.y <= g_screenHeight - g_paddle2.size.y - 18.0f)
			g_paddle2.position.y += speed * deltaTime;
	}
	
}
void MoveBall(float deltaTime)
{
	
	DrawCircle(g_ball.position.x, g_ball.position.y, radius, RAYWHITE);
	g_ball.position.x -= g_ball.velocityX * deltaTime;
	g_ball.position.y += g_ball.velocityY * deltaTime;
	Vector2 reset = { centerX, centerY };
	// Check out of bounds

	if (g_ball.position.x <= 1.0f)
	{
		RenderScore(g_ball);
		Sound fxScored = LoadSound("Resources/SoundEffects/scored.wav");
		PlaySound(fxScored);
		g_ball.position = reset;
		g_ball.position.x += -g_ball.velocityX * deltaTime;
	}
	if (g_ball.position.x >= g_screenWidth - g_ball.radius)
	{
		RenderScore(g_ball);
		Sound fxScored = LoadSound("Resources/SoundEffects/scored.wav");
		PlaySound(fxScored);
		g_ball.position = reset;
		g_ball.position.x += -g_ball.velocityX * deltaTime;
	}

	if (g_ball.position.y <= 1.0f)
	{
		g_ball.velocityY = -g_ball.velocityY;
		g_ball.position.y = 0.0f;
		Sound fxPong = LoadSound("Resources/SoundEffects/pong.mp3");
		PlaySound(fxPong);
	}
	if (g_ball.position.y >= g_screenHeight - g_ball.radius)
	{
		g_ball.velocityY = -g_ball.velocityY;
		Sound fxPong = LoadSound("Resources/SoundEffects/pong.mp3");
		PlaySound(fxPong);
	
	}
}
void InitScore()
{

	if (scoreCounter1 == 0) {
		DrawText(std::to_string(scoreCounter1).c_str(), centerX / 2 + g_screenWidth / 2, g_paddleWidth, 120, BEIGE);
	}
	else if (scoreCounter1 > 0)
	{
		DrawText(std::to_string(scoreCounter1).c_str(), centerX / 2 + g_screenWidth / 2, g_paddleWidth, 120, BEIGE);
	}

	if (scoreCounter2 == 0) {
		DrawText(std::to_string(scoreCounter2).c_str(), centerX / 2, g_paddleWidth, 120, SKYBLUE);
	}
	else
	{
		DrawText(std::to_string(scoreCounter2).c_str(), centerX / 2, g_paddleWidth, 120, SKYBLUE);
	}
}
void RenderScore(Ball& ball)
{
		if (g_ball.position.x <= 1.0f) {

			scoreCounter1++;
		}
		if (g_ball.position.x >= g_screenWidth - g_ball.radius)
		{
			scoreCounter2++;
		}
}
void EndGame()
{
	if (scoreCounter1 == 20) {
		DrawText("GAME OVER", 550, g_paddleHeight + 100, 150, RED);
		DrawText("PLAYER 2 WON!", g_paddleWidth + 550, g_paddleHeight + 400, 100, GREEN);
		gameOver = true;
	


	}
	else if (scoreCounter2 == 20) {
		DrawText("GAME OVER", 550, g_paddleHeight + 100, 150, RED);
		DrawText("PLAYER 1 WON!", g_paddleWidth + 550, g_paddleHeight + 400, 100, GREEN);
		gameOver = true;
	}
}
void HandlePaddleCollisions(Paddle& paddle, Ball& ball, bool& wasColliding)
{
	float paddleTop = paddle.position.y;
	float paddleLeft = paddle.position.x;
	float paddleBottom = paddle.position.y + paddle.size.y;
	float paddleRight = paddle.position.x + paddle.size.x;

	Vector2 closestPoint;

	// closest point
	if (ball.position.x < paddleLeft)
		closestPoint.x = paddleLeft;
	else if (ball.position.x > paddleRight)
		closestPoint.x = paddleRight;
	else
		closestPoint.x = ball.position.x;

	if (ball.position.y < paddleTop)
		closestPoint.y = paddleTop;
	else if (ball.position.y > paddleBottom)
		closestPoint.y = paddleBottom;
	else
		closestPoint.y = ball.position.y;

	Vector2 distance = { ball.position.x - closestPoint.x, ball.position.y - closestPoint.y };

	float mag = sqrt(distance.x * distance.x + distance.y * distance.y);

	bool isColliding = (mag <= ball.radius);

	if (isColliding && !wasColliding)
	{
		
		if (mag != 0)
		{
			distance.x /= mag;
			distance.y /= mag;

			float penetration = ball.radius - mag;

			ball.position.x += distance.x * penetration;
			ball.position.y += distance.y * penetration;
		}

		// set bounce offset so the bounce is more realistic
		float paddleCenterY = (paddleTop + paddleBottom) / 2;
		float distanceFromCenter = ball.position.y - paddleCenterY;

		float normalized = distanceFromCenter / (paddle.size.y / 2);

		float maxBounce = 300.0f;
		Sound fxPing = LoadSound("Resources/SoundEffects/pingp1.mp3");
		PlaySound(fxPing);
		std::cout << "sound played" << '\n';
		ball.velocityY = normalized * maxBounce;
		ball.velocityX = -ball.velocityX;
		
	}

	wasColliding = isColliding;
}
std::ostream& operator<<(std::ostream& out, const Vector2& vec)
{
	out << "x value: " << vec.x << ", " << "y value: " << vec.y << '\n';
	return out;
}
