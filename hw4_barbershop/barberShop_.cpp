#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using namespace std;

int SIZE = 2;

class Barbershop {
public:
  condition_variable cv;
  queue<thread::id> pool;
  mutex m;
  mutex log_m;
  bool worker_is_ready = true;
  bool working_day = true;
};

class Costumer {
public:
  Barbershop &room;
  Costumer(Barbershop &Room) : room{Room} {}

  void try_get_haircut() {
    {
      lock_guard<mutex> lock(room.m);
      if (room.pool.size() >= SIZE) {
        {
          lock_guard<mutex> lock(room.log_m);
          cout << "costumer: " << this_thread::get_id() << " going home\n";
        }
        return;
      }
      room.pool.push(this_thread::get_id());

      {
        lock_guard<mutex> lk(room.log_m);
        cout << "costumer: " << this_thread::get_id() << " in queue\n";
      }
    }
    {
      unique_lock<mutex> lk(room.m);

      room.cv.notify_one();
      room.cv.wait(lk, [this] { return room.worker_is_ready; });
    }
  }
  void i_am_last() {
    this_thread::sleep_for(chrono::milliseconds(1000));
    room.working_day = false;
    room.cv.notify_all();
    try_get_haircut();
  }
};

class Barber {
public:
  Barbershop &room;

  Barber(Barbershop &Room) : room{Room} {}
  void work_day() {
    while (room.working_day) {
      unique_lock<mutex> lk(room.m);
      room.cv.wait(lk, [this] { return !room.pool.empty(); });
      room.worker_is_ready = false;

      this_thread::sleep_for(chrono::milliseconds(3));

      {
        lock_guard<mutex> lock(room.log_m);
        cout << "Worker thread is processing " << room.pool.front() << endl;
      }
      room.pool.pop();

      {
        lock_guard<mutex> lock(room.log_m);
        cout << "Worker thread signals cut completed\n";
      }

      lk.unlock();
      room.worker_is_ready = true;
      room.cv.notify_one();
    }
    {
      lock_guard<mutex> lock(room.log_m);
      cout << "last costumer\n";
    }
  };
};

Barbershop ChopChop;

int main() {
  thread s([]() { Costumer(ChopChop).i_am_last(); });
  thread worker([]() { Barber(ChopChop).work_day(); });
  thread t1([]() { Costumer(ChopChop).try_get_haircut(); }),
      t2([]() { Costumer(ChopChop).try_get_haircut(); }),
      t3([]() { Costumer(ChopChop).try_get_haircut(); }),
      t4([]() { Costumer(ChopChop).try_get_haircut(); });

  s.join();
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  worker.join();

  return 1;
}
/*
Возможный вывод:

costumer: 4 in queue
costumer: 7 in queue
costumer: 6 going home
costumer: 5 going home
Worker thread is processing 4
Worker thread signals cut completed
Worker thread is processing 7
Worker thread signals cut completed
costumer: 2 in queue
Worker thread is processing 2
Worker thread signals cut completed
last costumer
*/