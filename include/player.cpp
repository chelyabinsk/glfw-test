// A VERY basic music player
// 1) Load audio (MP3 only)
// 2) Calculate and store FFT
// 3) Pause/Play/Rewind
#include "player.h"

// Constructor
Player::Player(){
    std::cout << "Init player" << std::endl;
    
    plan_init = false;
}

void Player::update_num_groups(size_t n){
    num_groups = n;
    gen_groups();
}

void Player::init_thread(){
    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (char*)malloc(buffer_size * sizeof(unsigned char));

    rawFFT = (double*) malloc(sizeof(double) * buffer_size/2);
    sep_vec = (double*) malloc(sizeof(double) * num_groups);

    read_file("black.mp3");  // Load file

    // // Prepare FFTw things
    // flag = 0;
    // in = (double*) fftw_malloc(sizeof(double) * buffer_size);
    // out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * buffer_size);
    // p = fftw_plan_dft_r2c_1d(buffer_size,in, out,flag);


    // fftwf_free(sep_arrs);

    plan_init = false;

    // Prepare the pocketfft class
    fftClass.set_fft_width(buffer_size);

    lazy_play();

}

void Player::gen_groups(){
    float step_size = 1.0f/num_groups;
    groups.assign(num_groups,0.0f);
    for(size_t j=0;j<num_groups;j++){
        groups[j] = 15.0f*exp(j*step_size*7.12f);
    }
}

void Player::read_file(const char* filePath){
    // Should add error handling code
    // add stuff to gracefully load another file in runtime

    /* open the file and get the decoding format */
    mpg123_open(mh, filePath);
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    // Reset song timer
    time = 0;
}

void Player::lazy_play(){
    size_t group_pos = 0;
    float freq_space = 1.0f*rate/(buffer_size);

    for(i=0;i<num_groups;i++)
        std::cout << groups[i] << " ";
    std::cout << std::endl << freq_space << " " << buffer_size << " " << rate << std::endl;

    /* decode and play */
    // Will add pause an rewind functions later
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
      ao_play(dev, buffer, done);
      time += 0.5*freq_space;
      //mpg123_tellframe  -- find current frame number
      //std::cout << mpg123_tellframe(mh) << std::endl;

    //   // Copy buffer for FFT
    //   for(i=0;i<buffer_size/2;i++)
    //     in[i] = buffer[i];

      // Do the FFT
    //   fftw_execute(p);
      
    //   group_pos = 0;
    //   for(i=0;i<num_groups;i++)
    //     sep_vec[i] = 0;
    //   // Calculate abs value of the FFT for plot
    //   for(i=0;i<buffer_size/2;i++){
    //     rawFFT[i] = 2.0f*sqrt(out[0][i]*out[0][i] + out[1][i]*out[1][i]);
    //     // std::cout << (i+1)*freq_space << std::endl;
    //     if(groups[group_pos]<=(i+1)*freq_space){
    //         group_pos++;
    //     }
    //     if(group_pos>=num_groups){
    //         group_pos = num_groups - 1;
    //     }
    //     sep_vec[group_pos] += rawFFT[i];
    //   }

    //   std::ofstream myfile;
    //   myfile.open ("fft_out.csv");
    //   for(i=0;i<buffer_size/2;i++){
    //     //std::cout << 2.0f*sqrt(out[0][i]*out[0][i] + out[1][i]*out[1][i]) << " ";
    //     // Save FFT to a text file
    //     myfile << i << "," << 2.0f*sqrt(out[i][0]*out[i][0] + out[i][1]*out[i][1]) << "\n";
    //   }
    //   myfile.close();

    // if(time > 1){
    //     break;
    // }

    //  for(i=0;i<num_groups;i++)
    //     std::cout << sep_vec[i]/pow(2,30) << " ";
    // std::cout << std::endl; 

    //   std::cout << groups[num_groups-1] << std::endl;
    //   std::cout << rawFFT[1] << std::endl;
    //   std::cout << time << std::endl;
    }
    //time += 0.25*buffer_size/rate;
    std::cout << time << std::endl;

    /* clean up */
    // ao_close(dev); // Good

}

void Player::start_thread(const std::string &tname)
{
    //std::thread thrd = std::thread(&Player::sleep_for, this, tname, 3600);
    std::thread thrd = std::thread(&Player::init_thread, this);
    thrd.detach();
    tm_[tname] = std::move(thrd);
    std::cout << "Thread " << tname << " created:" << std::endl;
}

void Player::stop_thread(const std::string &tname)
{
    ThreadMap::const_iterator it = tm_.find(tname);
    if (it != tm_.end()) {
        it->second.std::thread::~thread(); // thread not killed
        tm_.erase(tname);
        std::cout << "Thread " << tname << " killed:" << std::endl;
    }
}


// Deconstructor
Player::~Player(){
    // Cleaup
    if(plan_init){
        // fftw_free(in);
        // fftw_free(out);
        // fftw_destroy_plan(p);
    }
    
    free(buffer);
    free(rawFFT);
    free(sep_vec);

    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    
    ao_shutdown();
}