//
//  myCamera.h
//  Gltut6
//
//  Created by Adenike Adeyemi on 12/14/18.
//  Copyright © 2018 Adenike Adeyemi. All rights reserved.
//

#ifndef myCamera_h
#define myCamera_h

#include <glad/glad.h>
#include <glmc.h>
#include <stdio.h>
#include <stdlib.h>

//Default Values
#define YAW -90.0
#define PITCH 0.0
#define SPEED 2.5
#define SENSITIVITY 0.1
#define ZOOM 45.0

//Control enums
typedef enum Directions {
    FOREWARD,
    BACKWARD,
    LEFTWARD,
    RIGHTWARD
}Directions;

//Camera Data
typedef struct Camera{
    t_vec3 position;
    t_vec3 front;
    t_vec3 up;
    t_vec3 right;
    t_vec3 worldUp;
    
    float yaw;
    float pitch;
    
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
}Camera;

//Camera functions
void vectorCamera(Camera* , t_vec3 , t_vec3);
void scalarCamera(Camera* , float, float, float, float, float, float);
void getViewMatrix(Camera*, t_mat4);
void processKeyBoard(Camera*, Directions, float);
void processMouseMovement(Camera*, float, float, int);
void processMouseScroll(Camera*, float);


#endif /* myCamera_h */
