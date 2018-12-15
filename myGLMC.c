//
//  myGLMC.c
//  Gltut6
//
//  Created by Adenike Adeyemi on 12/14/18.
//  Copyright © 2018 Adenike Adeyemi. All rights reserved.
//

#include "myGLMC.h"

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
    if(in == NULL || out == NULL)
        return;
    
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
    if(in == NULL || out == NULL)
        return;
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
void mat4Zero(t_mat4 in){
    if(in == NULL)
        return;
    for(int i = 0; i < 16; i++){
        in[i] = 0.0;
    }
}
//my own perspective function
void myPerspective(t_vec4 frustum, t_mat4 dest)
{
    if(frustum == NULL || dest == NULL)
        return;
    // 0  1  2  3
    // 4  5  6  7
    // 8  9 10 11
    //12 13 14 15
    double f, fn;
    mat4Zero(dest);
    f = 1/tan(frustum[FOV]*0.5);
    fn = 1 / (frustum[NEAR]-frustum[FAR]);
    
    dest[0] = f / frustum[ASPECT];
    dest[5] = f;
    dest[10] = (frustum[NEAR] + frustum[FAR]) * fn;
    dest[11] = -1.0;
    dest[14] = 2.0 * fn * frustum[NEAR] * frustum[FAR];
    
}

void vec3Add(t_vec3 v1, t_vec3 v2, t_vec3 out){
    if(v1 == NULL || v2 ==NULL || out == NULL)
        return;
    for(int i = 0; i < 3; i++)
        out[i] = v1[i] + v2[i];
}

void vec3Sub(t_vec3 v1, t_vec3 v2, t_vec3 out){
    if(v1 == NULL || v2 ==NULL || out == NULL)
        return;
    for(int i = 0; i < 3; i++)
        out[i] = v1[i] - v2[i];
}

void vec3ScalarMul(double scalar, t_vec3 in, t_vec3 out){
    if(in == NULL || out == NULL)
        return;
    
    for(int i=0; i<3; i++){
        out[i] = scalar*in[i];
    }
}

void vec3Copy(t_vec3 in, t_vec3 out){
    if(in == NULL || out == NULL)
        return;
    
    for(int i=0; i<3; i++){
        out[i] = in[i];
    }
}

void vec3Print(t_vec3 in){
    if(in == NULL)
        return;
    printf("X: %lf, Y: %lf, Z: %lf\n", in[0], in[1], in[2]);
}
