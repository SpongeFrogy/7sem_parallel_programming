#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
class Set
{
    private:
        vector<T> data;
    public:
        bool add(T value)
        {
            if (find(data.begin(), data.end(), value) == data.end())
            {
                data.push_back(value);
                return  true;
            }
            return false;
        }

        bool remove(T value)
        {
            auto it = find(data.begin(), data.end(), value);
            if (it != data.end())
            {
                data.erase(it);
                return true;
            }
            return false;
        }
        bool contains(T value)
        {
            return find(data.begin(), data.end(), value) != data.end();
        }
};

int main()
{
    Set<int> set;
    set.add(2);
    cout << "add new value to set: " << set.add(1) << endl;
    cout << "add to set value that already in it: " << set.add(1) << endl;
    cout << "remove value: " << set.remove(1) << endl;
    cout << "remove value that not in set: " << set.remove(1) << endl;
    cout << "contains value that contains: " << set.contains(2) << endl;
    cout << "contains value that not contains: " << set.contains(3) << endl;


    return 0;
}