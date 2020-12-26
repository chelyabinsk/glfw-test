#include <glad/gl.h>
//#include <GLFW/glfw3.h>
#include "glfw/include/GLFW/glfw3.h"
#include <iostream>
#include <math.h>
#include "shader.h"  // Shader loader helper
#include "stb_image.h"  // Image loader helper
#include "mpg123/src/libmpg123/mpg123.h"  // mpg123 library to help stream MP3

// GLM maths library
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "plot/surface.h"  // Class to deal with the actual rendering logic

#include <main.h>


int s_width = 500;
int s_height = 500;

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(s_width,s_height,"Hello",NULL,NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }
    
    glViewport(0,0,s_width,s_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    float obj_s = 1.0f;
    
    float vertices[] = {
        // positions          // colors           // texture coords
        obj_s,  obj_s, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        obj_s, -obj_s, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
       -obj_s, -obj_s, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
       -obj_s,  obj_s, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    }; 
    
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    unsigned int texture1,texture2;
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
    
    Surface s;
    
    // Shader myShader("shaders/basic_vertex.glsl","shaders/basic_fragment2.glsl");
    // myShader.use();
    // myShader.setInt("texture1", 0);
    // myShader.setInt("texture2", 1);

    double lastTime = glfwGetTime();
    unsigned int nbFrames = 1;
    //float cols[3] = {0.0f,0.0f,0.0f};
    
   
    // unsigned int transformLoc = glGetUniformLocation(myShader.ID,"transform");
    // 
    // glm::mat4 unit_mat = glm::mat4(1.0f);
    // glm::mat4 trans = glm::mat4(1.0f);
    // 
    // unsigned int N_bars = 50;
    // unsigned int i;
    
    while(!glfwWindowShouldClose(window))
    {
        // Process inputs here
        processInput(window);
        
        // Do logic stuff here
        ///
        ///
        
        // Draw Stuff here
        s.frame();
        

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        showFPS(window,nbFrames,lastTime);
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void showFPS(GLFWwindow *pWindow,unsigned int &nbFrames, double &lastTime){
    // Measure speed
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    
    nbFrames++;
    if ( delta >= 1.0 ){ // If last cout was more than 1 sec ago
        
        double fps = double(nbFrames)/delta;
        
        std::stringstream ss;
        ss << " [" << fps << " FPS]";
        
        glfwSetWindowTitle(pWindow, ss.str().c_str());
        //std::cout << fps << std::endl;
        
        nbFrames = 0;
        lastTime = currentTime;
    }
    
}

float RandomNumber(float Min, float Max){
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}