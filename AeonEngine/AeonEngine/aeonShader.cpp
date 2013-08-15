//
//  aeonShader.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 6/18/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#include "aeonShader.h"

char vShaderCode[1000];
char fShaderCode[1000];

GLuint vertexShader;
GLuint fragmentShader;

GLuint shaderProgram;

enum shaderAttributes { vert_data, tex_data, color_data, xform_data };

void aeon_InitVertexShader()
{
    FILE *fp = fopen("vertex.shader", "r");

    assert(fp);

    int i;
    for(i = 0; !feof(fp); i++)
    {
        vShaderCode[i] = fgetc(fp);
    }
    
    vShaderCode[--i] = '\0';
}

void aeon_InitFragmentShader()
{
    FILE *fp = fopen("fragment.shader", "r");
    
    assert(fp);
    
    int i;
    for(i = 0; !feof(fp); i++)
    {
        fShaderCode[i] = fgetc(fp);
    }
    
    fShaderCode[--i] = '\0';
}

GLuint aeon_GetShaderProgram()
{
    return shaderProgram;
}

void aeon_InitShaderProgram()
{
    // vertex shader
    aeon_InitVertexShader();
    const char* src = vShaderCode;
    int status;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &src, NULL);
    glCompileShader(vertexShader);
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        char infoLog[256];
        int l;
        glGetShaderInfoLog(vertexShader, 254, &l, infoLog);
        infoLog[255] = '\0';
        printf("\n\n%s\n\n", infoLog);
        assert(0);
    }
    
    // fragment shader
    aeon_InitFragmentShader();
    src = fShaderCode;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &src, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        char infoLog[256];
        int l;
        glGetShaderInfoLog(fragmentShader, 254, &l, infoLog);
        infoLog[255] = '\0';
        printf("\n\n%s\n\n", infoLog);
        assert(0);
    }
    
    // create and link shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    glBindAttribLocation(shaderProgram, vert_data, "vert_data");
    glBindAttribLocation(shaderProgram, tex_data, "tex_data");
    glBindAttribLocation(shaderProgram, color_data, "color_data");
    glBindAttribLocation(shaderProgram, xform_data, "xform_data");
    
    glLinkProgram(shaderProgram);
    
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        char infoLog[256];
        int l;
        glGetShaderInfoLog(shaderProgram, 254, &l, infoLog);
        infoLog[255] = '\0';
        printf("\n\n%s\n\n", infoLog);
        assert(0);
    }
    glUseProgram(shaderProgram);
    
    // this is the projection for geometry not affected by the camera
    float left = 0.0f;
    float right = 100.0f; // (float)width;
    float bottom = 65.0f; // (float)height;
    float top = 0.0f;
    float near = -1.0f;
    float far = 1.0f;
    
    float p[16] = {0.0f};
    p[0] = 2.0f / (right - left);
    p[5] = 2.0f / (top - bottom);
    p[10] = -2.0f / (far - near);
    p[12] = -(right + left) / (right - left);
    p[13] = -(top + bottom) / (top - bottom);
    p[14] = -(far + near) / (far - near);
    p[15] = 1.0f;
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "static_projection"), 1, false, p);
}