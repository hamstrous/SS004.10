#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

int cellSize = 30;
int cellCount = 25;

double lastUpdateTime = 0;

bool eventTriggered(double interval)
{
	double current = GetTime();
	if (current - lastUpdateTime >= interval)
	{
		lastUpdateTime = current;
		return true;
	}
	return false;
}
class Ran
{
public:
	deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
	Vector2 direction = {1, 0};

	void Draw() {
		for (int i = 0; i < body.size();i++) {
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment = Rectangle{ x * cellSize,y * cellSize,(float) cellSize, (float) cellSize };
			DrawRectangleRounded(segment, 0.5, 6, darkGreen);
		}
	}
};

class Food
{
public:
	Vector2 position;
	Texture2D texture;

	Food()
	{
		Image image = LoadImage("Graphics/Food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos();
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void Draw()
	{
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
	}

	Vector2 GenerateRandomPos()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x, y };
	}
};

int Conflict() {
	cout << "Starting the game..." << endl;
	InitWindow(cellSize * cellCount, cellSize * cellCount, "Retro Snake");
	SetTargetFPS(60);

	Food food = Food();
	Ran snake = Ran();


	while (WindowShouldClose() == false)
	{
		BeginDrawing();
		//Drawing
		food.Draw();
		snake.Draw();
		ClearBackground(green);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

int main()
{
	Conflict();
	return 0;
	cout << "Starting the game..." << endl;
	InitWindow(cellSize * cellCount, cellSize * cellCount, "Retro Snake");
	SetTargetFPS(60);

	Food food = Food();
	Ran snake = Ran();
	

	while (WindowShouldClose() == false)
	{
		BeginDrawing();
		//Drawing
		food.Draw();
		snake.Draw();
		ClearBackground(green);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
