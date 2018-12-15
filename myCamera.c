//
//  myCamera.c
//  Gltut6
//
//  Created by Adenike Adeyemi on 12/14/18.
//  Copyright © 2018 Adenike Adeyemi. All rights reserved.
//

#include "myCamera.h"
#include "myGLMC.h"

void updateCameraVectors(Camera *camera){
    t_vec3 front;
    front[X] = cos(toRadians(camera->yaw)) * cos(toRadians(camera->pitch));
    front[Y] = sin(toRadians(camera->pitch));
    front[Z] = sin(toRadians(camera->yaw)) * cos(toRadians(camera->pitch));
    glmc_normalize(front);
    vec3Copy(front, camera->front);
    
    glmc_cross(camera->front, camera->worldUp, camera->right);
    glmc_normalize(camera->right);
    
    glmc_cross(camera->front, camera->right, camera->up);
    glmc_normalize(camera->up);
}

void vectorCamera(Camera* camera, t_vec3 position , t_vec3 up){
    if(camera == NULL){
        camera = malloc(sizeof(*camera));
        
        if(camera == NULL){
            printf("Out of Memory");
            exit(EXIT_FAILURE);
        }
    }
    glmc_vec3(position[X], position[Y], position[Z], camera->position);
    glmc_vec3(up[X], up[Y], up[Z], camera->worldUp);
    
    camera->yaw = YAW;
    camera->pitch = PITCH;
    glmc_vec3(0.0, 0.0, -1.0, camera->front);
    camera->movementSpeed = SPEED;
    camera->mouseSensitivity = SENSITIVITY;
    camera->zoom = ZOOM;
    updateCameraVectors(camera);
}
void scalarCamera(Camera* camera, float xPos, float yPos, float zPos, float xUp, float yUp, float zUp){
    if(camera == NULL){
        camera = malloc(sizeof(*camera));
        
        if(camera == NULL){
            printf("Out of Memory");
            exit(EXIT_FAILURE);
        }
    }
    glmc_vec3(xPos, yPos, zPos, camera->position);
    glmc_vec3(xUp, yUp, zUp, camera->worldUp);
    
    camera->yaw = YAW;
    camera->pitch = PITCH;
    glmc_vec3(0.0, 0.0, -1.0, camera->front);
    camera->movementSpeed = SPEED;
    camera->mouseSensitivity = SENSITIVITY;
    camera->zoom = ZOOM;
    updateCameraVectors(camera);
}
void getViewMatrix(Camera* camera, t_mat4 output){
    
    t_vec3 target;
    
    for(int i=0; i<3; i++){
        target[i] = camera->position[i] + camera->front[i];
    }
    
    //glmc_look_at(camera->position, target, camera->worldUp, output);
    myLookAt(camera->position, target, camera->worldUp, output);

}
void processKeyBoard(Camera* camera, Directions dir, float deltaTime){
    
    double velocity = camera->movementSpeed * deltaTime;
    t_vec3 temp, temp2;
    if(dir == FOREWARD){
        vec3ScalarMul(velocity, camera->front, temp);
        //temp[Y] = 0.0;// This is my solution to prevent "flying" around space
        vec3Copy(camera->position, temp2);
        vec3Add(temp2, temp, camera->position);
        
    }
    if(dir == BACKWARD){
        vec3ScalarMul(velocity, camera->front, temp);
        //temp[Y] = 0.0;// This is my solution to prevent "flying" around space
        vec3Copy(camera->position, temp2);
        vec3Sub(temp2, temp, camera->position);
    }
    if(dir == RIGHTWARD){
        vec3ScalarMul(velocity, camera->right, temp);
        vec3Copy(camera->position, temp2);
        vec3Add(temp2, temp, camera->position);
    }
    if(dir == LEFTWARD){
        vec3ScalarMul(velocity, camera->right, temp);
        vec3Copy(camera->position, temp2);
        vec3Sub(temp2, temp, camera->position);
    }
    
    camera->position[Y] = 0.0;// This the tutorials solution to keeps camera at ground level
}
void processMouseMovement(Camera* camera, float xOffset, float yOffset, int contrainPitch){
    xOffset *= camera->mouseSensitivity;
    yOffset *= camera->mouseSensitivity;
    
    camera->yaw += xOffset;
    camera->pitch += yOffset;
    
    if(contrainPitch){
        if(camera->pitch > 89.0)
            camera->pitch = 89.0;
        if(camera->pitch < -89.0)
            camera->pitch = -89.0;
    }
    
    updateCameraVectors(camera);
}
void processMouseScroll(Camera* camera, float yOffset){
    if(camera->zoom >= 1.0 && camera->zoom <= 45.0)
        camera->zoom -= yOffset;
    if(camera->zoom >= 45.0)
        camera->zoom = 45.0;
    if(camera->zoom <= 1.0)
        camera->zoom = 1.0;
}

