#include <chrono>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <synchapi.h>
#include <thread>
#include <time.h>
#include <vector>
#include <mutex>

using namespace std;

vector<int> pool;

timed_mutex m;
timed_mutex log_m;
chrono::milliseconds timeout = 100ms;

void writeFunc()
{
    if (m.try_lock_for(timeout))
    {
        int v = 7;  
        pool.push_back(v);
        m.unlock();
    }
    else
    {
        log_m.lock();
        cout << "Writer Thread: Failed access" << endl;
        log_m.unlock();
    }
}

void long_writeFunc()
{
    if (m.try_lock_for(timeout))
    {
        //cout << m.try_lock() << endl;
        Sleep(150);
        int v = 7;  
        pool.push_back(v);
        m.unlock();
    }
    else
    {
        cout << "Writer Thread: Failed access" << endl;
    }
}

void empty_writeFunc()
{
    if (m.try_lock_for(timeout))
    {
        // empty work
        m.unlock();
    }
    else
    {
        cout << "Writer Thread: Failed access" << endl;
    }
}

void grabFunc()
{
    if (m.try_lock_for(timeout))
    {
        if (pool.empty())
        {
            throw length_error("pool is empty");
        }

        int v = pool.back();
        pool.pop_back();
        log_m.lock();
        cout << "Taken data is: " << v << endl;
        log_m.unlock();
        m.unlock();

    }
    else
    {
        cout << "Reader Thread: Failed access" << endl;
    }
}



int main()
{

    thread t1(writeFunc);
    thread t2(grabFunc);
    t1.join();
    t2.join();

    cout << "--------------------------------------" << endl;
    thread t3(long_writeFunc);
    thread t4(grabFunc);
    t3.join();
    t4.join();

    pool.pop_back();
    cout << "--------------------------------------" << endl;
    
    
    thread t5(empty_writeFunc);
    thread t6(grabFunc);
    t5.join();
    t6.join();

// writeFunc is donevalue is: 
// 7
// grabFunc is done
// --------------------------------------
// failed access
// grabFunc is done
// long_writeFunc is done
// --------------------------------------
// empty work
// empty_writeFunc is done
// terminate called after throwing an instance of 'std::length_error'
//   what():  pool is empty
    return 1;
}