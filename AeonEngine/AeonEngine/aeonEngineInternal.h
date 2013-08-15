//
//  aeonKernelTypes.h
//  AeonEngine
//
//  Created by Isaac Turner on 6/7/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#ifndef AeonEngine_aeonEngineInternal_h
#define AeonEngine_aeonEngineInternal_h

#include "aeonKernel.h"
#include "aeonPool.h"

struct aeon_Atlas : public PooledResource
{
#ifdef FRAMEWORK_IS_SDL
    GLuint glTexture;
#endif
    int width;
    int height;
};

int aeon_GetImageWidth(aeon_AtlasHandle imageHandle);
int aeon_GetImageHeight(aeon_AtlasHandle imageHandle);

int aeon_GetTicks(void);
void aeon_Delay(int t);
void aeon_BeginDrawing(void);
void aeon_EndDrawing(void);
void aeon_Shutdown(void);
void aeon_UpdateInput(void);
void aeon_UpdateInterpolators(float);
void aeon_UpdateTimers(float);
void aeon_DrawSprites(void);
void aeon_RenderImages(void);
GLuint aeon_GetShaderProgram();

#endif
