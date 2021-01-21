#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>
#include "shader.h"  // Shader loader helper
#include "stb_image.h"  // Image loader helper
#include <mpg123.h>  // mpg123 library to help stream MP3

// GLM maths library
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "plot/surface.h"  // Class to deal with the actual rendering logic


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

unsigned int init_vertex_shader(unsigned int shaderProgram);
unsigned int init_fragment_shader(unsigned int shaderProgram);
unsigned int init_fragment_shader2(unsigned int shaderProgram);

void showFPS(GLFWwindow *pWindow,unsigned int &nbFrames, double &lastTime);
float RandomNumber(float Min, float Max);

void show_thread(const std::string &keyword);