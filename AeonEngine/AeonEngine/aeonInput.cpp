//
//  aeonInput.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 7/27/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#include "framework.h"
#include "aeonInput.h"

//----------------------------------------
// Aeon Keyboard Interface
//----------------------------------------
struct keyData
{
    int keyType = AEON_KEYTYPE_NORMAL;
    bool isDown = false;
};

static keyData keyStates[255];

bool aeon_GetKeyState(char key)
{
    bool state = keyStates[key].isDown;
    if(state && keyStates[key].keyType == AEON_KEYTYPE_NOREPEAT)
    {
        keyStates[key].isDown = AEON_KEY_IS_UP;
    }
    return state;
}

void aeon_SetKeyRepeat(int keyType, char key)
{
    keyStates[key].keyType = keyType;
}

//----------------------------------------
// SDL\OpenGL
//----------------------------------------
#ifdef FRAMEWORK_IS_SDL

#include "SDL/SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"

void aeon_UpdateInput(void)
{
    SDL_Event event;
    
    while(SDL_PollEvent(&event))
    {
        int keyCode = (int)event.key.keysym.sym;
        switch(event.type)
        {
            case SDL_KEYDOWN:
                keyStates[keyCode].isDown = AEON_KEY_IS_DOWN;
                break;
            case SDL_KEYUP:
                keyStates[keyCode].isDown = AEON_KEY_IS_UP;
                break;
            default:
                break;
        }
    }
}

#endif