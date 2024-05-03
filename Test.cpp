#include <iostream>
#include <raylib.h>

using namespace std;

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

int CellSize = 30;
int CellCount = 25;

class Snake
{
public:
	bool addSegment = false;

	void Update()
	{
		body.push_front(Vector2Add(body[0], direction));
		if(addSegment == true)
		{
			addSegment = false;
		}
		else
		{
			body.pop_back();
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
		DrawTexture(texture, position.x * CellSize, position.y * CellSize, WHITE);
	}

	Vector2 GenerateRandomPos()
	{
		float x = GetRandomValue(0, CellCount - 1);
		float y = GetRandomValue(0, CellCount - 1);
		return Vector2{ x, y };
	}
};

class Game
{
public:
	void CheckCollisionWithFood()
	{
		if(Vector2Equals(snake.body[0], food.position));
		{
			snake.addSegment = true;
		}
	}
};

int main()
{
	cout << "Starting the game..." << endl;
	InitWindow(CellSize * CellCount, CellSize * CellCount, "Retro Snake");
	SetTargetFPS(60);

	Food food = Food();

	while (WindowShouldClose() == false)
	{
		BeginDrawing();

		//Drawing
		ClearBackground(green);
		food.Draw();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
