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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void myPerspective(double*, double*);
void zeroMat4(t_mat4 in);
void mat4Transpose(t_mat4 in, t_mat4 out);
double toRadians(double degrees);
void mat4Copy(t_mat4 in, t_mat4 out);
void mat4DoubleToFloat(t_mat4 in, float* out);

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
    //MVP Matrix
    //-------------------------
    GLfloat m4_out[16]; //float output to shaders
    
    
    
    t_vec4 frustrum; //FOV, Ascpect, Near, Far
    t_vec3 trans; //translate model
    t_vec3 axis; //rotation axis
    //camera
    t_vec3 cameraPos;
    t_vec3 cameraTarget;
    t_vec3 cameraDirection;
    t_vec3 cameraRight;
    t_vec3 cameraUp;
    t_vec3 up;
    
    t_mat4 model;
    t_mat4 view;
    t_mat4 projection;
    t_mat4 temp;
   
    glmc_vec3(0.0, 0.0, 3.0, cameraPos);
    glmc_vec3(0.0, 0.0, 0.0, cameraTarget);
    glmc_sub_vec3(cameraPos, cameraTarget, cameraDirection);
    glmc_normalize(cameraDirection);
    
    glmc_vec3(0.0, 1.0, 0.0, up);
    glmc_cross(up, cameraDirection, cameraRight);
    glmc_normalize(cameraRight);
    
    glmc_cross(cameraDirection, cameraRight, cameraUp);
    glmc_normalize(cameraUp);
    
    //initialize matricies
    glmc_identity(view);
    glmc_identity(projection);
    
    //projection
    glmc_vec4(toRadians(45.0), (double)WINDOW_WIDTH/(double)WINDOW_HEIGHT, 0.1, 100.0, frustrum);
    myPerspective(frustrum, projection);
    mat4DoubleToFloat(projection, m4_out);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, m4_out);
    
    //view
    //glmc_vec3(0.0, 0.0, -3.0, trans);
    //glmc_translate(view, trans, temp);
    //mat4Copy(temp, view);
    
    //glmc_look_at(cameraPos, cameraTarget, up, view);
    //mat4DoubleToFloat(view, m4_out);
    //glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, m4_out);

    
    
    glEnable(GL_DEPTH_TEST);
    //--------------------------
    //While Loop
    //--------------------------
    while(!glfwWindowShouldClose(window)){
        
        //clear screen
        glClearColor(0.0, 0.5, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        
        //activate textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex1);
        
        //view matrix
        glmc_identity(view);
        GLdouble radius = 10.0;
        GLdouble camX = sin(glfwGetTime()) * radius;
        GLdouble camZ = cos(glfwGetTime())* radius;
        
        glmc_vec3(camX, 0.0, camZ, cameraPos);
        glmc_vec3(0.0, 0.0, 0.0, cameraTarget);
        glmc_vec3(0.0, 1.0, 0.0, up);
        glmc_look_at(cameraPos, cameraTarget, up, view);
        mat4DoubleToFloat(view, m4_out);
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, m4_out);
        
        for(int i = 0; i < 10; i++){
            //model matrix
            glmc_identity(model);
            glmc_translate(model, cube_Positions[i], temp);
            glmc_vec3(1.0, 0.3, 0.5, axis);
            double angle = 20 * i;
            if(i%3 == 0){
                angle = 25.0 * glfwGetTime();
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

//converts matrix to float matrix
void mat4DoubleToFloat(t_mat4 in, float* out){
    if(in == NULL || out == NULL)
        return;
    
    for(int i = 0; i< 16; i++){
        out[i] = (float)in[i];
    }
}

//copy matrix
void mat4Copy(t_mat4 in, t_mat4 out){
    
    for(int i = 0; i< 16; i++){
        out[i] = in[i];
    }
}

//convert degrees to Radians
double toRadians(double degrees){
    return degrees*M_PI/180.0;
}

//Transpose matrix
void mat4Transpose(t_mat4 in, t_mat4 out){
    
    // 0  1  2  3   -> 0  4  8 12
    // 4  5  6  7      1  5  9 13
    // 8  9 10 11      2  6 10 14
    //12 13 14 15      3  7 11 15
    
    out[0] = in[0];
    out[1] = in[4];
    out[2] = in[8];
    out[3] = in[12];
    
    out[4] = in[1];
    out[5] = in[5];
    out[6] = in[9];
    out[7] = in[13];
    
    out[8] = in[2];
    out[9] = in[6];
    out[10] = in[10];
    out[11] = in[14];
    
    out[12] = in[3];
    out[13] = in[7];
    out[14] = in[11];
    out[15] = in[15];
    
}

//zero out matrix
void zeroMat4(t_mat4 in){
    for(int i = 0; i < 16; i++){
        in[i] = 0.0;
    }
}
//my own perspective function
void myPerspective(t_vec4 frustum, t_mat4 dest)
{

    // 0  1  2  3
    // 4  5  6  7
    // 8  9 10 11
    //12 13 14 15
    double f, fn;
    zeroMat4(dest);
    f = 1/tan(frustum[FOV]*0.5);
    fn = 1 / (frustum[NEAR]-frustum[FAR]);
    
    dest[0] = f / frustum[ASPECT];
    dest[5] = f;
    dest[10] = (frustum[NEAR] + frustum[FAR]) * fn;
    dest[11] = -1.0;
    dest[14] = 2.0 * fn * frustum[NEAR] * frustum[FAR];
    
}

