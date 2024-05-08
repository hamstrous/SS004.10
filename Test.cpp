
#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

int cellSize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;


bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
	for (unsigned int i = 0; i < deque.size(); i++)
	{
		if (Vector2Equals(deque[i], element))
		{
			return true;
		}
	}
	return false;
}

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
class Snake
{
public:
	bool addSegment = false;
	deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
	Vector2 direction = { 1, 0 };

	void Draw()
	{
		for (unsigned int i = 0; i < body.size(); i++)
		{
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment = Rectangle{ x * cellSize, y * cellSize, (float)cellSize, (float)cellSize };
			DrawRectangleRounded(segment, 0.5, 6, darkGreen);
		}
	}

	void Update()
	{
		body.push_front(Vector2Add(body[0], direction));
		if (addSegment == true)
		{
			addSegment = false;
		}
		else
		{
			body.pop_back();
		}
	}

	void Reset()
	{
		body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        	direction = {1, 0};
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

class Game
{
public:
	Snake snake = Snake();
	Food food = Food();
	bool running = true;

	void CheckCollisionWithFood()
	{
		if (Vector2Equals(snake.body[0], food.position))
		{
			food.position = food.GenerateRandomPos();
			snake.addSegment = true;
		}
	}

	void draw()
	{
		food.Draw();
		snake.Draw();
		CheckCollisionWithFood();
	}

	void update()
	{
		if (running)
        	{
            	snake.Update();
            	CheckCollisionWithFood();
            	CheckCollisionWithEdges();
            	CheckCollisionWithTail();
        	}
	}

	void CheckCollisionWithEdges()
    	{
        	if (snake.body[0].x == cellCount || snake.body[0].x == -1)
        	{
            	GameOver();
	        }
        	if (snake.body[0].y == cellCount || snake.body[0].y == -1)
        	{
            	GameOver();
        	}
    	}

	void GameOver()
	{
		snake.Reset();
		food.position = food.GenerationRandomPos(snake.body);
		running = false;
	}

	void CheckCollisionWithTail()
	{
		deque<Vector2> headlessBody = snake.body;
        	headlessBody.pop_front();
        	if (ElementInDeque(snake.body[0], headlessBody))
        	{
            	GameOver();
        	}
	}

};
int main()
{
	cout << "Starting the game..." << endl;
	InitWindow(2*offset + cellSize * cellCount, 2*offset + cellSize * cellCount, "Retro Snake");
	SetTargetFPS(60);

	Game game = Game();
	while (WindowShouldClose() == false)
	{
		BeginDrawing();

		if (eventTriggered(0.2))
		{
			game.snake.Update();
		}

		if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
		{
			game.snake.direction = { 0, -1 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
		{
			game.snake.direction = { 0, 1 };
			game.running = true;
		}
		if (IsKeyDown(KEY_LEFT) && game.snake.direction.x != 1)
		{
			game.snake.direction = { -1, 0 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
		{
			game.snake.direction = { 1, 0 };
			game.running = true;
		}
		//Drawing
		ClearBackground(green);
		DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, 
			(float)cellSize*cellCount+10, (float)cellSize*cellCount+10}, 5, darkGreen);
		DrawText("Snake", offset -5, 20, 40, darkGreen);
		game.draw();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
