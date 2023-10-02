#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

class Message
{
    public:
        void set(const int &value) {this->_set(value);};
        int getAndRemove() {return this->_getAndRemove();};
        
    private:
        std::mutex _m;
        std::vector<int> _pool;

        void _set(int value)
        {
            lock_guard<mutex> lock(_m);
            this->_pool.push_back(value);
            cout << "Set value: " << value << " from: " << this_thread::get_id() << endl; 
        }
        int _getAndRemove()
        {
            lock_guard<mutex> lock(_m);
            int value = this->_pool.back();
            this->_pool.pop_back();
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