#include <cassert>
#include <chrono>
#include <iostream>
#include <mutex>
#include <omp.h>
#include <vector>

using namespace std;

template <typename T> T reduction_atomic(const vector<T> &data) {
  T sum = T();

  auto begin = omp_get_wtime();

#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < data.size(); i++) {
    sum += data[i];
  }
  auto duration = omp_get_wtime() - begin;

  cout << duration << " s. ";

  return sum;
}

template <typename T> T reduction_critical(const vector<T> &data) {
  T sum = T();

  auto begin = omp_get_wtime();

#pragma omp parallel
  {
    T local_sum = T();
#pragma omp for
    for (int i = 0; i < data.size(); i++) {
      local_sum += data[i];
    }
#pragma omp critical
    sum += local_sum;
  }
  auto duration = omp_get_wtime() - begin;

  cout << duration << " s. ";

  return sum;
}

template <typename T> T reduction_lock(const std::vector<T> &data) {
  omp_lock_t sum_lock;
  T sum = T();

  omp_init_lock(&sum_lock);

  auto begin = omp_get_wtime();

#pragma omp parallel shared(sum_lock)
  {
    T local_sum = T();
#pragma omp for
    for (int i = 0; i < data.size(); i++) {
      local_sum += data[i];
    }
    omp_set_lock(&sum_lock);
    sum += local_sum;
    omp_unset_lock(&sum_lock);
  }

  auto duration = omp_get_wtime() - begin;

  std::cout << duration << " s. ";

  omp_destroy_lock(&sum_lock);

  return sum;
}

template <typename T> T reduction_der_for(const vector<T> &data) {
  T sum = T();

  auto begin = omp_get_wtime();

#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < data.size(); i++) {
    sum += data[i];
  }
  auto duration = omp_get_wtime() - begin;

  cout << duration << " s. ";
  return sum;
}

template <typename T> T reduction_linear(const vector<T> &data) {
  T sum = T();

  auto begin = omp_get_wtime();

  for (int i = 0; i < data.size(); i++) {
    sum += data[i];
  }
  auto duration = omp_get_wtime() - begin;

  cout << duration << " s. ";
  return sum;
}

int main() {
  const long N = 10000000;
  std::vector<double> data(N, 1.);

  omp_set_dynamic(0);
  omp_set_num_threads(4);

  cout << "\nAtomic:\t\t";
  auto atomic = reduction_atomic(data);

  cout << "\nCritical:\t";
  auto critical = reduction_critical(data);

  cout << "\nLock:\t\t";
  auto lk = reduction_lock(data);

  cout << "\nDirective for:\t";
  auto dir = reduction_der_for(data);

  cout << "\nLinear: \t";
  auto sum = reduction_linear(data);

  assert(atomic == critical);
  assert(critical == lk);
  assert(lk == dir);
  assert(sum == dir);

  return 0;
}
/*
Atomic:         0.0100679 s. 
Critical:       0.0097207 s. 
Lock:           0.0094414 s. 
Directive for:  0.0133997 s. 
Linear:         0.052538 s. 
*/