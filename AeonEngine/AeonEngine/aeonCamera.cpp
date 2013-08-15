//
//  aeonCamera.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 8/14/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#include "aeonCamera.h"
#include "aeonEngineInternal.h"

// this aspect ratio gives very good results in both 4:3 and widescreen
float aspectX = 1.0f;
float aspectY = 0.65f;

struct projectionData
{
    float left = 0.0f;
    float right = 100.0f;
    float bottom = 65.0f;
    float top = 0.0f;
    float near = -1.0f;
    float far = 1.0f;
} pd;

void aeon_CameraLookAt(float x, float y, float z)
{
    float width = z * aspectX;
    float height = z * aspectY;
    pd.left = x - (width / 2);
    pd.right = x + (width / 2);
    pd.top = y - (height / 2);
    pd.bottom = y + (height / 2);
    
    // orthographic projection camera matrix
    float p[16] = {0.0f};
    p[0] = 2.0f / (pd.right - pd.left);
    p[5] = 2.0f / (pd.top - pd.bottom);
    p[10] = -2.0f / (pd.far - pd.near);
    p[12] = -(pd.right + pd.left) / (pd.right - pd.left);
    p[13] = -(pd.top + pd.bottom) / (pd.top - pd.bottom);
    p[14] = -(pd.far + pd.near) / (pd.far - pd.near);
    p[15] = 1.0f;
    
    glUniformMatrix4fv(glGetUniformLocation(aeon_GetShaderProgram(), "dynamic_projection"), 1, false, p);
}