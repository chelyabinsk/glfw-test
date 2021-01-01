#include "main.h"

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
    
    Surface s;

    // double lastTime = glfwGetTime();
    // unsigned int nbFrames = 1;

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
        
        //showFPS(window,nbFrames,lastTime);
    }
    
    
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