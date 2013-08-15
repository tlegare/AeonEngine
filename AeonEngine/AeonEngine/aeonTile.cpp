//
//  aeonTile.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 8/10/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#include "aeonTile.h"
#include "aeonEngineInternal.h"
#include "aeonPool.h"

#define MAX_TILES POOL_MAX

#define MAX_INSTANCES 256

struct aeon_TileInstance
{
    aeon_Point position;
};

struct aeon_Tile : public PooledResource
{
    aeon_Color color;
    unsigned int w;
    unsigned int h;
    float rotation;
    float scale;
    aeon_Rect imgRect;
    bool isActive;
    aeon_Point* instanceList;
    unsigned int instanceCount = 0;
};

static aeonPool<aeon_Tile> tilePool;

aeon_TileHandle aeon_CreateTile(float w, float h, aeon_AtlasHandle atlas, aeon_Rect subImage)
{
    aeon_Tile* newTile = NULL;
    aeon_TileHandle newTileHandle = 0;
    
    unsigned int i = tilePool.getNextEmptySlot();
    newTile = &tilePool.storage[i];
    newTileHandle = tilePool.mangleHandle(i);
    
    newTile->color.r = 1.0f;
    newTile->color.g = 1.0f;
    newTile->color.b = 1.0f;
    newTile->color.a = 1.0f;
    newTile->w = (w == 0.0f) ? (subImage.lowerRight.x - subImage.upperLeft.x) : w;
    newTile->h = (h == 0.0f) ? (subImage.lowerRight.y - subImage.upperLeft.y) : h;
    newTile->rotation = 0.0;
    newTile->scale = 1.0;
    newTile->isActive = false;
    newTile->isAlive = true;
    
    unsigned int imgW = aeon_GetImageWidth(atlas);
    unsigned int imgH = aeon_GetImageHeight(atlas);
    
    if(subImage.upperLeft.x == 0)
        newTile->imgRect.upperLeft.x = 0;
    else
    {
        newTile->imgRect.upperLeft.x = 1 / (imgW / subImage.upperLeft.x);
    }
    if(subImage.upperLeft.y == 0)
        newTile->imgRect.upperLeft.y = 0;
    else
    {
        newTile->imgRect.upperLeft.y = 1 / (imgH / subImage.upperLeft.y);
    }
    newTile->imgRect.lowerRight.x = 1 / (imgW / (1 + subImage.lowerRight.x));
    newTile->imgRect.lowerRight.y = 1 / (imgH / (1 + subImage.lowerRight.y));
    
    return newTileHandle;
}

void aeon_DestroyTile(aeon_TileHandle t)
{
    aeon_Tile* tp = tilePool.getElement(t);
    tp->isActive = false;
    tilePool.killElement(t);
}

void aeon_DrawTiles()
{
    for(int i = 0; i < MAX_TILES; i++)
    {
        if(tilePool.storage[i].isAlive)
        {
            if(tilePool.storage[i].isActive)
            {
                aeon_Tile t = tilePool.storage[i];
                for(int j = 0; j < t.instanceCount; j++)
                {
                    float x = t.instanceList[j].x;
                    float y = t.instanceList[j].y;
                    float z = t.instanceList[j].z;
                    aeon_DrawSubImage(x, y, z, t.w, t.h, t.rotation, t.scale, t.color, t.imgRect);
                }
            }
        }
    }
}

void aeon_SetTileProperties(aeon_Color c, float rotation, float scale, aeon_TileHandle t)
{
    aeon_Tile* tp = tilePool.getElement(t);
    tp->color = c;
    tp->rotation = rotation;
    tp->scale = scale;
}

void aeon_SetTileActive(bool isActive, aeon_TileHandle t)
{
    aeon_Tile* tp = tilePool.getElement(t);
    tp->isActive = isActive;
}

void aeon_ToggleTileActive(aeon_TileHandle t)
{
    aeon_Tile* tp = tilePool.getElement(t);
    tp->isActive = !tp->isActive;
}

void aeon_SetTileInstanceList(aeon_Point ilist[], unsigned int icount, aeon_TileHandle t)
{
    aeon_Tile* tp = tilePool.getElement(t);
    tp->instanceCount = icount;
    tp->instanceList = ilist;
}