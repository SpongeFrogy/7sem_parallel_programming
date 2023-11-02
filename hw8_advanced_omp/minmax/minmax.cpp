#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;

template <typename T> T max_omp(const vector<T> &data) {
  T max_value = data[0];
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();
#pragma omp parallel for
  for (int i = 0; i < data.size(); i++) {

#pragma omp critical
    {
      if (data[i] > max_value) {
        max_value = data[i];
      }
    }
  }
  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end - begin).count();
  cout << duration << " ms. ";
  return max_value;
}

template <typename T> T max_linear(const vector<T> &data) {
  T max_value = data[0];
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

  for (int i = 0; i < data.size(); i++) {
    {
      if (data[i] > max_value) {
        max_value = data[i];
      }
    }
  }
  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end - begin).count();
  cout << duration << " ms. ";
  return max_value;
}

int main() {
  omp_set_dynamic(0);
  omp_set_num_threads(4);

  vector<int> data(1000000, 0);
  srand(time(0));
  generate(data.begin(), data.end(), rand);

  cout << "OpenMP:\t";
  auto max_v_omp = max_omp(data);

  cout << "\nLinear:\t";
  auto max_v_lin = max_linear(data);

  assert(max_v_lin == max_v_omp);

  return 0;
}
/*
OpenMP: 145 ms.
Linear: 4 ms.
*/