#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "time.h"
#include <chrono>
#include <queue>
#include <stack>

using namespace std;


mutex m;
mutex p_m;
mutex log_m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = true;
bool working_day = true;
bool worker_is_ready = true;
queue<thread::id> pool;
int SIZE = 2;



void worker_thread()
{
    while (working_day)
    {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{return !pool.empty();});
    worker_is_ready = false;

    this_thread::sleep_for(chrono::milliseconds(10));
    
    {lock_guard<mutex> lock(log_m);
    std::cout << "Worker thread is processing " << pool.front() << endl;}
    pool.pop();

    processed = true;
    {lock_guard<mutex> lock(log_m);
    std::cout << "Worker thread signals cut completed\n";}

    lk.unlock();
    worker_is_ready = true;
    cv.notify_one();
    processed = false;
    }
    {lock_guard<mutex> lock(log_m);
    cout << "last costumer\n";}
}


void costumer()
{
    {
    lock_guard<mutex> lock(m);
    if (pool.size() >= SIZE)
    {
        {lock_guard<mutex> lock(log_m);
        cout << "costumer: " << this_thread::get_id() << " going home\n";}
        return;
    }
    pool.push(this_thread::get_id());

    {lock_guard<mutex> lk(log_m);
    cout << "costumer: " << this_thread::get_id() << " in queue\n";}

    }
    {
        std::unique_lock<std::mutex> lk(m);

        
        cv.notify_one();
        cv.wait(lk, []{return worker_is_ready;});

        // {lock_guard<mutex> lock(log_m);
        // cout << "costumer: " << this_thread::get_id() << " going to worker\n";}
    }
}

void stop_thread()
{
    this_thread::sleep_for(chrono::milliseconds(1000));
    working_day = false;
    cv.notify_all();
    costumer();
}


int main()
{
    thread s(stop_thread);
    thread worker(worker_thread);
    thread t1(costumer), t2(costumer), t3(costumer), t4(costumer);

    s.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    worker.join();
}
/*
costumer: 6 in queue
costumer: 5 in queue
costumer: 4 going home
costumer: 7 going home
Worker thread is processing 6
Worker thread signals cut completed
Worker thread is processing 5
Worker thread signals cut completed
costumer: 2 in queue
Worker thread is processing 2
Worker thread signals cut completed
last costumer
*/