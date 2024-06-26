#include <iostream>
#include "raylib.h"
#include <deque>
#include "raymath.h"

using namespace std;

static bool allowMove = false;
Color green = { 6, 191, 43, 255 };
Color skyblue = { 157, 215, 239, 255 };
Color darkGreen = { 43, 51, 24, 255 };
Color white = {255, 255, 255, 255};
//kích thước mỗi ô trên bản đồ (pixel)
int cellSize = 30;
//Số lượng ô mỗi hàng và mỗi cột
int cellCount = 25;
//Vị trí pixel trên cùng bên trái của bản đồ
int offset = 75;

bool start = false;
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

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake
{
public:
    //Vị trí thân rắn
    deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
    Vector2 direction = { 1, 0 };
    bool addSegment = false;

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{ offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize };
            DrawRectangleRounded(segment, 0.5, 6, green);
        }

        if (body[0].x > body[1].x)
        {
            Texture2D head_texture;
            Image head_image = LoadImage("Graphics/RightHead.png");
            head_texture = LoadTextureFromImage(head_image);
            UnloadImage(head_image);
            DrawTexture(head_texture, offset + body[0].x * cellSize, offset + body[0].y * cellSize - 1, WHITE);
        }
        if (body[0].x < body[1].x)
        {
            Texture2D head_texture;
            Image head_image = LoadImage("Graphics/LeftHead.png");
            head_texture = LoadTextureFromImage(head_image);
            UnloadImage(head_image);
            DrawTexture(head_texture, offset + body[0].x * cellSize - 9, offset + body[0].y * cellSize - 1, WHITE);
        }
        if (body[0].y < body[1].y)
        {
            Texture2D head_texture;
            Image head_image = LoadImage("Graphics/UpHead.png");
            head_texture = LoadTextureFromImage(head_image);
            UnloadImage(head_image);
            DrawTexture(head_texture, offset + body[0].x * cellSize - 1, offset + body[0].y * cellSize - 9, WHITE);
        }
        if (body[0].y > body[1].y)
        {
            Texture2D head_texture;
            Image head_image = LoadImage("Graphics/DownHead.png");
            head_texture = LoadTextureFromImage(head_image);
            UnloadImage(head_image);
            DrawTexture(head_texture, offset + body[0].x * cellSize - 1, offset + body[0].y * cellSize, WHITE);
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
        body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
        direction = { 1, 0 };
    }
};

class Food
{

public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody)
    {
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{ x, y };
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
    {
        Vector2 position = GenerateRandomCell();
        //Kiểm tra rắn có chạm vào thức ăn không
        while (ElementInDeque(position, snakeBody))
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
    bool running = false;
    int score = 0;
	Sound eatSound;
	Sound wallSound;
	
    Game()
    {
	    InitAudioDevice();
	    eatSound = LoadSound("Sound/Sounds_eat.mp3");
	    wallSound = LoadSound("Sound/Sounds_wall.mp3");
    }

    ~Game()
    {
	    UnloadSound(eatSound);
	    UnloadSound(wallSound);
	    CloseAudioDevice();
    }

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        if (running)
        {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood()
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
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
        start = false;
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wallSound);
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
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Snake Game");
    SetTargetFPS(60);

    Game game = Game();
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(skyblue);
        DrawRectangleLinesEx(Rectangle{ (float)(offset + cellSize * 5), (float)(offset + cellSize * 13), (float)cellSize * 15, (float)cellSize * 3 }, 5, darkGreen);
        DrawText("Snake", (float)(offset + cellSize * 5) + cellSize * 3, (float)(offset + cellSize * 5), 90, darkGreen);
        DrawText("Start", (float)(offset + cellSize * 5) + cellSize * 5, (float)(offset + cellSize * 13) + 20, 50, darkGreen);
        EndDrawing();
        if (IsKeyDown(KEY_ENTER))
        {
            BeginDrawing();
            DrawRectangleLinesEx(Rectangle{ (float)(offset+cellSize*5), (float)(offset+cellSize*13), (float)cellSize * 15, (float)cellSize * 3}, 5, white);
            DrawText("Start", (float)(offset+cellSize*5)+cellSize*5, (float)(offset+cellSize*13)+20, 50, white);
            EndDrawing();
            lastUpdateTime = GetTime();
            while (!EventTriggered(0.5))
            {
            }
            start = true;
        }
        while (start)
        {
            Texture2D texture;
            Image image = LoadImage("Graphics/food.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            DrawTexture(texture, offset - 5, offset + cellSize * cellCount + 12, WHITE);
            BeginDrawing();
            if (EventTriggered(0.2))
            {
                game.Update();
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
            if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
            {
                game.snake.direction = { -1, 0 };
                game.running = true;
            }
            if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
            {
                game.snake.direction = { 1, 0 };
                game.running = true;
            }

            // Drawing
            ClearBackground(skyblue);
            DrawRectangleLinesEx(Rectangle{ (float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10 }, 5, darkGreen);
            DrawText("Snake", offset - 5, 20, 40, darkGreen);
            DrawText(TextFormat("%i", game.score), offset + 28, offset + cellSize * cellCount + 10, 40, darkGreen);
            game.Draw();
            EndDrawing();
            if (WindowShouldClose())
            {
                break;
            }
        }
    }
    CloseWindow();
    return 0;
}
