#include "plot/circlePlot.h"
#include <math.h>
#include <iostream>

CirclePlot::CirclePlot(unsigned int N_bars, float centre_offset): N_bars(N_bars), 
                                                           centre_offset(centre_offset),
                                                           defaultShader(){
  loadDefaultShader();
  gen_bars(N_bars, centre_offset);
  transformLoc = glGetUniformLocation(defaultShader.ID,"transform");
}

CirclePlot::CirclePlot() : defaultShader(){
  loadDefaultShader();
  transformLoc = glGetUniformLocation(defaultShader.ID,"transform");
}

void CirclePlot::loadDefaultShader(){
  // Load the default shaders
  defaultShader.loadShader("shaders/basic_vertex.glsl","shaders/basic_fragment2.glsl");
  defaultShader.use();
}

void CirclePlot::gen_bars(unsigned int _N_bars, float _centre_offset){
  // Set global variables
  N_bars = _N_bars;
  centre_offset = _centre_offset;
  
  float width = calc_width();
  
  bars.assign( N_bars, Bar() );  // magic! don't know/care if this is efficient
  for(unsigned int i=0;i<N_bars;i++){
    bars[i].width = width;
    bars[i].ID = i;
    bars[i].height = RandomNumber(0.1f,0.4f);
  }
  
  // Maybe add texture on top of the square
  loadTextures();
}

void CirclePlot::loadTextures(){
  glGenTextures(1,&texture1);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  
  // Load texture into memory
  int width, height, nrChannels;
  unsigned char *data = stbi_load("textures/wall.jpeg",&width,&height,&nrChannels,0);
  if(data){
    stbi_set_flip_vertically_on_load(true);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }else{
    std::cout << "Failed to load texture" << std::endl;
  }
  
  glGenTextures(1,&texture2);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2);
  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  
  // Load texture into memory
  //int width, height, nrChannels;
  data = stbi_load("textures/awesomeface.png",&width,&height,&nrChannels,0);
  if(data){
    stbi_set_flip_vertically_on_load(true);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }else{
    std::cout << "Failed to load texture" << std::endl;
  }
  
  // Free image memory
  stbi_image_free(data);
}


float CirclePlot::calc_width(){
  // Calculate width of each bar based on the
  // number of bars, distance from the center and bar spacing
  return (float)((2*M_PI*centre_offset)/((N_bars+1)*2)-min_bar_spacing);
}

void CirclePlot::update_heights(){
  // Function to update the bar heights
  // I think I will pass a list of float height values
}

void CirclePlot::draw(){
  // Function that calls GLFW to draw the bars on the screen
  
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2);

  // Select unit square
  glBindVertexArray(square.VAO);
  // std::cout << "offset: " << centre_offset << std::endl
  //      << "width: " << bars[0].width << std::endl
  //      << "height: " << bars[0].height << std::endl << std::endl;
  // trans = glm::rotate(unit_mat, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
  // trans = glm::translate(trans, glm::vec3(0.0f,0.0f, 0.0f));
  // trans = glm::scale(trans, glm::vec3(0.125f*0.125f, 0.125f*0.125f, 1.0f));
  // 
  // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
  // glDrawElements(GL_TRIANGLES, square.n_vert, GL_UNSIGNED_INT, 0);
  
  // float height = 0.25f;
  // float centre_offset = 0.25f;
  for(i=0;i<N_bars;i++){
      // Rotate around Z-axis
      trans = glm::rotate(unit_mat, 2*(float)M_PI*i/N_bars, glm::vec3(0.0f, 0.0f, 1.0f));
      trans = glm::translate(trans, glm::vec3(0.0f, centre_offset + bars[i].height*0.5f, 0.0f));
      trans = glm::scale(trans, glm::vec3(bars[i].width, bars[i].height*0.5f, 1.0f));

      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

      // Draw element on screen
      glDrawElements(GL_TRIANGLES, square.n_vert, GL_UNSIGNED_INT, 0);
  }
}

float CirclePlot::RandomNumber(float Min, float Max)
{
  return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}