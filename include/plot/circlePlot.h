#ifndef CIRCLEPLOT_H
#define CIRCLEPLOT_H
#include "plot/bar.h"
#include "vector"
#include "shader.h"
#include "shapes/square.h"
#include "stb_image.h"  // Image loader helper

// GLM maths library
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

class CirclePlot{
public:
  unsigned int N_bars;
  Square square;
  float centre_offset;
  float min_bar_spacing = 0.01f;
  
  // Constructor
  CirclePlot(unsigned int N_bars, float centre_offset);
  CirclePlot();
  void gen_bars(unsigned int N_bars, float centre_offset);
  void loadDefaultShader();
  
  float calc_width();
  
  void update_heights();
  
  void loadTextures();
  float RandomNumber(float Min, float Max);
  
  void draw();
private:
  Shader defaultShader;
  unsigned int texture1,texture2;
  std::vector<Bar> bars;
  unsigned int i;
  
  unsigned int transformLoc;
  glm::mat4 unit_mat = glm::mat4(1.0f);
  glm::mat4 trans = glm::mat4(1.0f);
};

#endif