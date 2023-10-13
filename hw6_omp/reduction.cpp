#include <iostream>
#include <omp.h>

int main() {
    const int num_elements = 10000;
    int data[num_elements];
    int sum = 0;

    // Initialize data
    for (int i = 0; i < num_elements; ++i) {
        data[i] = i;
    }

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < num_elements; ++i) {
        sum += data[i];
    }

    std::cout << "Sum: " << sum << std::endl;

    return 0;
}