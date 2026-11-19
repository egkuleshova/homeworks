#include <iomanip>
#include <iostream>

// Метеостанция: за несколько месяцев записана температура, в каждом дне
// 4 замера (ночь, утро, день, вечер). Для каждого месяца считаем среднюю
// температуру и самый тёплый замер, потом печатаем отчёт.
//
// int*** t: t[месяц][день][замер]. Число месяцев и дней в каждом месяце
// вводится и у месяцев разное (28, 30, 31), поэтому все три уровня
// выделяются через new: t - массив месяцев, t[i] - дни i-го месяца,
// t[i][j] - 4 замера за день. Статический int[12][31][4] тратил бы память
// и не позволял задать произвольное число месяцев.
//
// void*: в отчёте лежат значения разных типов: название месяца (char*),
// среднее (double*) и указатель прямо на самую тёплую ячейку внутри t
// (int*). Чтобы хранить их в одном массиве и печатать одним циклом, держим
// void* и рядом букву с типом, при печати приводим обратно.
//
// Формат ввода:
// m
// название_месяца d   (d >= 1)
// d строк по 4 числа
// ...
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  const int kMeas = 4;
  int m;
  std::cin >> m;
  if (m <= 0) {
    return 0;
  }
  char** name = new char*[m];
  int* d = new int[m];
  int*** t = new int**[m];
  for (int i = 0; i < m; ++i) {
    name[i] = new char[32];
    std::cin >> std::setw(32) >> name[i] >> d[i];
    t[i] = new int*[d[i]];
    for (int j = 0; j < d[i]; ++j) {
      t[i][j] = new int[kMeas];
      for (int k = 0; k < kMeas; ++k) {
        std::cin >> t[i][j][k];
      }
    }
  }
  double* avg = new double[m];
  int sz = 3 * m;
  void** rep = new void*[sz];
  char* type = new char[sz];
  int cnt = 0;
  for (int i = 0; i < m; ++i) {
    int sum = 0;
    int* mx = &t[i][0][0];
    for (int j = 0; j < d[i]; ++j) {
      for (int k = 0; k < kMeas; ++k) {
        sum += t[i][j][k];
        if (t[i][j][k] > *mx) {
          mx = &t[i][j][k];
        }
      }
    }
    avg[i] = static_cast<double>(sum) / (d[i] * kMeas);
    rep[cnt] = name[i];
    type[cnt] = 's';
    ++cnt;
    rep[cnt] = &avg[i];
    type[cnt] = 'd';
    ++cnt;
    rep[cnt] = mx;
    type[cnt] = 'i';
    ++cnt;
  }
  std::cout << std::fixed << std::setprecision(1);
  for (int p = 0; p < sz; ++p) {
    if (type[p] == 's') {
      std::cout << static_cast<char*>(rep[p]) << ": ";
    } else if (type[p] == 'd') {
      std::cout << "avg " << *static_cast<double*>(rep[p]) << ", ";
    } else {
      std::cout << "max " << *static_cast<int*>(rep[p]) << "\n";
    }
  }
  delete[] type;
  delete[] rep;
  delete[] avg;
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < d[i]; ++j) {
      delete[] t[i][j];
    }
    delete[] t[i];
    delete[] name[i];
  }
  delete[] t;
  delete[] d;
  delete[] name;
  return 0;
}
