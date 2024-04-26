#include <iostream>
#include <raylib.h>

using namespace std;

int main(){
  cout << "Game starting ..." << endl;
  InitWindow(750, 750, "Snake");
  SetTargetFPS(60);

  while(WindowShouldClose() == false)
  {
    BeginDrawing();
    //Draw
    ClearBackground(GREEN);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
