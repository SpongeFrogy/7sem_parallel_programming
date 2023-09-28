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
        cout << "failed access" << endl;
    }
}

void long_writeFunc()
{
    if (m.try_lock_for(timeout))
    {
        Sleep(150);
        int v = 7;  
        pool.push_back(v);
        m.unlock();
    }
    else
    {
        cout << "failed access" << endl;
    }
}

void empty_writeFunc()
{
    if (m.try_lock_for(timeout))
    {
        cout << "empty work" << endl;
        m.unlock();
    }
    else
    {
        cout << "failed access" << endl;
    }
}

void grabFunc()
{
    if (m.try_lock_for(timeout))
    {
        if (pool.empty())
        {
            throw length_error(" pool is empty");
        }

        int v = pool.back();
        pool.pop_back();
        
        cout << "value is: " << v << endl;
        m.unlock();

    }
    else
    {
        cout << "failed access" << endl;
    }
    

}



int main()
{

    thread t1(writeFunc);
    thread t2(grabFunc);
    t1.join();
    t2.join();

    Sleep(100);
    cout << "--------------------------------------" << endl;
    thread t3(long_writeFunc);
    thread t4(grabFunc);
    t3.join();
    t4.join();

    pool.pop_back();
    Sleep(100);
    cout << "--------------------------------------" << endl;
    
    
    thread t5(empty_writeFunc);
    thread t6(grabFunc);
    t5.join();
    t6.join();

// value is: 7
// --------------------------------------
// failed access
// --------------------------------------
// empty work
// terminate called after throwing an instance of 'std::length_error'
//   what():   pool is empty
    return 1;
}