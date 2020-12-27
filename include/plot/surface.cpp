//#include "shader.h"
#include "plot/surface.h"
// Might also include the fft processing class here

// Surface constructor
Surface::Surface() {
  circlePlot.gen_bars(10,0.5f);
}

// frame logic
void Surface::frame(){
  if(plotType==0){
    circlePlot.draw();
  }
}

Surface::~Surface(){
  
}