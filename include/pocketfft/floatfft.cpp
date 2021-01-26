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
  // Initialising stuff
  // I need to do weird resizings because I can't C++ :(
    shape.resize(1);
    shape[0] = {fft_width};
    resf.resize(fft_width);
}

void FFTworker::frand(vector<float> &v){
  for(float &i:v){
    i = drand48();
  }
}

void FFTworker::do_fft(vector<float> in, vector<float> &out){
  r2c(shape, stridein, strideout, axes, FORWARD,
        in.data(), resf.data(), 1.f);
  for(int i=0;i<out.size();++i){
      out[i] = 2.f*abs(resf[i]);
      // cout << 2*abs(resf[i]) << endl;
    }
}

FFTworker::~FFTworker(){
    // free(shape);
}