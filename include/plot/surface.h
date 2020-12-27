#ifndef SURFACE_H
#define SURFACE_H

#include "plot/circlePlot.h"
#include "plot/linePlot.h"

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
};

#endif