#ifndef SURFACE_H
#define SURFACE_H

#include "plot/circlePlot.h"
#include "plot/linePlot.h"

#include "player.h"  // Class to handle audio
#include <thread>  // Multithreading

class Surface{
public:
  int plotType = 0;
  // Constructor
  Surface();
  ~Surface();
  void frame();
private:
  LinePlot linePlot;
  CirclePlot circlePlot;
  Player playerClass;
  
  std::string tname1;
};

#endif