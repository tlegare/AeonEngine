//
//  vertexShader.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 6/18/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#include "vertexShader.h"
#include <assert.h>
#include <stdio.h>

char vShaderCode[1000];
char fShaderCode[1000];

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

const char* aeon_GetVertexShader()
{
    return vShaderCode;
}

const char* aeon_GetFragmentShader()
{
    return fShaderCode;
}