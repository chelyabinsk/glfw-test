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
    buffer.resize(buffer_size);
    in.resize(buffer_size/2);
    
    // Might want to do the grouping in the plot class
    sep_vec = (double*) malloc(sizeof(double) * num_groups);

    read_file("black.mp3");  // Load file

    // Prepare the pocketfft class
    fftClass.set_fft_width(buffer_size/2);

    // Start the actual player
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
    format.channels = channels; // Try to force MONO
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);
    
    std::cout << "channels=" << channels << std::endl;

    // I DON'T KNOW IF THIS IS RIGHT!
    // BUT IT LOOKS LIKE NUMBER OF CHANNELS AFFECTS HOW MUCH DATA IS READ
    // PROBABLY WRONG!
    rawFFT.resize((buffer_size/(2*2*pow(2,channels-1))) +1);
    in.resize(buffer_size/(2*pow(2,channels-1)));  // div by 2 because 2x char = short int

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
    while (mpg123_read(mh, &buffer[0], buffer_size, &done) == MPG123_OK){
      ao_play(dev, &buffer[0], done);
      time += 0.5*freq_space;
      //mpg123_tellframe  -- find current frame number
      //std::cout << mpg123_tellframe(mh) << std::endl;

      // "clever" way to change (2x char)=(short int) INTO float
      short* tst = reinterpret_cast<short*>(&buffer[0]);
      in.assign(tst,tst+buffer_size/2);

    // TODO: Find a way to convert stereo to mono ...
    //   if(channels>1){
    //       for(i=0;i<in.size();++i){
    //           in[i] += buffer[i+(buffer_size/2)/2];
    //       }
    //   }


      // Do the FFT
      fftClass.do_fft(in,rawFFT);
    
    //   std::ofstream myfile;
    //   myfile.open ("fft_out.csv");
      
    //   for(i=0;i<rawFFT.size();++i){
    //     //std::cout << 2.0f*sqrt(out[0][i]*out[0][i] + out[1][i]*out[1][i]) << " ";
    //     // Save FFT to a text file
    //     myfile << i << "," << 2.f*abs(rawFFT[i]) << "\n";
    //   }
    //   myfile.close();

    // break;

    }
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
    free(sep_vec);

    // MP3 decoder
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    
    // Sound maker
    // ao_close(dev);  // This sometimes crashes :(
    ao_shutdown();
}