#include <iostream>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <random>

using namespace std;

//10 18 목표
//1.방 배열에서 주변 지뢰 개수 보여주는 알고리즘 생각해내기(예외처리 부분 간단히 할 수 있는 방법 구상)
//2. 자동 펼침(주변 지뢰 갯수 0개인 경우 주변 0짜리 모두 공개)
int **makeMap(int &row, int &col);
void hideBoom(int **(&map), const int &row, const int &col, const int &count);
void showMap(int **(&map), const int &row, const int &col);
void deleteMap(int **&map, const int &row);
void playGame(int **(&map), const int &row, const int &col, const int &count);
void getCount(const int &row, const int &col, int &count);
void setCursorView(bool visible);
void gotoxy(int x, int y);
void setColor(int color, int bgcolor);
bool hasWon(const int &row, const int &col, const int &countRound, const int &count);

int main(void)
{
  cout << "학번: 201911156, 이름: 김별찬" << endl;

  int row, col, count;

  setCursorView(false);

  int **map = makeMap(row, col);
  getCount(row, col, count);
  hideBoom(map, row, col, count);
  showMap(map, row, col);
  playGame(map, row, col, count);
  deleteMap(map, row);
  system("pause");

  return 0;
}

typedef struct cursor
{
  int x;
  int y;
} CURSOR;

int **makeMap(int &row, int &col)
{
  cout << "맵의 가로와 세로 길이를 입력하세요: ";
  cin >> row >> col;
  int **map = new int *[row];
  for (int i = 0; i < row; i++)
  {
    map[i] = new int[col];
    for (int j = 0; j < col; j++)
    {
      map[i][j] = 0;
    }
  }
  return map;
}

void hideBoom(int **(&map), const int &row, const int &col, const int &count)
{
  srand((unsigned)time(NULL));
  int num = 0;
  while (num != count)
  {
    int x = rand() % row;
    int y = rand() % col;
    if (map[x][y] == 0)
    {
      map[x][y] = 1;
      num++;
    }
  }
}

//멥 하나로 운용하는법
//1. 숨겨진 칸 필요
//2. 숨겨진 칸이지만 지뢰가 있는 칸
//3. 밝혀진 칸
void showMap(int **(&map), const int &row, const int &col)
{
  string items[] = {"□", "■", "★"};

  //짝수: 폭탄 없음. 홀수: 폭탄 있음
  //0, 1: 숨겨짐. 2, 3: 밝혀짐
  //문자: 주변 폭탄에 있는 폭탄의 개수
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      if (map[i][j] == 0) //숨겨진 칸
      {
        cout << items[0] << "  ";
      }
      else if (map[i][j] == 1)
      {
        cout << items[2] << "  ";
      }
      else if (map[i][j] == 2) //밝혀진 폭탄 없는 칸
      {
        cout << items[1] << "  ";
      }
      else if (map[i][j] >= '1' && map[i][j] <= '8')
      {
        char c = map[i][j];
        cout << c << "   ";
      }

      /*
			if (map[i][j] == 0 || map[i][j] == 1) //숨겨진 칸
			{
				cout << items[0] << "  ";
			}
			else if (map[i][j] == 2) //밝혀진 폭탄 없는 칸
			{
				cout << items[1] << "  ";
			}
			else if (map[i][j] == 3)//밝혀진 폭탄 있는 칸
			{
				cout << items[2] << "  ";
			}
			else if (map[i][j] >= '1' && map[i][j] <= '8') {

				cout << (char) map[i][j] << " ";
			}
			*/
    }
    cout << endl;
  }
}

void deleteMap(int **&map, const int &row)
{
  for (int i = 0; i < row; i++)
  {
    delete[] map[i];
  }
  delete[] map;
}

void playGame(int **(&map), const int &row, const int &col, const int &count)
{
  CURSOR cursor;
  cursor.x = 0;
  cursor.y = 0;
  int countRound = 0;
  int checkBlock = 0;

  while (true)
  {
    system("cls");
    showMap(map, row, col);

    if (hasWon(row, col, countRound, count))
    {
      cout << "게임 클리어!" << endl;
      return;
    }
    cout << cursor.x << endl;
    cout << cursor.y << endl;
    cout << checkBlock << endl;

    gotoxy(cursor.x, cursor.y);
    setColor(rand() % 15 + 1, 0);
    cout << "○";

    bool hasCharacter;
    switch (_getch())
    {
    case 224:
      switch (_getch())
      {
      case 72:
        cursor.y -= 1;
        break;
      case 80:
        cursor.y += 1;
        break;
      case 75:
        cursor.x -= 2;
        break;
      case 77:
        cursor.x += 2;
        break;
      default:
        break;
      }
      break;
    case 13: //칸 밝히기, 엔터키
      hasCharacter = (cursor.x % 4 == 0);
      if (hasCharacter)
      { //커서의 위치에 문자가 있었는지 확인
        //커서의 좌표를 배열의 행열상 위치로 변환
        int hPos = cursor.x / 4;
        int vPos = cursor.y;
        if (map[vPos][hPos] == 0)
        { //숨겨진 블럭
          countRound++;
          if ((vPos > 0 && vPos < row - 1) && (hPos > 0 && hPos < col - 1))
          {
            //모서리 부분 제외
            int countBoom = 0;
            for (int i = -1; i < 2; i++)
            {
              for (int j = -1; j < 2; j++)
              {
                if (map[vPos + i][hPos + j] == 1)
                {
                  countBoom++;
                }
              }
            }

            if (countBoom == 0)
            {
              map[vPos][hPos] += 2;
            }
            else
            {
              map[vPos][hPos] = countBoom + '0';
            }

            showMap(map, row, col);
          }
          else
          {
            map[cursor.y][cursor.x / 4] += 2;
            // ---------------수정 필요 ------------
            /*
						if (!(vPos > 0 && vPos < row - 1) && (hPos > 0 && hPos < col - 1)) {
							//위, 아래 모서리

						} else if ((vPos > 0 && vPos < row - 1) && !(hPos > 0 && hPos < col - 1)) {
							//왼쪽, 오른쪽 모서리

						} else if (!(vPos > 0 && vPos < row - 1) && !(hPos > 0 && hPos < col - 1)) {						
							//꼭짓점
						}
						*/
          }
        }
        else if (map[cursor.y][cursor.x / 4] == 1)
        { //숨겨진 폭탄
          map[cursor.y][cursor.x / 4] += 2;
          system("cls");
          showMap(map, row, col);
          cout << "게임 오버" << endl;
          return;
        }
      }
      break;
    case 27: //esc 버튼을 누름
      system("cls");
      cout << "프로그램 종료" << endl;
      return;
    }
  }
}

void getCount(const int &row, const int &col, int &count)
{
  cout << "지뢰 개수를 입력하세요: ";
  cin >> count;
  while (count > row * col)
  {
    cout << "지뢰 개수가 너무 많습니다. 다시 입력하여 주십시오" << endl;
    cin >> count;
  }
}

void setCursorView(bool visible)
{
  CONSOLE_CURSOR_INFO cursor = {1, visible};
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void gotoxy(int x, int y)
{
  COORD Pos = {x, y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void setColor(int color, int bgcolor)
{
  color &= 0xf;
  bgcolor &= 0xf;
  static HANDLE std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(std_output_handle, bgcolor << 4 | color);
}

bool hasWon(const int &row, const int &col, const int &countRound, const int &count)
{
  if (countRound == row * col - count)
    return true;
  return false;
}