//
//  myGLMC.h
//  Gltut6
//
//  Created by Adenike Adeyemi on 12/14/18.
//  Copyright © 2018 Adenike Adeyemi. All rights reserved.
//

#ifndef myGLMC_h
#define myGLMC_h

#include <glad/glad.h>
#include <glmc.h>
#include <stdio.h>
#include <math.h>

double toRadians(double degrees);
void mat4DoubleToFloat(t_mat4 in, float* out);
void mat4Copy(t_mat4 in, t_mat4 out);
void mat4Transpose(t_mat4 in, t_mat4 out);
void mat4Zero(t_mat4 in);

void myPerspective(t_vec4 frustrum, t_mat4 out);

void vec3Add(t_vec3 v1, t_vec3 v2, t_vec3 out);
void vec3Sub(t_vec3 v1, t_vec3 v2, t_vec3 out);
void vec3ScalarMul(double scalar, t_vec3 in, t_vec3 out);
void vec3Copy(t_vec3 in, t_vec3 out);
void vec3Print(t_vec3 in);

#endif /* myGLMC_h */
