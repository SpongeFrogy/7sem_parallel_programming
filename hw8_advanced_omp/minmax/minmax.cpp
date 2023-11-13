#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;

template <typename T> T max_omp(const vector<T> &data) {
  T max_value = data[0];
  auto begin = omp_get_wtime();
#pragma omp parallel for
  for (int i = 0; i < data.size(); i++) {

#pragma omp critical
    {
      if (data[i] > max_value) {
        max_value = data[i];
      }
    }
  }
  auto duration = omp_get_wtime() - begin;
  cout << duration << " s. ";
  return max_value;
}

template <typename T> T max_linear(const vector<T> &data) {
  T max_value = data[0];
  auto begin = omp_get_wtime();

  for (int i = 0; i < data.size(); i++) {
    {
      if (data[i] > max_value) {
        max_value = data[i];
      }
    }
  }
  auto duration = omp_get_wtime() - begin;
  cout << duration << " s. ";
  return max_value;
}

int main() {
  omp_set_dynamic(0);
  omp_set_num_threads(4);

  vector<int> data(10000000, 0);
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
OpenMP: 1.42785 s. 
Linear: 0.0493946 s. 
*/