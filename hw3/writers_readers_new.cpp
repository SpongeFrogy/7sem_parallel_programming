#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <synchapi.h>
#include <thread>
#include <time.h>
#include <vector>
#include <string>


using namespace std;

mutex m;
shared_timed_mutex shared_m;


class Message
{
    public:
        void set(int value) {this->_set(value);};
        int read() {return this->_read();};
        
    private:
        mutex _m;
        mutex _log_m;
        shared_timed_mutex _shared_m;
        vector<int> _pool = {0};

        void _set(const int &value)
        {
            lock_guard<mutex> lock(_m);
            Sleep(rand());
            this->_pool.push_back(value);

            this ->_cout("Set value: ", value, this_thread::get_id());
        }
        int _read()
        {
            if (this->_pool.empty())
            {
                this->_cout("Reader Thread: pool len is ", 0, this_thread::get_id());
            }
            lock_guard<shared_timed_mutex> lock(_shared_m);
            Sleep(rand());
            int value = this->_pool.back();
            auto id = this_thread::get_id();
            this ->_cout("read value: ", value, id);
            return value;
        }

        void _cout(string mes, int s, thread::id id)
        {
            lock_guard<mutex> lock(_log_m);
            cout << mes << s << " from: " << id << endl;
        }
};

Message resourse;

int main()
{
    
    const int numThreads = 5;

    vector<thread> writers;

    vector<thread> readers;

    for (int i = 0; i < numThreads; ++i) {
        writers.emplace_back([](int id){resourse.set(id);}, i);
        readers.emplace_back([](int id){resourse.read();}, i+numThreads);
    }

    for (std::thread& thread : writers) {
        thread.join();
    };
    for (std::thread& thread : readers) {
        thread.join();
    }

    return 1;
}