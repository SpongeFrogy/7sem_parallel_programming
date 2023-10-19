#include <chrono>
#include <iostream>
#include <mutex>
#include <synchapi.h>
#include <thread>

using namespace std;

int N_FORK = 4;
int N_PHILOSOPHER = 5;

mutex log_m;

struct Fork {
  mutex m;
};

class Philosopher {
public:
  Philosopher(Fork *left, Fork *right)
      : _left(left), _right(right){}; // передача нужных вилок

  ~Philosopher() {
    if (isFinishedEat()) {
      _th.join();
    };
    {
      //lock_guard<mutex> lk(log_m);
      cout << "Philosopher deleted\n";
    }
  }; // тоже проверить закончил ли есть
  void eat() {
    _th = thread([this] { _processEat(); });
  }; // тут происходит запуск потока

  bool isFinishedEat() {
    return _th.joinable();
  }; // тут проверка закончил ли он есть

private:
  void _processEat() {
    scoped_lock lock(_left->m, _right->m);
    this_thread::sleep_for(chrono::milliseconds(20));
    {
      lock_guard<mutex> lk(log_m);
      cout << "Philosopher " << this_thread::get_id() << " ate\n";
    }
  }; // функция потока, отвечает за процесс еды

private:
  Fork *_left = nullptr; // поля в классах всегда приватные
  Fork *_right = nullptr;
  thread _th;
};

int main() {

  Fork fork[N_FORK];
  Philosopher *phi[N_PHILOSOPHER];

  for (int i = 0; i < N_PHILOSOPHER; i++) {
    phi[i] = new Philosopher(&fork[i], &fork[(i + 1) % N_FORK]);
  }

  for (int i = 0; i < N_PHILOSOPHER; i++) {
    phi[i]->eat(); // start each philosopher eating
  }

  Sleep(10000);
  //this_thread::sleep_for(chrono::minutes(1));

  // for (int i = 0; i < N_PHILOSOPHER; i++) {
  //     delete phi[i]; // don't forget to clean up when you're done
  // }

  return 0;
}