#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

unsigned int init_vertex_shader(unsigned int shaderProgram);
unsigned int init_fragment_shader(unsigned int shaderProgram);
unsigned int init_fragment_shader2(unsigned int shaderProgram);

void showFPS(GLFWwindow *pWindow,unsigned int &nbFrames, double &lastTime);
float RandomNumber(float Min, float Max);