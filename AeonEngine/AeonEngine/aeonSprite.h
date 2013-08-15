//
//  aeonSprite.h
//  AeonEngine
//
//  Created by Isaac Turner on 5/23/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#ifndef AeonEngine_aeonSprite_h
#define AeonEngine_aeonSprite_h

#include "aeonKernel.h"

typedef unsigned int aeon_SpriteHandle;

aeon_SpriteHandle aeon_CreateSprite(float width, float height, aeon_AtlasHandle image, aeon_Rect subImage);
void aeon_DestroySprite(aeon_SpriteHandle s);

void aeon_SetSpritePosition(float x, float y, aeon_SpriteHandle s);
void aeon_SetSpritePosition(float x, float y, float z, aeon_SpriteHandle s);
void aeon_SetSpritePosition(aeon_Point p, aeon_SpriteHandle s);

void aeon_SetSpriteScale(float scale, aeon_SpriteHandle s);

void aeon_SetSpriteRotation(float rot, aeon_SpriteHandle s);

void aeon_SetSpriteColor(float r, float g, float b, float a, aeon_SpriteHandle s);
void aeon_SetSpriteColor(aeon_Color c, aeon_SpriteHandle s);

void aeon_SetSpriteProperties(aeon_Point p, float rot, float scale, aeon_Color c, aeon_SpriteHandle s);

void aeon_SetSpriteActive(bool active, aeon_SpriteHandle s);
void aeon_ToggleSpriteActive(aeon_SpriteHandle s);


#endif
