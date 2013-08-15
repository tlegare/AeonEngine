//
//  aeonTile.h
//  AeonEngine
//
//  Created by Isaac Turner on 8/10/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#ifndef AeonEngine_aeonTile_h
#define AeonEngine_aeonTile_h

#include "aeonKernel.h"

typedef unsigned int aeon_TileHandle;

aeon_TileHandle aeon_CreateTile(float w, float h, aeon_AtlasHandle atlas, aeon_Rect subImage);
void aeon_DestroyTile(aeon_TileHandle t);

void aeon_SetTileProperties(aeon_Color c, float rotation, float scale, aeon_TileHandle t);
void aeon_SetTileActive(bool isActive, aeon_TileHandle t);
void aeon_ToggleTileActive(aeon_TileHandle t);
void aeon_SetTileInstanceList(aeon_Point ilist[], unsigned int icount, aeon_TileHandle t);

void aeon_DrawTiles(); // this needs to go at some point

#endif
