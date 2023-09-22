#include <Windows.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

using namespace std;

struct ThreadData {
    int cols;
    int* row;
    int product;
    int seed;
};

DWORD WINAPI threadProc(LPVOID lpParameter) {
    ThreadData* data = (ThreadData*)lpParameter;
    srand(data->seed); // set random seed
    int* row = data->row; // read thread data
    int product = 1;
    int flag = -1;
    for (int i = 0; i < data->cols; i++)
    {
        int value = rand() % 10;
        row[i] = value;
        if (value % 2 == 1)
        {
            flag = 1; // at list one value exists
            product *= value;
        }
    }
    if (flag == -1) product = -1; // * if there are no values (maybe there some better solutions) 
    data->product = product;
    return 0;
}

int main() {
    int rows = 6;
    int cols = 6;

    int matrix[rows][cols];
    HANDLE threads[rows];
    ThreadData data[rows];
    for (int i = 0; i < rows; i++) // set initial data for threads 
    {
        data[i].row = matrix[i];
        data[i].cols = cols;
        data[i].seed = rand(); // * for randomizing in threads (without setting seed all threads create same "random" values)
    }
    //cout << "threads start" << endl;
    for (int i = 0; i < rows; i++) // create threads
    {

        threads[i] = CreateThread(NULL, 0, threadProc, &data[i], 0, NULL);
        if (threads[i] == NULL) {
            cout << "Error creating thread " << i << endl;
            return 1;
        }
    }

    for (int i = 0; i < rows; i++) {
        WaitForSingleObject(threads[i], INFINITE); // wait till threads[i] is done 
        cout << "Row " << i << ": ";
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "Product: " << data[i].product << endl;
    }

    return 0;
}