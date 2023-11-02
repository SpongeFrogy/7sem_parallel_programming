#include <chrono>
#include <iostream>
#include <omp.h>
#include <vector>


using namespace std;

int N = 30000;

void test() {
  vector<vector<int>> v(N, vector<int>(N, 0));

#pragma omp parallel shared(v)
  {
#pragma omp sections
    {
#pragma omp section
      {
        for (int i = 0; i < v.size(); i++) {
          for (int j = 0; j < v[i].size(); j++) {
            v[i][j] = i + j;
          }
        }
      }

#pragma omp section
      for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
          // cout << omp_get_thread_num() << ": " << v[i][j] << endl;
          v[i][j]++;
        }
      }
    }
  }
}

void test_no() {
  vector<vector<int>> v(N, vector<int>(N, 0));
  {
    {
      for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
          v[i][j] = i + j;
        }
      }
    }

    {
      for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
          // cout << omp_get_thread_num() << ": " << v[i][j] << endl;
          v[i][j]++;
        }
      }
    }
  }
}

template <typename T> long time_m(T f) {

  chrono::steady_clock::time_point begin = chrono::steady_clock::now();
  f();
  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end - begin).count();
  return duration;
}

int main() {
  //   omp_set_dynamic(0);
  omp_set_num_threads(4);
  cout << "time OpenMP " << time_m(test) << " ms.\n";
  cout << "time linear " << time_m(test_no) << " ms.";

  return 0;
}
/*
time OpenMP 11366 ms.
time linear 18049 ms.
*/