#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "shader.h"  // Shader loader helper
#include "stb_image.h"  // Image loader helper
#include "mpg123/src/libmpg123/mpg123.h"  // mpg123 library to help stream MP3
#include <main.h>


int s_width = 600;
int s_height = 600;

float offset[3] = {0.0f,0.0f,0.0f};
float pos[2] = {0.0f,0.0f};

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
    
    float s_f = 0.1f;
    float s_t = 1.0f;
    float cols[4] = {1.0f,0.0f,0.0f,1.0f};
    float vertices[] = {
        // positions          // colors           // texture coords
        s_f,  s_f, 0.0f,   cols[0],cols[1],cols[2],   s_t, s_t, // top right
        s_f, -s_f, 0.0f,   cols[0],cols[1],cols[2],   s_t, 0.0f, // bottom right
       -s_f, -s_f, 0.0f,   cols[0],cols[1],cols[2],   0.0f, 0.0f, // bottom left
       -s_f,  s_f, 0.0f,   cols[0],cols[1],cols[2],   0.0f, s_t  // top left 
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
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
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
    data = stbi_load("textures/dvdlogo.png",&width,&height,&nrChannels,0);
    if(data){
        stbi_set_flip_vertically_on_load(true);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    
    // Free image memory
    stbi_image_free(data);
    
    Shader myShader("shaders/basic_vertex.glsl","shaders/basic_fragment2.glsl");
    myShader.use();
    myShader.setInt("texture1", 0);
    myShader.setInt("texture2", 1);

    double lastTime = glfwGetTime();
    unsigned int nbFrames = 1;
    //float cols[4] = {0.0f,0.0f,1.0f,1.0f};
    float vel_x = 0.0025f;
    float vel_y = 0.004f;
    
    while(!glfwWindowShouldClose(window))
    {
        // Process inputs here
        processInput(window);
        
        // Do logic stuff here
        ///
        ///
        pos[0] += vel_x;
        pos[1] += vel_y;
        if((pos[0] + s_f >= 1) | (pos[0] - s_f <= -1)){
            vel_x *= -1;
            cols[0] = RandomNumber(0.0f,1.0f);
            cols[1] = RandomNumber(0.0f,1.0f);
            cols[2] = RandomNumber(0.0f,1.0f);
        }
        if((pos[1] + s_f >= 1) |( pos[1] - s_f <= -1)){
            vel_y *= -1;
            cols[0] = RandomNumber(0.0f,1.0f);
            cols[1] = RandomNumber(0.0f,1.0f);
            cols[2] = RandomNumber(0.0f,1.0f);
        }
        // Draw Stuff here
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        // Draw second triangle
        myShader.use();
        myShader.setVec3("offset",pos);
        //RandomNumber
        myShader.setVec4("newColor",cols);
        
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
    if(glfwGetKey(window,GLFW_KEY_UP)==GLFW_PRESS){
        offset[1] += 0.01;
        pos[1] += 0.01;
    }
    if(glfwGetKey(window,GLFW_KEY_DOWN)==GLFW_PRESS){
        offset[1] -= 0.01;
        pos[1] -= 0.01;
    }
    if(glfwGetKey(window,GLFW_KEY_LEFT)==GLFW_PRESS){
        offset[0] -= 0.01;
        pos[0] -= 0.01;
    }
    if(glfwGetKey(window,GLFW_KEY_RIGHT)==GLFW_PRESS){
        offset[0] += 0.01;
        pos[0] += 0.01;
    }
    
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

float RandomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}