//
//  aeonSprite.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 5/23/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#include "aeonEngineInternal.h"
#include "aeonSprite.h"
#include "aeonPool.h"

#define MAX_SPRITES POOL_MAX

struct aeon_Sprite : public PooledResource
{
    aeon_Point position;
    aeon_Color color;
    unsigned int w;
    unsigned int h;
    float rotation;
    float scale;
    aeon_Rect imgRect;
    bool isActive;
};

static aeonPool<aeon_Sprite> spritePool;

aeon_SpriteHandle aeon_CreateSprite(float width, float height, aeon_AtlasHandle atlas, aeon_Rect subImage)
{
    aeon_Sprite* newSprite = NULL;
    aeon_SpriteHandle newSpriteHandle = 0;
    
    unsigned int i = spritePool.getNextEmptySlot();
    newSprite = &spritePool.storage[i];
    newSpriteHandle = spritePool.mangleHandle(i);
    
    newSprite->position.x = 0.0;
    newSprite->position.y = 0.0;
    newSprite->position.z = 1.0;
    newSprite->color.r = 1.0f;
    newSprite->color.g = 1.0f;
    newSprite->color.b = 1.0f;
    newSprite->color.a = 1.0f;
    if(width == 0.0f) newSprite->w = subImage.lowerRight.x - subImage.upperLeft.x;
    else newSprite->w = width;
    if(height == 0.0f) newSprite->h = subImage.lowerRight.y - subImage.upperLeft.y;
    else newSprite->h = height;
    newSprite->rotation = 0.0;
    newSprite->scale = 1.0;
    newSprite->isActive = false;
    newSprite->isAlive = true;
    
    unsigned int imgW = aeon_GetImageWidth(atlas);
    unsigned int imgH = aeon_GetImageHeight(atlas);
    
    if(subImage.upperLeft.x == 0)
        newSprite->imgRect.upperLeft.x = 0;
    else
    {
        newSprite->imgRect.upperLeft.x = 1 / (imgW / subImage.upperLeft.x);
    }
    if(subImage.upperLeft.y == 0)
        newSprite->imgRect.upperLeft.y = 0;
    else
    {
        newSprite->imgRect.upperLeft.y = 1 / (imgH / subImage.upperLeft.y);
    }
    newSprite->imgRect.lowerRight.x = 1 / (imgW / (1 + subImage.lowerRight.x));
    newSprite->imgRect.lowerRight.y = 1 / (imgH / (1 + subImage.lowerRight.y));
    
    return newSpriteHandle;
}

void aeon_DestroySprite(aeon_SpriteHandle s)
{    
    aeon_Sprite* sp = spritePool.getElement(s);
    sp->isActive = false;
    spritePool.killElement(s);
}

void aeon_DrawSprites()
{
    for(int i = 0; i < MAX_SPRITES; i++)
    {
        if(spritePool.storage[i].isAlive)
        {
            if(spritePool.storage[i].isActive)
            {
                aeon_Sprite s = spritePool.storage[i];
                aeon_DrawSubImage(s.position.x, s.position.y, s.position.z, s.w, s.h, s.rotation, s.scale, s.color, s.imgRect);
            }
        }
    }
}

void aeon_SetSpritePosition(float x, float y, aeon_SpriteHandle s)
{
    aeon_Sprite* sp = spritePool.getElement(s);
    sp->position.x = x;
    sp->position.y = y;
}

void aeon_SetSpritePosition(float x, float y, float z, aeon_SpriteHandle s)
{
    aeon_Sprite* sp = spritePool.getElement(s);
    sp->position.x = x;
    sp->position.y = y;
    sp->position.z = z;
}

void aeon_SetSpritePosition(aeon_Point p, aeon_SpriteHandle s)
{
    aeon_Sprite* sp = spritePool.getElement(s);
    sp->position = p;
}

void aeon_SetSpriteScale(float scale, aeon_SpriteHandle s)
{
    aeon_Sprite* sp = spritePool.getElement(s);
    sp->scale = scale;
}

void aeon_SetSpriteRotation(float rot, aeon_SpriteHandle s)
{
    aeon_Sprite* sp = spritePool.getElement(s);
    sp->rotation = rot;
}

void aeon_SetSpriteColor(float r, float g, float b, float a, aeon_SpriteHandle s)
{
    aeon_Sprite* sp = spritePool.getElement(s);
    sp->color.r = r;
    sp->color.g = g;
    sp->color.b = b;
    sp->color.a = a;
}

void aeon_SetSpriteColor(aeon_Color c, aeon_SpriteHandle s)
{
    aeon_Sprite* sp = spritePool.getElement(s);
    sp->color = c;
}

void aeon_SetSpriteProperties(aeon_Point p, float rot, float scale, aeon_Color c, aeon_SpriteHandle s)
{
    aeon_Sprite* sp = spritePool.getElement(s);
    sp->position = p;
    sp->rotation = rot;
    sp->scale = scale;
    sp->color = c;
}

void aeon_SetSpriteActive(bool active, aeon_SpriteHandle s)
{
    aeon_Sprite* sp = spritePool.getElement(s);
    sp->isActive = active;
}

void aeon_ToggleSpriteActive(aeon_SpriteHandle s)
{
    aeon_Sprite* sp = spritePool.getElement(s);
    sp->isActive = !(sp->isActive);
}



