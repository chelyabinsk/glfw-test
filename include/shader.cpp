#include <glad/gl.h>
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void Shader::loadShader(const char* vertexPath, const char* fragmentPath){
  // 1. Retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
  try 
  {
    // open files
    vShaderFile.open(vertexPath, std::ifstream::in);
    fShaderFile.open(fragmentPath, std::ifstream::in);
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();		
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    vertexCode   = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch(std::exception const& e){
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();
  
  // 2. compile shaders
  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  // vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  
  // print compile errors if any
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    
  };
  
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  
  // print compile errors if any
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  };
  
  // shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  // print linking errors if any
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  
  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath){
  loadShader(vertexPath,fragmentPath);
}

Shader::~Shader(){
  
}

Shader::Shader(){
  // Nothing going on here. go away
}

void Shader::use(){
  glUseProgram(Shader::ID);
}

void Shader::setBool(const std::string &name, bool value) const {         
  glUniform1i(glGetUniformLocation(Shader::ID, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(Shader::ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(Shader::ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, float value[3]) const{
  glUniform3f(glGetUniformLocation(Shader::ID, name.c_str()), 
              value[0],value[1],value[2]);
}
void Shader::setVec4(const std::string &name, float value[4]) const{
  glUniform4f(glGetUniformLocation(Shader::ID, name.c_str()), 
              value[0],value[1],value[2],value[3]);
}