#include <cstring>
#include <iomanip>
#include <iostream>

// Все операции одного типа: сумма по ссылке, аргумент (если нужен)
// операция читает сама.
void Deposit(int& s) {
  int k;
  std::cin >> k;
  if (k <= 0) {
    std::cout << "error\n";
    return;
  }
  s += k;
}

void Withdraw(int& s) {
  int k;
  std::cin >> k;
  if (k <= 0 || k > s) {
    std::cout << "error\n";
    return;
  }
  s -= k;
}

void Balance(int& s) { std::cout << s << "\n"; }

void Reset(int& s) { s = 0; }

// Своя команда: удвоить сумму.
void Double(int& s) { s *= 2; }

const int kCnt = 5;
const char* kNames[kCnt] = {"deposit", "withdraw", "balance", "reset",
                            "double"};
void (*kOps[kCnt])(int&) = {Deposit, Withdraw, Balance, Reset, Double};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int s = 0;
  char cmd[64];
  while (std::cin >> std::setw(64) >> cmd) {
    if (std::strcmp(cmd, "stop") == 0) {
      break;
    }
    int pos = -1;
    for (int i = 0; i < kCnt; ++i) {
      if (std::strcmp(cmd, kNames[i]) == 0) {
        pos = i;
        break;
      }
    }
    if (pos == -1) {
      std::cout << "error\n";
      std::cin.ignore(1000, '\n');
    } else {
      kOps[pos](s);
    }
  }
  return 0;
}
