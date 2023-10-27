#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;

template <typename T>
T max_omp(const vector<T> &data) {
  T max_value = data[0];

#pragma omp parallel for
  for (int i = 0; i < data.size(); i++) {
    
#pragma omp critical
    {
      if (data[i] > max_value) {
        max_value = data[i];
      }
    }
  }
  return max_value;
}

template <typename T>
T min_omp(const vector<T> &data) {
  T min_value = data[0];

#pragma omp parallel for
  for (int i = 0; i < data.size(); i++) {

#pragma omp critical
    {
      if (data[i] < min_value) {
        min_value = data[i];
      }
    }
  }
  return min_value;
}

int main() {

  vector<int> data(10, 0);
  srand(time(0));
  generate(data.begin(), data.end(), rand);

  cout << "vector: ";
  for (int v : data) {
    cout << v << ", ";
  }
  cout << endl;

  cout << "Minimum value: " << min_omp(data) << endl;
  cout << "Maximum value: " << max_omp(data) << endl;

  return 0;
}