#include <iostream>
#include <omp.h>

int main() {
    const int num_elements = 10000;
    int data[num_elements];

    for (int i = 0; i < num_elements; ++i) {
        data[i] = i;
    }

    #pragma omp parallel for
    for (int i = 0; i < num_elements; ++i) {
        data[i] *= 2;
    }

    for (int i = 0; i < num_elements; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}