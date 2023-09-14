#include <iostream>
#include <cmath>
#include <time.h>
#include <emmintrin.h>
#include <xmmintrin.h>
#include <assert.h>

#include <limits>
#include <iomanip>
typedef std::numeric_limits< float > dbl;


using namespace std;

float dot_normal(float *x,float *y,int n)
{
    float s;
    int i;
    s=0;
    for(i=0;i<n;i++) 
    s+=x[i]*y[i];
return s;
}

float dot_sse(float* a,float* b,int n)
{
    int i = 0;
    float res_sse[4];
    float res = 0;
    __m128 sum = _mm_setzero_ps();
    for(;i<n-3;i+=4)
    {
        __m128 x = _mm_load_ps(&a[i]);
        __m128 y = _mm_load_ps(&b[i]);
        __m128 mul = _mm_mul_ps(x, y);
        sum = _mm_add_ps(sum, mul);

    }
    for (int i0=i;i0<n;i0++)
    {
        res+=a[i0]*b[i0];
    }
    _mm_store_ps(res_sse, sum);
    res+=res_sse[0] + res_sse[1] + res_sse[2] + res_sse[3];
return res;
}

int main()
{
    int size = 10001;
    float a[size], b[size];
    for (int i=0; i<size; i++)
    {
        a[i] = i*0.000001;
        b[i] = i*0.000342;
    }
    // clock_t start_normal = clock();
    float res_n = dot_normal(a, b, size);
    // clock_t end_normal = clock();
    float res_s = dot_sse(a, b, size);
    // clock_t end_sse = clock();
    cout << setprecision(numeric_limits<float>::digits10 + 2) << res_n << std::endl;
    std::cout << std::setprecision(std::numeric_limits<float>::digits10 + 2) << res_s << std::endl;
    //cout << res_n << endl << res_s << endl << res_g;
    // cout << "normal time :" << (double)(end_normal - start_normal) / CLOCKS_PER_SEC << endl;
    // cout << "sse time :" << (double)(end_sse - end_normal) / CLOCKS_PER_SEC << endl;
    
}