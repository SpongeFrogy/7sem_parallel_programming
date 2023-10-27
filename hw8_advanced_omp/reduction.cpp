#include <cassert>
#include <chrono>
#include <iostream>
#include <mutex>
#include <omp.h>
#include <vector>

using namespace std;

template <typename T> T reduction_atomic(const vector<T> &data) {
  T sum = T();

  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < data.size(); i++) {
    sum += data[i];
  }

  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end - begin).count();

  cout << duration << " ms. ";

  return sum;
}

template <typename T> T reduction_critical(const vector<T> &data) {
  T sum = T();
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

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

  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end - begin).count();

  cout << duration << " ms. ";

  return sum;
}

template <typename T> T reduction_lock(const vector<T> &data) {
  mutex m;
  T sum = T();
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

#pragma omp parallel
  {
    T local_sum = T();
#pragma omp for
    for (int i = 0; i < data.size(); i++) {
      local_sum += data[i];
    }
    {
      lock_guard<mutex> lk(m);
      sum += local_sum;
    }
  }

  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end - begin).count();

  cout << duration << " ms. ";
  return sum;
}

template <typename T> T reduction_der_for(const vector<T> &data) {
  T sum = T();
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < data.size(); i++) {
    sum += data[i];
  }
  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end - begin).count();

  cout << duration << " ms. ";
  return sum;
}

template <typename T> T reduction_linear(const vector<T> &data) {
  T sum = T();
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();
  for (int i = 0; i < data.size(); i++) {
    sum += data[i];
  }
  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end - begin).count();

  cout << duration << " ms. ";
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
  auto lk = reduction_atomic(data);

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
Atomic:         22 ms. 
Critical:       19 ms. 
Lock:           16 ms. 
Directive for:  19 ms. 
Linear:         69 ms. 
*/