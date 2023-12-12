#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

__device__ int res = 0; 

__device__ bool condition(int i, int j) {
    return i == j;
}

__global__ void countThreads(int rows, int cols) {
    int i = blockIdx.y * blockDim.y + threadIdx.y;
    int j = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < rows && j < cols) {
        if (condition(i, j)) {
            atomicAdd(&res, 1);
        }
    }
}

int main() {
    int rows = 10;
    int cols = 10;

    int numThreadsPerBlock = 16;
    int numBlocksX = (cols + numThreadsPerBlock - 1) / numThreadsPerBlock;
    int numBlocksY = (rows + numThreadsPerBlock - 1) / numThreadsPerBlock;

    dim3 gridSize(numBlocksX, numBlocksY);
    dim3 blockSize(numThreadsPerBlock, numThreadsPerBlock);

    countThreads<<<gridSize, blockSize>>>(rows, cols);

    cudaDeviceSynchronize();

    int result;
    cudaMemcpyFromSymbol(&result, res, sizeof(int));

    printf("Number of threads satisfying the condition: %d\n", result);

    getchar();

    return 0;
}