#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <queue>

using namespace std;

int SIZE = 2;


class BarberShop
{
    private:
    condition_variable _cv;
    queue<thread::id> _pool;
    mutex _m;
    mutex _log_m;
    //bool _ready = false;
    //bool _processed = true;
    bool _working_day = true;
    bool _worker_is_ready = true;

    public:
    void barber()
    {
    while (_working_day)
    {
        std::unique_lock<std::mutex> lk(_m);
        _cv.wait(lk, [this]{return !_pool.empty();});
        _worker_is_ready = false;

        this_thread::sleep_for(chrono::milliseconds(3));
        
        {lock_guard<mutex> lock(_log_m);
        std::cout << "Worker thread is processing " << _pool.front() << endl;}
        _pool.pop();

        {lock_guard<mutex> lock(_log_m);
        std::cout << "Worker thread signals cut completed\n";}

        lk.unlock();
        _worker_is_ready = true;
        _cv.notify_one();
        }
        {lock_guard<mutex> lock(_log_m);
        cout << "last costumer\n";}
    };
    void costumer()
    {
        {
        lock_guard<mutex> lock(_m);
        if (_pool.size() >= SIZE)
        {
            {lock_guard<mutex> lock(_log_m);
            cout << "costumer: " << this_thread::get_id() << " going home\n";}
            return;
        }
        _pool.push(this_thread::get_id());

        {lock_guard<mutex> lk(_log_m);
        cout << "costumer: " << this_thread::get_id() << " in queue\n";}

        }
        {
            std::unique_lock<std::mutex> lk(_m);

            
            _cv.notify_one();
            _cv.wait(lk, [this]{return _worker_is_ready;});

            // {lock_guard<mutex> lock(log_m);
            // cout << "costumer: " << this_thread::get_id() << " going to worker\n";}
        }
    }
    void stop_thread()
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
        _working_day = false;
        _cv.notify_all();
        costumer();
    }
};

BarberShop ChopChop;

int main()
{
    thread s([](){ChopChop.stop_thread();});
    thread worker([](){ChopChop.barber();});
    thread t1([](){ChopChop.costumer();}), t2([](){ChopChop.costumer();}), t3([](){ChopChop.costumer();}), t4([](){ChopChop.costumer();});

    s.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    worker.join();
}
/*
возможный вывод:

costumer: 7 in queue
costumer: 5 in queue
costumer: 6 going home
costumer: 4 going home
Worker thread is processing 7
Worker thread signals cut completed
Worker thread is processing 5
Worker thread signals cut completed
costumer: 2 in queue
Worker thread is processing 2
Worker thread signals cut completed
last costumer
*/