#ifndef PLAYER_H
#define PLAYER_H

#include <ao/ao.h>  // Produce audio
#include <mpg123.h>  // Stream and decode MP3
#include <fftw3.h>  // Include FFT
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>

#include <thread>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include <cstdlib>
//#include <pthread.h>
#include <sys/prctl.h>

// Output file writer
#include <iostream>
#include <fstream>

#include "pocketfft/floatfft.h"

#define BITS 8

using namespace std; 

class Player{
public:
    int channels;
    long rate;
    size_t buffer_size;
    float time;
    vector<float> rawFFT;

    double *sep_vec;
    
    size_t num_groups;

    Player();
    ~Player();

    void start_thread(const std::string &tname);
    void stop_thread(const std::string &tname);
    void update_num_groups(size_t n);
private:
    typedef std::unordered_map<std::string, std::thread> ThreadMap;
    ThreadMap tm_;

    void init_thread();
    void read_file(const char* filePath);
    void lazy_play();
    void gen_groups();

    std::vector<float> groups;

    // Audio player
    size_t i;
    mpg123_handle *mh;
    std::vector<char> buffer;
    size_t done;
    int err;

    int driver;
    ao_device *dev;
    ao_sample_format format;
    int encoding;

    // pocketfft stuff
    FFTworker fftClass;

    vector<float> in;
};

#endif