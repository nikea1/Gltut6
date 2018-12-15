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

void mat4CrossMat4(t_mat4 n, t_mat4 m, t_mat4 out){
    /*
     0  1  2  3   0  1  2  3
     4  5  6  7   4  5  6  7
     8  9 10 11   8  9 10 11
     12 13 14 15  12 13 14 15
     
     
     */
    
    out[0] = m[0]*n[0] + m[4]*n[1] + m[8]*n[2] + m[12]*n[3];
    out[1] = m[1]*n[0] + m[5]*n[1] + m[9]*n[2] + m[13]*n[3];
    out[2] = m[2]*n[0] + m[6]*n[1] + m[10]*n[2] + m[14]*n[3];
    out[3] = m[3]*n[0] + m[7]*n[1] + m[11]*n[2] + m[15]*n[3];
    out[4] = m[0]*n[4] + m[4]*n[5] + m[8]*n[6] + m[12]*n[7];
    out[5] = m[1]*n[4] + m[5]*n[5] + m[9]*n[6] + m[13]*n[7];
    out[6] = m[2]*n[4] + m[6]*n[5] + m[10]*n[6] + m[14]*n[7];
    out[7] = m[3]*n[4] + m[7]*n[5] + m[11]*n[6] + m[15]*n[7];
    out[8] = m[0]*n[8] + m[4]*n[9] + m[8]*n[10] + m[12]*n[11];
    out[9] = m[1]*n[8] + m[5]*n[9] + m[9]*n[10] + m[13]*n[11];
    out[10] = m[2]*n[8] + m[6]*n[9] + m[10]*n[10] + m[14]*n[11];
    out[11] = m[3]*n[8] + m[7]*n[9] + m[11]*n[10] + m[15]*n[11];
    out[12] = m[0]*n[12] + m[4]*n[13] + m[8]*n[14] + m[12]*n[15];
    out[13] = m[1]*n[12] + m[5]*n[13] + m[9]*n[14] + m[13]*n[15];
    out[14] = m[2]*n[12] + m[6]*n[13] + m[10]*n[14] + m[14]*n[15];
    out[15] = m[3]*n[12] + m[7]*n[13] + m[11]*n[14] + m[15]*n[15];
}
void mat4Print(t_mat4 matrix){
    int i;
    for(i = 0; i < 16; i++){
        //space out numbers incase of negative signs
        if(matrix[i] >= 0){
            printf(" ");
        }
        //print out data
        printf("%.1f ", matrix[i]);
        //makes a new row
        if(i!=0 && i%4 == 3){
            printf("\n");
        }
    }
    printf("\n");
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

void myLookAt(t_vec3 eye, t_vec3 target, t_vec3 worldUp, t_mat4 out){
   
    t_vec3 direction;
    t_vec3 right;
    t_vec3 up;
    t_mat4 lookAt;
    t_mat4 cameraPos;
    
    glmc_sub_vec3(eye, target, direction);
    glmc_normalize(direction);
    glmc_cross(worldUp, direction, right);
    glmc_normalize(right);
    glmc_cross(direction, right, up);
    
    lookAt[0] = right[0];
    lookAt[1] = up[0];
    lookAt[2] = direction[0];
    lookAt[3] = 0;
    
    lookAt[4] = right[1];
    lookAt[5] = up[1];
    lookAt[6] = direction[1];
    lookAt[7] = 0;
    
    lookAt[8] = right[2];
    lookAt[9] = up[2];
    lookAt[10] = direction[2];
    lookAt[11] = 0;
    
    lookAt[12] = 0;
    lookAt[13] = 0;
    lookAt[14] = 0;
    lookAt[15] = 1;
    
    
    glmc_identity(cameraPos);
    
    cameraPos[12] = eye[0]*-1;
    cameraPos[13] = eye[1]*-1;
    cameraPos[14] = eye[2]*-1;
    
    
    mat4CrossMat4(cameraPos, lookAt, out);
}
