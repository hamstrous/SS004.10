#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
#define MAX 10000
using namespace std;

int dodai = 7;
void gotoxy(int x, int y)
{
    static HANDLE h = NULL;
    if(!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
     COORD c = { x, y };
    SetConsoleCursorPosition(h,c);
}
void SetUpSnake(int toadox[], int toadoy[])
{
    int x = 36, y = 13;
    for (int i = 0; i < dodai; i++)
    {
        toadox[i] = x;
        toadoy[i] = y;
        x--;
    }
}
void DrawSnake(int toadox[], int toadoy[])
{
    for (int i = 0; i < dodai; i++)
    {
        gotoxy(toadox[i], toadoy[i]);
        if (i == 0) cout << "0";
        else cout << "o";
    }
}
bool FruitInSnake(int toadox[], int toadoy[], int xF, int yF)
{
    for(int i = 0; i < dodai; i++)
        if(xF == toadox[i] && yF == toadoy[i])
            return true; //qua spawn ngay con ran
    return false;
}
bool SnakeEatFruit(int x, int y, int xF, int yF)
{
    if (x == xF && y == yF)
        return true; //ran an qua
    return false;
}
void CreateFruit(int &xF, int &yF, int toadox[], int toadoy[])
{
    do
    {
        xF = rand() % 71 + 1;
        yF = rand() % 25 + 1;
    } while (FruitInSnake(toadox, toadoy, xF, yF) == true);
    gotoxy(xF, yF);
    cout << "x";
}
void LeftWall(int x, int y)
{
    y = 0;
    while (y <= 26) //Tuong trai
    {
        gotoxy(0, y);
        cout << "|";
        y++;
    }
}
void RightWall(int x, int y)
{
    y = 0;
    while (y <= 26) //Tuong phai
    {
        gotoxy(72, y);
        cout << "|";
        y++;
    }
}
void TopWall(int x, int y)
{
    x = 1;
    while (x < 72)
    {
        gotoxy(x, 0);
        cout << "-";
        x++;
    }
}
void BotWall(int x, int y)
{
    x = 1;
    while (x < 72)
    {
        gotoxy(x, 26);
        cout << "-";
        x++;
    }
}
void DrawWall(int x, int y)
{
    LeftWall(x, y);
    RightWall(x, y);
    TopWall(x, y);
    BotWall(x, y);
}
void Add(int a[], int x)
{
    for(int i = dodai; i > 0; i--)
    {
        a[i] = a[i - 1];
    }
    a[0] = x;
    dodai++;
}
void Delete(int a[], int vt)
{
    for (int i = vt; i < dodai; i++)
    {
        a[i] = a[i + 1];
    }
    dodai--;
}
void FixSnake(int toadox[], int toadoy[], int x, int y, int &xF, int &yF)
{
    //them dau mang
    Add(toadox, x);
    Add(toadoy, y);
    if (SnakeEatFruit(toadox[0], toadoy[0], xF, yF) != true) //ran khong an qua
    {
        //xoa cuoi mang
        Delete(toadox, dodai - 1);
        Delete(toadoy, dodai - 1);
    }
    else //ran an qua
        CreateFruit(xF, yF, toadox, toadoy);
    //ve ran lai
    DrawSnake(toadox, toadoy);
}
void Move(int &check, int &x, int &y)
{
    if(_kbhit())
        {
            char s = _getch();
            if (s == 'w' && check != 0)
                check = 1;
            if (s == 's' && check != 1)
                check = 0;
            if (s == 'a' && check != 2)
                check = 3;
            if (s == 'd'&& check != 3)
                check = 2;
        }
        if (check == 0)
            y++;
        if (check == 1)
            y--;
        if (check == 2)
            x++;
        if (check == 3)
            x--;
}
void DeleteOldData(int toadox[], int toadoy[])
{
    for(int i = 0; i < dodai; i++)
    {
        gotoxy(toadox[i], toadoy[i]);
        cout << " ";
    }
}
bool TouchWall (int toadox[], int toadoy[])
{
    if (toadox[0] == 72 || toadox[0] == 0 || toadoy[0] == 0 || toadoy[0] == 26)
        return true;
    return false;
}
bool TouchTail (int toadox[], int toadoy[])
{
    for(int i  = 1; i < dodai; i++)
        if (toadox[0] == toadox[i] && toadoy[0] == toadoy[i])
            return true;
    return false;
}
bool EndGame(int toadox[], int toadoy[])
{
    if (TouchWall(toadox, toadoy) == true || TouchTail(toadox, toadoy) == true)
        return true;
    return false;
}
int main()
{
    srand(time(NULL));
    int xF, yF;
    int dx = 36, dy = 13;
    int x = 72, y = 26, check = 2;
    int toadox[MAX], toadoy[MAX];

    //tao tuong
    DrawWall(x, y);

    //tao va ve ran
    SetUpSnake(toadox, toadoy);
    DrawSnake(toadox, toadoy);

    //tao qua
    CreateFruit(xF, yF, toadox, toadoy);
    while (true)
    {
        DeleteOldData(toadox, toadoy);
        Move(check, dx, dy);
        FixSnake(toadox, toadoy, dx, dy, xF, yF);
        if (EndGame(toadox, toadoy) == true)
            break;
        Sleep(300);
    }
    gotoxy(73,0);
    cout << "End game";

    _getch();
    return 0;
}
