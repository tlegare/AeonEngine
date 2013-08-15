//
//  main.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 2/13/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <CoreFoundation/CoreFoundation.h>
#include "aeonKernel.h"
#include "aeonEngineInternal.h"
#include "aeonEngine.h"

const int FPS = 60;

const float FRAME_TIME = 1000 / FPS;
const float FIXED_DELTA = FRAME_TIME / 1000;

// the game programmer should provide these functions
extern bool gameUpdate(float dt);
extern void gameInit(void);

// ------------------------------------------------
// The update function
// ------------------------------------------------
bool aeon_Update(float dt)
{
    aeon_UpdateTimers(dt);
    aeon_UpdateInterpolators(dt);
    aeon_UpdateInput();

    return gameUpdate(dt);
}

// ------------------------------------------------
// The draw function
// ------------------------------------------------
void aeon_Draw()
{
    aeon_BeginDrawing();
    {
        aeon_DrawSprites();
        aeon_DrawTiles();
        aeon_RenderImages();
    }
    aeon_EndDrawing();
}

// ------------------------------------------------
// main
// ------------------------------------------------
int main(int argc, char* args[])
{
    bool isRunning = true;
    
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcePath;
    Uint8 resourceString[PATH_MAX];
    
    // Look for a resource in the main bundle by name and type.
    resourcePath = CFBundleCopyResourcesDirectoryURL(mainBundle);
    CFURLGetFileSystemRepresentation(resourcePath, true, resourceString, PATH_MAX);
    chdir((const char*)resourceString);
#endif
    
    gameInit();
    
    // this is a fixed-rate loop
    while(isRunning)
    {
        int startT = aeon_GetTicks();
        {
            isRunning = aeon_Update(FIXED_DELTA);
            aeon_Draw();
        }
        int endT = aeon_GetTicks();
        
        int delayTime = FRAME_TIME - (endT - startT);
        if(delayTime > 0) aeon_Delay(delayTime);
    }
    
    aeon_Shutdown();
  
    return 0;
}