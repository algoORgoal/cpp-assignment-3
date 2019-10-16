#include <iostream>
#include <random>
#include <string>
#include <time.h>

using namespace std;

//오늘의 목표
// 1. 사용자로부터 맵 크기 받기
// 2. 맵 생성하기
// 3. 지뢰 심기
// 4. 맵 출력하기

int **makeMap(int &row, int &col) {
  cout << "맵의 가로와 세로 길이를 입력하세요: ";
  cin >> row >> col;
  int **map = new int *[row];
  for (int i = 0; i < row; i++) {
    map[i] = new int[col];
    for (int j = 0; j < col; j++) {
      map[i][j] = 0;
    }
  }
  return map;
}
void hideBoom(int **(&map), const int &row, const int &col) {

  srand((unsigned)time(NULL));
  int num = 0;
  while (num != count) {
    int x = rand() % row;
    int y = rand() % col;
    if (map[x][y] == 0) {
      map[x][y] = 1;
      num++;
    }
  }
}
void showMap(int **(&map), const int &row, const int &col) {
  string items[] = {"□", "★", "○"};
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      if (map[i][j] == 0) //지뢰 아님
      {
        cout << items[0] << ' ';
      } else if (map[i][j] == 1) //지뢰
      {
        cout << items[1] << ' ';
      }
    }
    cout << endl;
  }
}

void deleteMap(int **&map, const int &row) {
  for (int i = 0; i < row; i++) {
    delete[] map[i];
  }
  delete[] map;
}

int getCount(const int &row, const int &col) {
  cout << "지뢰 개수를 입력하세요: ";
  int count;
  cin >> count;
  while (count > row * col) {
    cout << "지뢰 개수가 너무 많습니다. 다시 입력하여 주십시오" << endl;
    cin >> count;
  }
  return count;
}

int main(void) {
  cout << "학번: 201911156, 이름: 김별찬" << endl;
  int row, col, count;

  int **map = makeMap(row, col);
  count = getCount(row, col);
  hideBoom(map, row, col);
  showMap(map, row, col);
  deleteMap(map, row);
}
