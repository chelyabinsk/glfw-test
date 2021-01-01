#include <ao/ao.h>
#include <mpg123.h>
#include <fftw3.h>  // Inlcude the FFT
#include <iostream>
#include <math.h>

#define BITS 8

int main(int argc, char *argv[])
{
    mpg123_handle *mh;
    char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;

    if(argc < 2)
        exit(0);

    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = 2*mpg123_outblock(mh);
    buffer = (char*)malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    mpg123_open(mh, argv[1]);
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);
    
    double time = 0;

    // Initialise FFT
    float *in;
    fftwf_complex *out;
    fftwf_plan p;
    unsigned flag;

    in = (float*) fftwf_malloc(sizeof(float) * buffer_size);
    out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * buffer_size);

    //p = fftwf_plan_dft_r2c_1d(buffer_size, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    //p = fftwf_plan_dft_r2c_1d(buffer_size, in, out,flag);
    p = fftwf_plan_dft_r2c_1d(buffer_size,in, out,flag);

    /* decode and play */
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
      ao_play(dev, buffer, done);
      time += 0.25*buffer_size/rate;
      //mpg123_tellframe  -- find current frame number
      std::cout << mpg123_tellframe(mh) << std::endl;

      // Copy buffer for FFT
      for(int i=0;i<buffer_size/2;i++)
        in[i] = buffer[i];

      // Do the FFT
      fftwf_execute(p);

      for(int i=0;i<buffer_size/4;i++)
        std::cout << 2.0f*sqrt(out[0][i]*out[0][i] + out[1][i]*out[1][i]) << " ";
      

      // Try to do FFT on the buffer


      break;
    }
    //time += 0.25*buffer_size/rate;
    std::cout << time << std::endl;

    /* clean up */
    fftwf_destroy_plan(p);
    fftwf_free(out);
    fftwf_free(in);
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

    return 0;
}