#ifndef FLOATFFT_H
#define FLOATFFT_H
#include <complex>
#include <cmath>
#include <vector>
#include <iostream>
#include <time.h>
#include "pocketfft/pocketfft_hdronly.h"

using namespace std;
using namespace pocketfft;

class FFTworker{
public:
    // Constructor
    FFTworker();
    // FFTworker(size_t n_els);

    // Deconstructor
    ~FFTworker();

    void set_fft_width(size_t fft_width);

    void do_fft(vector<float> in, vector<float> &out);

    size_t num_groups;
    vector<size_t> shape;
    
private:
    // Generate vector of random floats
    void frand(vector<float> &v);

    stride_t stridein{sizeof(float)};
    stride_t strideout{sizeof(complex<float>)};
    shape_t axes{0};
    vector<complex<float>> resf;
};
#endif