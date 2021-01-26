// A very simple program to do the FFT using pocketff
// This separate file only works with vector of floats
// I am using this file to avoid recompiling pocketfft
// Plus it helps me a bit with testing
#include "pocketfft/floatfft.h"

// FFTworker::FFTworker(size_t n_els){
//     // shape_t shape{n_els};
// }

FFTworker::FFTworker(){
    // std::cout << "FFT Woker" << std::endl;

}

void FFTworker::set_fft_width(size_t fft_width){
    shape[0] = {fft_width};
}

void FFTworker::frand(vector<float> &v){
  for(float &i:v){
    i = drand48();
  }
}

FFTworker::~FFTworker(){
    // free(shape);
}