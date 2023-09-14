#include <iostream>
#include <cmath>
#include <time.h>
#include <emmintrin.h>
#include <xmmintrin.h>
#include <assert.h>



using namespace std;

void sqrt_normal(float* a, int size, float* b)
{
    for (int i = 0; i < size; i++)
    {
        b[i] = sqrt(a[i]);
    }
}

void sqrt_sse(float* a, int size, float* b)
{
    for  (int i = 0; i < size; i+=4)
    {
        __m128 x = _mm_load_ps(&a[i]);
        __m128 res = _mm_sqrt_ps((__m128) x);
        _mm_store_ps(&b[i], res);

    }
}


int main()
{
    int size = 10001;
    float a[size];
    for (int i=0; i<size; i++)
    {
        a[i] = i + 1;
    }

    float res_n[size], res_s[size];
    // clock_t start_normal = clock();
    sqrt_normal(a, size, res_n);
    // clock_t end_normal = clock();
    sqrt_sse(a, size, res_s);
    // clock_t end_sse = clock();
    for (int i=0; i < size; i++)
    {
        assert(res_n[i] == res_s[i]);
    }
    // cout << "normal time :" << (double)(end_normal - start_normal) / CLOCKS_PER_SEC << endl;
    // cout << "sse time :" << (double)(end_sse - end_normal) / CLOCKS_PER_SEC << endl;
    
}


