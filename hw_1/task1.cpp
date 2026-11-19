#include <iomanip>
#include <iostream>

struct MinMax {
  int mn;
  int mx;
};

double Average(const int* a, int n) {
  int sum = 0;
  for (int i = 0; i < n; ++i) {
    sum += a[i];
  }
  return static_cast<double>(sum) / n;
}

MinMax FindMinMax(const int* a, int n) {
  MinMax ans;
  ans.mn = a[0];
  ans.mx = a[0];
  for (int i = 1; i < n; ++i) {
    if (a[i] < ans.mn) {
      ans.mn = a[i];
    }
    if (a[i] > ans.mx) {
      ans.mx = a[i];
    }
  }
  return ans;
}

int CountFrost(const int* a, int n) {
  int cnt = 0;
  for (int i = 0; i < n; ++i) {
    if (a[i] < 0) {
      ++cnt;
    }
  }
  return cnt;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n;
  std::cin >> n;
  if (n <= 0) {
    return 0;
  }
  int* t = new int[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> t[i];
  }
  MinMax mm = FindMinMax(t, n);
  std::cout << std::fixed << std::setprecision(1) << Average(t, n) << "\n";
  std::cout << mm.mn << " " << mm.mx << "\n";
  std::cout << CountFrost(t, n) << "\n";
  delete[] t;
  return 0;
}
