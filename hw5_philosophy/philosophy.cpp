#include <chrono>
#include <iostream>
#include <mutex>
#include <synchapi.h>
#include <thread>

using namespace std;

int N_FORK = 4;
int N_PHILOSOPHER = 5;

mutex log_m;

class Fork {
public:
  mutex m;
};

class Philosopher {
public:
  Fork &left, &right;
  Philosopher(Fork &left, Fork &right) : left(left), right(right){};
  void eat() {
    scoped_lock lock(left.m, right.m);
    this_thread::sleep_for(chrono::milliseconds(20));
    {
      lock_guard<mutex> lk(log_m);
      cout << "Philosopher " << this_thread::get_id() << " ate\n";
    }
  };
};

int main() {

  Fork fork[N_FORK];
  thread phi[N_PHILOSOPHER];

  for (int i = 0; i < N_PHILOSOPHER; i++) {
    phi[i] = thread(
        [i, &fork]() { Philosopher(fork[i], fork[(i + 1) % N_FORK]).eat(); });
  }
  for (int i = 0; i < N_PHILOSOPHER; i++) {
    phi[i].join();
  }

  return 1;
}