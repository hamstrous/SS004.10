#include <iostream>
#include <raylib.h>

using namespace std;

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

int CellSize = 30;
int CellCount = 25;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
	for(unsigned int i = 0; i < deque.size(); i++)
	{
		if (Vector2Equals(deque[i], element))
		{
			return true;
		}
	}
	return false;
}
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

	Food(deque<Vector2> snakebody)
	{
		Image image = LoadImage("Graphics/Food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(snakebody);
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void Draw()
	{
		DrawTexture(texture, position.x * CellSize, position.y * CellSize, WHITE);
	}

	Vector2 GenerateRandomCell()
	{
		float x = GetRandomValue(0, CellCount - 1);
		float y = GetRandomValue(0, CellCount - 1);
		return Vector2{x, y};
	}

	Vector2 GenerateRandomPos(deque<Vector2> snakebody)
	{
		Vector2 position = GenerateRandomCell();
		while (ElementInDeque(position, snakebody))
		{
			position = GenerateRandomCell();
		}
		return position;
	}
};

class Game
{
public:
	Snake snake = Snake();
	Food food = Food(snake.body);

	void Update()
	{
		snake.Update();
		CheckCollisionWithFood();
	}
	void CheckCollisionWithFood()
	{
		if(Vector2Equals(snake.body[0], food.position));
		{
			food.position = food.GenerateRandomPos(snake.body);	
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
