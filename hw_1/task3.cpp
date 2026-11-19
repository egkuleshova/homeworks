#include <cstring>
#include <iomanip>
#include <iostream>

const int kH = 5;
const int kW = 10;

void Clear(char b[kH][kW]) {
  for (int i = 0; i < kH; ++i) {
    for (int j = 0; j < kW; ++j) {
      b[i][j] = ' ';
    }
  }
}

bool Inside(int x, int y) { return 0 <= x && x < kW && 0 <= y && y < kH; }

bool Put(char b[kH][kW], int x, int y, char c) {
  if (!Inside(x, y)) {
    return false;
  }
  b[y][x] = c;
  return true;
}

bool Text(char b[kH][kW], int x, int y, const char* w) {
  int len;
  len = static_cast<int>(std::strlen(w));
  // сначала проверяем, что слово влезает целиком, потом пишем
  if (!Inside(x, y) || !Inside(x + len - 1, y)) {
    return false;
  }
  for (int i = 0; i < len; ++i) {
    Put(b, x + i, y, w[i]);
  }
  return true;
}

void Print(const char b[kH][kW]) {
  for (int i = 0; i < kH; ++i) {
    for (int j = 0; j < kW; ++j) {
      std::cout << b[i][j];
    }
    std::cout << "\n";
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  char b[kH][kW];
  Clear(b);
  char cmd[64];
  while (std::cin >> std::setw(64) >> cmd) {
    if (std::strcmp(cmd, "quit") == 0) {
      break;
    }
    if (std::strcmp(cmd, "clear") == 0) {
      Clear(b);
    } else if (std::strcmp(cmd, "put") == 0) {
      int x;
      int y;
      char c;
      std::cin >> x >> y >> c;
      if (!Put(b, x, y, c)) {
        std::cout << "error\n";
      }
    } else if (std::strcmp(cmd, "text") == 0) {
      int x;
      int y;
      char w[64];
      std::cin >> x >> y >> std::setw(64) >> w;
      if (!Text(b, x, y, w)) {
        std::cout << "error\n";
      }
    } else if (std::strcmp(cmd, "print") == 0) {
      Print(b);
    } else {
      std::cout << "error\n";
      std::cin.ignore(1000, '\n');
    }
  }
  return 0;
}
