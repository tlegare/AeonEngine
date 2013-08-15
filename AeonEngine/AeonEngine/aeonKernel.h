//
//  aeonKernel.h
//  AeonEngine
//
//  Created by Isaac Turner on 5/7/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#ifndef AeonEngine_aeonKernel_h
#define AeonEngine_aeonKernel_h

#include <assert.h>
#include "framework.h"

#define AEON_CONTINUE_GAME true
#define AEON_END_GAME false

//----------------------------------------
// Framework is SDL\OpenGL
//----------------------------------------
#ifdef FRAMEWORK_IS_SDL

#include "SDL/SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"

#endif

//----------------------------------------
// Aeon Display Mode Constants
//----------------------------------------
#define AEON_MODE_FULLSCREEN true
#define AEON_MODE_WINDOWED false

//----------------------------------------
// Aeon Engine
//----------------------------------------

// Types

typedef unsigned int aeon_AtlasHandle;

enum aeon_LoopType { AEON_LOOP, AEON_ONESHOT, AEON_ONESHOT_DESTROY };

struct aeon_Point
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct aeon_Rect
{
    aeon_Point upperLeft;
    aeon_Point lowerRight;
};

struct aeon_Color
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
};

// Functions

// Images
aeon_AtlasHandle aeon_LoadAtlas(const char* path);
void aeon_DrawSubImage(float x, float y, float z, float w, float h, float rot, float scale, aeon_Color color, aeon_Rect subImg);
void aeon_DestroyAtlas(aeon_AtlasHandle imageHandle);

// Misc
void aeon_ShowCursor(bool show);
void aeon_Init(int width, int height, int depth, bool fullscreen);

#endif
