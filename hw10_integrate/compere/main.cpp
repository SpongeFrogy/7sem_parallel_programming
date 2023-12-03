#include <cassert>
#include <chrono>
#include <cstddef>
#include <emmintrin.h>
#include <immintrin.h>
#include <iostream>
#include <omp.h>
#include <ostream>
#include <smmintrin.h>
#include <time.h>
#include <tmmintrin.h>
#include <xmmintrin.h>

using namespace std;

// const size_t MAXSIZE = 8192;
// const size_t MAXSIZE = 16384;
const size_t MAXSIZE = 32768;
// const size_t MAXSIZE = 10;

const int P = 2; // Number of threads for OpenMP

template <typename T> int time_measure(T func, const int *x, size_t n) {
  auto begin = chrono::steady_clock::now();
  auto res = func(x, n);
  auto end = chrono::steady_clock::now();

  cout << chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

  return res;
}

template <typename T> int time_measure_omp(T func, const int *x, size_t n) {
  auto begin = omp_get_wtime();
  auto res = func(x, n);
  auto end = omp_get_wtime();

  cout << int((end - begin) / 1e-6);

  return res;
}

//  Без использования параллельных вычислений
int LinearSumma(const int *x, size_t n) {
  int s = 0;
  for (size_t i = 0; i < n; ++i) {
    s += x[i];
  }
  return s;
}

//  С использованием SSE команд
int SseSumma(const int *x, size_t n) {
  __m128i mmSum = _mm_setzero_si128();
  size_t i = 0;

  for (; i < n - n % 4; i += 4) {
    __m128i mmValues = _mm_load_si128((__m128i *)(x + i));
    mmSum = _mm_add_epi32(mmSum, mmValues);
  }

  int res[4];
  _mm_storeu_si128((__m128i *)res, mmSum);

  return res[0] + res[1] + res[2] + res[3];
}

// Каскадный метод, последовательный режим
int CascadeSumma(const int *x, size_t n) {
  int y[MAXSIZE];
  memcpy(y, x, n * sizeof(int));
  while (n >= 2) {
    n /= 2;
    for (size_t j = 0; j < n; ++j)
      y[j] = y[2 * j] + y[2 * j + 1];
  }
  return y[0];
}

// Комбинированный метод. Цикл. - openMP
int Omp1Summa(const int *x, size_t n) {
  int *localSumma = (int *)malloc(P * sizeof(int));
  const int portionSize = n / P;
#pragma omp parallel num_threads(P)
  {
    const int id = omp_get_thread_num();
    int sum = 0;
    for (int i = id * portionSize; i < (id + 1) * portionSize; i++) {
      sum += x[i];
    }
    localSumma[id] = sum;
  }
  return CascadeSumma(localSumma, P);
}

// Комбинированный метод. Секции (для 2-х потоков). - openMP
int OmpSectionSumma(const int *x, size_t n) {
  int *localSumma = (int *)malloc(2 * sizeof(int));
  const int portionSize = n / 2;
#pragma omp parallel sections shared(localSumma, x)
  {
#pragma omp section
    {
      const int id = omp_get_thread_num();
      int sum = 0;
      for (int i = id * portionSize; i < (id + 1) * portionSize; i++) {
        sum += x[i];
      }
      localSumma[id] = sum;
    }
#pragma omp section
    {
      const int id = omp_get_thread_num();
      int sum = 0;
      for (int i = id * portionSize; i < (id + 1) * portionSize; i++) {
        sum += x[i];
      }
      localSumma[id] = sum;
    }
  }
  return localSumma[0] + localSumma[1];
}

int main() {
  omp_set_dynamic(0); // Explicitly disable dynamic teams
  int x[MAXSIZE];
  for (size_t i = 0; i < MAXSIZE; i++) {
    x[i] = i * i;
  }
  int number_of_methods = 5;
  int res_summa[number_of_methods];

  cout << "MAXSIZE: " << MAXSIZE << endl;

  cout << "linear time: ";
  res_summa[0] = time_measure(LinearSumma, x, MAXSIZE);
  cout << " [µs]" << endl;

  cout << "Sse time: ";
  res_summa[1] = time_measure(SseSumma, x, MAXSIZE);
  cout << " [µs]" << endl;

  cout << "Cascade time: ";
  res_summa[2] = time_measure(CascadeSumma, x, MAXSIZE);
  cout << " [µs]" << endl;

  cout << "Combination1 time: ";
  res_summa[3] = time_measure(Omp1Summa, x, MAXSIZE);
  cout << " [µs]" << endl;

  cout << "Combination Section time: ";
  res_summa[4] = time_measure(OmpSectionSumma, x, MAXSIZE);
  cout << " [µs]" << endl;

  for (int i = 0; i < number_of_methods; i++) {
    // cout << res_summa[i] << endl;
    assert(res_summa[i] == res_summa[(i + 1) % number_of_methods]);
  }

  return 0;
}