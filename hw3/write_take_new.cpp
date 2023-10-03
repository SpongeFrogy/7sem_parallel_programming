#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

using namespace std;

class Message
{        
    private:
        std::mutex _m;
        std::vector<int> _pool;
    
    public:
        void set(const int &value)
        {
            lock_guard<mutex> lock(_m);
            _pool.push_back(value);
            cout << "Set value: " << value << " from: " << this_thread::get_id() << endl; 
        }

        int getAndRemove()
        {
            lock_guard<mutex> lock(_m);
            if (_pool.empty()){throw length_error("pool is empty");}
            
            int value = this->_pool.back();
            _pool.pop_back();
            cout << "Get and remove value: " << value << " from: " << this_thread::get_id() << endl;
            return value;
        }
};

Message resourse;


int main()
{
    thread t1([](){resourse.set(5);});
    thread t2([](){resourse.getAndRemove();});
    
    t1.join();
    t2.join();

    return 1;
}