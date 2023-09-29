#include <iostream>
#include <mutex>
#include <ostream>
#include <shared_mutex>
#include <synchapi.h>
#include <thread>
#include <time.h>

using namespace std;

shared_timed_mutex m;  // Create a shared mutex

mutex log_m;

chrono::milliseconds timeout = 100ms;

int sharedData = 0;  // Shared data

void readerThread() {
    if (m.try_lock_shared_for(timeout)) // Lock for shared (read) access
    {
        log_m.lock();
        cout << "Reader Thread: Reading sharedData = " << sharedData << endl;
        log_m.unlock();
        m.unlock_shared();

    }
    else 
    {
        log_m.lock();
        cout << "Reader Thread: Failed access" << endl;
        log_m.unlock();
    }
    // Shared data can be read concurrently by multiple reader threads
}

void writerThread() {
    if (m.try_lock_for(timeout)) // Lock for unique (write) access
    {
        Sleep(1);
        sharedData++;  // Modify shared data
        log_m.lock();
        cout << "Writer Thread: Incremented sharedData to " << sharedData << endl;
        log_m.unlock();
        m.unlock();
    }
    else
    {
        log_m.lock();
        cout << "Writer Thread: Failed access" << endl;
        log_m.unlock();
    }
}

int main() {
    thread reader1(readerThread);
    thread reader2(readerThread);
    thread writer1(writerThread);
    thread writer2(writerThread);

    thread reader3(readerThread);
    reader1.join();
    
    reader2.join();
    reader3.join();
    writer1.join();
    writer2.join();

    return 0;
}