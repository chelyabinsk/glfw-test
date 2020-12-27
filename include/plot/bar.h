#ifndef BAR_H
#define BAR_H

#include "iostream"

// GLM maths library
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

class Bar{
public:
  unsigned int ID;
  float height;
  float width;
  float theta;
  float color[4] = {0.0f,0.0f,0.0f,1.0f};

  // Constructor
  Bar();
  ~Bar();
  
  void updateHeight(float newHeight);
  void updateColor(float r, float g, float b, float w);
  void updateTheta(float th);
};

#endif