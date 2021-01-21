//#include "shader.h"
#include "plot/surface.h"
// Might also include the fft processing class here

// Surface constructor
Surface::Surface() {
  size_t n_groups = 10;

  circlePlot.gen_bars(n_groups,0.2f);

  std::string keyword("player_thread");
  tname1 = keyword;

  playerClass.update_num_groups(n_groups);
  // Start player thread
  playerClass.start_thread(tname1);
}

// frame logic
void Surface::frame(){
  if(plotType==0){
    circlePlot.draw();
  }
}

Surface::~Surface(){
  playerClass.stop_thread(tname1);
}