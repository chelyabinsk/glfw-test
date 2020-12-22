#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "shader.h"  // Shader loader helper
#include "stb_image.h"  // Image loader helper
#include <main.h>


int s_width = 400;
int s_height = 400;

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
    
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
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
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    unsigned int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    // Load texture into memory
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/wall.jpeg",&width,&height,&nrChannels,0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    
    // Free image memory
    stbi_image_free(data);
    
    Shader myShader("shaders/basic_vertex.glsl","shaders/basic_fragment2.glsl");

    double lastTime = glfwGetTime();
    unsigned int nbFrames = 1;
    //float cols[3] = {0.0f,0.0f,0.0f};
    
    while(!glfwWindowShouldClose(window))
    {
        // Process inputs here
        processInput(window);
        
        // Do logic stuff here
        ///
        ///
        
        // Draw Stuff here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        glBindTexture(GL_TEXTURE_2D, texture);
        // Draw second triangle
        myShader.use();
        //myShader.setVec3("offsets",offsets);
        //myShader.setVec4("ourColor",cols);
        
        glBindVertexArray(VAO);
        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES,sizeof(vertecies),GL_UNSIGNED_INT,0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
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