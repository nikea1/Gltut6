//
//  main.c
//  Gltut6
//
//  Created by Adenike Adeyemi on 11/28/18.
//  Copyright © 2018 Adenike Adeyemi. All rights reserved.
//
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glmc.h>

#include <stdio.h>

#include "myGLMC.h"
#include "myCamera.h"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//Camera struct
Camera *camera;

//Camera function
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

//Global camera
//t_vec3 cameraPos;
//t_vec3 cameraFront;
//t_vec3 up;
//double fov = 45.0;

//delta time
double deltaTime = 0;
double lastTime = 0;

//cursor init
double lastX = WINDOW_WIDTH/2.0;
double lastY = WINDOW_HEIGHT/2.0;
//double pitch = 0.0;
//double yaw = -90.0;
int firstMouse = 1;

int main(int argc, const char * argv[]) {
    //------------------------------------
    //Initialize GLFW and GLAD
    //------------------------------------
    GLFWwindow *window;
    
    if(!glfwInit()){
        printf("GLFW failed");
        exit(EXIT_FAILURE);
    }
    
    //GLFW hin for apple
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    
    //Create Window
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Camera", NULL, NULL);
    
    //Check if window was created
    if(!window){
        printf("Window cannot open.\n");
        exit(EXIT_FAILURE);
    }
    
    //Set Window to active
    glfwMakeContextCurrent(window);
    
    //Load Glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("GLAD cannot load");
        exit(EXIT_FAILURE);
    }
    
    //----------------------------
    //OpenGL Code Here
    //---------------------------
    
    GLuint program = initShaderFiles("vshader.vert", "fshader.frag");
    GLuint vao;
    GLuint vbo;
    
    GLuint tex0, tex1; //textures
    unsigned char *img;
    int width, height, nrchannel;
    
    // Cube points from tut 5
    GLfloat c_vertices[] = {
        //Position            //Textures
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    //location for each cube
    t_vec3 cube_Positions[] = {
        {0.0f, 0.0f,   0.0f},
        {2.0f, 5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        { 2.4f, -0.4f, -3.5f},
        {-1.7f,  3.0f, -7.5f},
        { 1.3f, -2.0f, -2.5f},
        { 1.5f,  2.0f, -2.5f},
        { 1.5f,  0.2f, -1.5f},
        {-1.3f,  1.0f, -1.5f},
        
    };
    //t_vec3 cameraTarget;
    //glmc_vec3(0.0, 0.0, 3.0, cameraPos);
    //glmc_vec3(0.0, 0.0, -1.0, cameraFront);
    //glmc_vec3(0.0, 1.0, 0.0, up);
    
    glUseProgram(program);
    //--------------------------
    //Texture
    //--------------------------
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex0);
    glBindTexture(GL_TEXTURE_2D, tex0);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    img = stbi_load("container.jpg", &width, &height, &nrchannel, 0);
    
    if(!img){
        printf("Image failed to load.");
        exit(EXIT_FAILURE);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(img);
    
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &tex1);
    glBindTexture(GL_TEXTURE_2D, tex1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_set_flip_vertically_on_load(GL_TRUE);
    img = stbi_load("awesomeface.png", &width, &height, &nrchannel, 0);
    
    if(!img){
        printf("Image failed to load.");
        exit(EXIT_FAILURE);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(img);
    
    //--------------------------
    // vertex buffers
    //--------------------------
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(c_vertices), c_vertices, GL_STATIC_DRAW);
    
    //--------------------------
    // atrribute data
    //--------------------------
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    
    
    //-------------------------
    //Uniform texture
    //-------------------------
    
    glUniform1i(glGetUniformLocation(program, "tex0"), 0);
    glUniform1i(glGetUniformLocation(program, "tex1"), 1);
    
    //-------------------------
    //Matrix Setup
    //-------------------------
    GLfloat m4_out[16]; //float output to shaders
    
    t_vec4 frustrum; //FOV, Ascpect, Near, Far
    t_vec3 axis; //rotation axis
   
    t_mat4 model;
    t_mat4 view;
    t_mat4 projection;
    t_mat4 temp;
    
    camera = malloc(sizeof(*camera));
    if(!camera){
        printf("Out of Memory.\n");
        exit(EXIT_FAILURE);
    }
    scalarCamera(camera, 0.0, 0.0, 3.0, 0.0, 1.0, 0.0);
    
    //initialize matricies
    //glmc_identity(view);
    //glmc_identity(projection);
    
    //projection
    //glmc_vec4(toRadians(45.0), (double)WINDOW_WIDTH/(double)WINDOW_HEIGHT, 0.1, 100.0, frustrum);
    //myPerspective(frustrum, projection);
    //mat4DoubleToFloat(projection, m4_out);
    //glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, m4_out);

    //--------------------------
    //Config
    //--------------------------
    glEnable(GL_DEPTH_TEST);
    double currentFrame;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback); //mouse
    glfwSetScrollCallback(window, scroll_callback); //mouse zoom
    //--------------------------
    //While Loop
    //--------------------------
    
    while(!glfwWindowShouldClose(window)){
        
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;
        
        processInput(window); //keyboard
        
        //clear screen
        glClearColor(0.0, 0.5, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        
        //activate textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex1);
        
        //projection
        glmc_identity(projection);
        //glmc_vec4(toRadians(fov), (double)WINDOW_WIDTH/(double)WINDOW_HEIGHT, 0.1, 100.0, frustrum);
        glmc_vec4(toRadians(camera->zoom), (double)WINDOW_WIDTH/(double)WINDOW_HEIGHT, 0.1, 100.0, frustrum);
        myPerspective(frustrum, projection);
        mat4DoubleToFloat(projection, m4_out);
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, m4_out);
        
        //view matrix
        glmc_identity(view);
        //GLdouble radius = 10.0;
        //GLdouble camX = sin(glfwGetTime()) * radius;
        //GLdouble camZ = cos(glfwGetTime())* radius;
        
        //glmc_vec3(camX, 0.0, camZ, cameraPos);
        //glmc_vec3(0.0, 0.0, 0.0, cameraTarget);
        //glmc_vec3(0.0, 1.0, 0.0, up);
        //glmc_look_at(cameraPos, cameraTarget, up, view);
        
        
        //vec3Add(cameraPos, cameraFront, cameraTarget);
        
        
        
        //glmc_look_at(cameraPos, cameraTarget, up, view); //look up
        getViewMatrix(camera, view);
        mat4DoubleToFloat(view, m4_out);
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, m4_out);
        
        for(int i = 0; i < 10; i++){
            //model matrix
            glmc_identity(model);
            glmc_translate(model, cube_Positions[i], temp);
            glmc_vec3(1.0, 0.3, 0.5, axis);
            double angle = 20 * i;
            if(i%3 == 0){
                //angle = 25.0 * glfwGetTime();
            }
            
            glmc_rotate(temp, toRadians(angle), axis, model);
            mat4DoubleToFloat(model, m4_out);
            glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, m4_out);
            
            //Draw
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    printf("Hello, World!\n");
    return 0;
}

//----------------------------
//Helper functions
//----------------------------

void mouse_callback(GLFWwindow* window, double xPos, double yPos){
    /*
     1. Calculate the mouse's offset since the last frame.
     2. Add the offset values to the camera's yaw and pitch values.
     3. Add some constraints to the maximum/minimum pitch values
     4. Calculate the direction vector
     */
    
    if(firstMouse){
        lastX = xPos;
        lastY = yPos;
        firstMouse = 0;
    }
    
    double xOffset = xPos - lastX;
    double yOffest = lastY - yPos;
    lastX = xPos;
    lastY = yPos;
    processMouseMovement(camera, xOffset, yOffest, 1);
    /*
    double sensativity = 0.05;
    xOffset *= sensativity;
    yOffest *= sensativity;
    
    yaw += xOffset;
    pitch += yOffest;
    
    if(pitch > 89.0){
        pitch = 89.0;
    }
    if(pitch < -89.0){
        pitch = -89.0;
    }
    
    t_vec3 front;
    front[X] = cos(toRadians(pitch)) * cos(toRadians(yaw));
    front[Y] = sin(toRadians(pitch));
    front[Z] = cos(toRadians(pitch)) * sin(toRadians(yaw));
    glmc_normalize(front);
    vec3Copy(front, cameraFront);
     */
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset){
    /*if(fov >= 1.0 && fov <= 45.0)
        fov -= yOffset;
    if(fov < 1.0)
        fov = 1.0;
    if(fov > 45.0)
        fov = 45.0;*/
    
    processMouseScroll(camera, yOffset);
}

void processInput(GLFWwindow *window){
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
    }
    
    //double cameraSpeed = 2.5 * deltaTime;
    //t_vec3 temp,temp2;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        printf("W\n");
        /*
        vec3ScalarMul(cameraSpeed, cameraFront, temp);
        vec3Add(temp, cameraPos, temp2);
        vec3Copy(temp2, cameraPos);*/
        processKeyBoard(camera, FOREWARD, deltaTime);
        
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        printf("S\n");
        /*
        vec3ScalarMul(cameraSpeed, cameraFront, temp);
        vec3Sub(cameraPos,temp, temp2);
        vec3Copy(temp2, cameraPos);*/
        processKeyBoard(camera, BACKWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        /*
        glmc_cross(cameraFront, up,temp);
        glmc_normalize(temp);
        vec3ScalarMul(cameraSpeed, temp, temp2);
        vec3Sub(cameraPos, temp2, temp);
        vec3Copy(temp, cameraPos);*/
        processKeyBoard(camera, LEFTWARD, deltaTime);
        printf("A\n");
    }
    
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        printf("D\n");
        /*glmc_cross(cameraFront, up, temp);
        glmc_normalize(temp);
        vec3ScalarMul(cameraSpeed, temp, temp2);
        vec3Add(cameraPos, temp2, temp);
        vec3Copy(temp, cameraPos);*/
        processKeyBoard(camera, RIGHTWARD, deltaTime);
        
    }
    
}

