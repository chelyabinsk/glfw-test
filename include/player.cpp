// A VERY basic music player
// 1) Load audio (MP3 only)
// 2) Calculate and store FFT
// 3) Pause/Play/Rewind
#include "player.h"

// Constructor
Player::Player(){
    std::cout << "Init player" << std::endl;
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

    read_file("black.mp3");  // Load file

    // Prepare FFTw things
    flag = 0;
    in = (float*) fftwf_malloc(sizeof(float) * buffer_size);
    out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * buffer_size);
    p = fftwf_plan_dft_r2c_1d(buffer_size,in, out,flag);

    rawFFT = (float*) malloc(sizeof(float) * buffer_size/2);
    sep_vec = (float*) malloc(sizeof(float) * num_groups);

    // fftwf_free(sep_arrs);

    lazy_play();

}

void Player::gen_groups(){
    float step_size = 1.0f/num_groups;
    groups.assign(num_groups,0.0f);
    for(size_t j=0;j<num_groups;j++){
        groups[j] = 15.0f*exp(j*step_size*7.22f);
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
    float freq_space = buffer_size/rate;
    /* decode and play */
    // Will add pause an rewind functions later
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
      ao_play(dev, buffer, done);
      time += 0.25*freq_space;
      //mpg123_tellframe  -- find current frame number
      //std::cout << mpg123_tellframe(mh) << std::endl;

      // Copy buffer for FFT
      for(i=0;i<buffer_size/2;i++)
        in[i] = buffer[i];

      // Do the FFT
      fftwf_execute(p);
      
      group_pos = 0;
      for(i=0;i<num_groups;i++)
        sep_vec[i] = 0;
      // Calculate abs value of the FFT for plot
      for(i=0;i<buffer_size/2;i++){
        rawFFT[i] = 2.0f*sqrt(out[0][i]*out[0][i] + out[1][i]*out[1][i]);
        if(groups[group_pos]<=(i+1)*freq_space)
            group_pos++;
        if(group_pos>=num_groups)
            group_pos = num_groups - 1;
        sep_vec[group_pos] += rawFFT[i];
      }

     for(i=0;i<num_groups;i++)
        std::cout << sep_vec[i] << " ";
    std::cout << std::endl; 

    //   std::cout << groups[num_groups-1] << std::endl;
    //   std::cout << rawFFT[1] << std::endl;
    //   std::cout << time << std::endl;
    }
    //time += 0.25*buffer_size/rate;
    std::cout << time << std::endl;

    /* clean up */
    ao_close(dev); // Good
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
    fftwf_destroy_plan(p);
    fftwf_free(out);
    fftwf_free(in);
    free(buffer);
    free(rawFFT);
    free(sep_vec);
    // ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();
}