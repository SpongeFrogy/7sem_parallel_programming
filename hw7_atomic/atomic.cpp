#include <atomic>
#include <chrono>
#include <iostream>
#include <synchapi.h>
#include <thread>

using namespace std;

class Treader {
public:
  Treader(int n) : _n{n} {};
  void start() {
    if (!_isRunning.load()) {
      _isRunning.store(true);
      _t = thread([this] { _thread_func(); });
    }
  }
  void stop() {
    if (_isRunning.load()) {
      _isRunning.store(false);
      if (_t.joinable()) {
        _t.join();
      }
    }
  }

private:
  void _thread_func() {
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    while (_isRunning.load()) {
      //cout << "Thread is running..." << endl;
      this_thread::sleep_for(chrono::seconds(_n));
      chrono::steady_clock::time_point end = chrono::steady_clock::now();
      cout << "Thread running " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " seconds" << endl;
    }
  };

private:
  int _n = 1;
  thread _t;
  atomic<bool> _isRunning;
};

int main(){

    Treader example = Treader(2);
    example.start();

    this_thread::sleep_for(chrono::seconds(20));
    example.stop();

    return  0;
}