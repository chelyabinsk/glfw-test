#ifndef SQUARE_H
#define SQUARE_H
#include <glad/gl.h> // include glad to get all the required OpenGL headers
class Square{
public:
  unsigned int VBO, VAO, EBO, n_vert;
  Square();
};
#endif