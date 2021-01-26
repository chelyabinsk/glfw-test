#include <complex>
#include <cmath>
#include <vector>
#include <iostream>
#include <time.h>
#include "pocketfft/pocketfft_hdronly.h"

using namespace std;
using namespace pocketfft;

template<typename T> void crand(vector<complex<T>> &v)
  {
    for (auto & i:v){
      i = complex<T>(drand48()-0.5, drand48()-0.5);
    }
  }

void frand(vector<float> &v){
  for(float &i:v){
    i = drand48();
  }
}

template<typename T1, typename T2> long double l2err
  (const vector<T1> &v1, const vector<T2> &v2)
  {
  long double sum1=0, sum2=0;
  for (size_t i=0; i<v1.size(); ++i)
    {
    long double dr = v1[i].real()-v2[i].real(),
                di = v1[i].imag()-v2[i].imag();
    long double t1 = sqrt(dr*dr+di*di), t2 = abs(v1[i]);
    sum1 += t1*t1;
    sum2 += t2*t2;
    }
  return sqrt(sum1/sum2);
  }


int main()
  {
    // Set random seed
    // srand48((unsigned int)time(NULL));

    size_t size = 9;
    vector<size_t> shape{size};

    stride_t stridein{sizeof(float)};

    stride_t strideout{sizeof(complex<float>)};
    size_t tmpout = sizeof(complex<float>);

    vector<float> dataf(size);

    // Generate random data
    // crand(dataf);
    frand(dataf);
    for(int i=0;i<9;++i){
      cout << dataf[i] << endl;
    }

    shape_t axes{0};

    vector<complex<float>> resf(size/2+1);

    r2c(shape, stridein, strideout, axes, FORWARD,
        dataf.data(), resf.data(), 1.f);
    
    cout << "!!" << endl;
     for(int i=0;i<resf.size();++i){
      cout << 2*abs(resf[i]) << endl;
    }
  }
