#include <iostream>

using namespace std;

template <class T> double trapezoid_rule(T &func, double a, double b, int n) {
  double sum = 0.5 * (func(a) + func(b));
  double dx = (b - a) / n;
  for (int i = 1; i < n; i++) {
    sum += func(a + i * dx);
  }
  return sum * dx;
}

double d_pi(const double &x) { return 4 / (1 + x * x); }

int main() {
  double pi = trapezoid_rule(d_pi, 0., 1., 10000);

  cout << pi;
  return 0;
}
