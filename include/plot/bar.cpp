#include "plot/bar.h"
Bar::Bar(){
  // glm::vec3 myvec;
  // myvec = glm::vec3(1.0f, 1.0f, 1.0f);
  // trans_mat[0] = 0.0f;  trans_mat[1] = 0.0f;// trans_mat[1] = 0.0f;
  // rot_mat[0] = 0.0f;    rot_mat[1] = 0.0f;  // rot_mat[2] = 0.0f;
  // scale_mat[0] = 0.0f;  scale_mat[1] = 0.0f; scale_mat[1] = 0.0f;
}

Bar::~Bar(){
  
}

void Bar::updateHeight(float newHeight){
  height = newHeight;
}

void Bar::updateColor(float r, float g, float b, float w){
  color[0] = r;
  color[1] = g;
  color[2] = b;
  color[3] = w;
}

void Bar::updateTheta(float th){
  theta = th;
}