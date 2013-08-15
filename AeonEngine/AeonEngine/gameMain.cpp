//
//  gameMain.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 6/7/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#include "gameMain.h"

struct Spinner
{
    aeon_SpriteHandle sprite;
    float r; // rotation
    float rs; // degrees per second
    float s; // scale
    float ss; // scale change per second
};

#define MAX_TEST_SPRITES 255
Spinner testSprites[MAX_TEST_SPRITES];

#define MAX_TEST_TILES 100
aeon_Point testTiles[MAX_TEST_TILES];

aeon_AtlasHandle testImage;

aeon_Font testFont;

aeon_TileHandle testTile;

float camX = 50.0f;
float camY = 32.5f;
float camZ = 100.0;
float camT = 10.0f;

void gameInit(void)
{
    aeon_Init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BITDEPTH, SCREEN_MODE);
    aeon_ShowCursor(false);
    
    srand(0xff726364);
    
    {
        testImage = aeon_LoadAtlas("chars.png");
        
        testFont.glyph_sheet = testImage;
        testFont.w = 2.0f;
        testFont.h = 2.0f;
        testFont.glyph_rect.upperLeft.x = 0;
        testFont.glyph_rect.upperLeft.y = 0;
        testFont.glyph_rect.lowerRight.x = 7;
        testFont.glyph_rect.lowerRight.y = 7;
        testFont.color.r = 0.85f;
        testFont.color.g = 0.85f;
        testFont.color.b = 0.0f;
        testFont.color.a = 1.0f;
        testFont.scale = 1.0f;
        testFont.kerning = 2.25;
        
        aeon_Rect subRect1;
        subRect1.upperLeft.x = 8;
        subRect1.upperLeft.y = 0;
        subRect1.lowerRight.x = 15;
        subRect1.lowerRight.y = 7;
        
        aeon_Rect subRect2;
        subRect2.upperLeft.x = 16;
        subRect2.upperLeft.y = 0;
        subRect2.lowerRight.x = 23;
        subRect2.lowerRight.y = 7;
        
        aeon_Rect tileRect;
        tileRect.upperLeft.x = 8;
        tileRect.upperLeft.y = 88;
        tileRect.lowerRight.x = 15;
        tileRect.lowerRight.y = 95;
        

        for(int i = 0; i < MAX_TEST_SPRITES; i++)
        {
            aeon_SpriteHandle *sp;
            
            sp = &testSprites[i].sprite;
            *sp = aeon_CreateSprite(2.0f, 2.0f, testImage, i % 2 ? subRect1 : subRect2);
            
            aeon_SetSpriteActive(true, *sp);
            aeon_SetSpritePosition(rand() % 100, rand() % 100, aeon_Map(rand() % 20, 0.0, 20.0, -0.75, 0), *sp);
            
            testSprites[i].s = rand() % 8 + 4;
            testSprites[i].ss = aeon_Map(rand() % 100, 0.0, 100.0, -20.0, 20.0);
            
            float cval = aeon_Map(rand() % 256, 0, 255, 0.125, 0.75);
            
            aeon_SetSpriteColor(0, cval, 0, cval, *sp);
            testSprites[i].r = 0;
            testSprites[i].rs = (rand() % 500 + 50) - 250;
        }
        
        testTile = aeon_CreateTile(10.0f, 10.0f, testImage, tileRect);
        aeon_Color tileColor;
        tileColor.r = 0.25f;
        tileColor.g = 0.25f;
        tileColor.b = 0.0f;
        tileColor.a = 1.0f;
        aeon_SetTileProperties(tileColor, 0, 1, testTile);
        aeon_SetTileActive(true, testTile);
        float y = 5.0f;
        float x = 5.0f;
        for(int i = 0; i < MAX_TEST_TILES; i++)
        {
            testTiles[i].x = x;
            testTiles[i].y = y;
            testTiles[i].z = -1.0f;
            x += 10.0f;
            if(x > 100.0f)
            {
                x = 5.0f;
                y += 10.0f;
            }
        }
        aeon_SetTileInstanceList(testTiles, MAX_TEST_TILES, testTile);
    }
}

bool gameUpdate(float dt)
{
    aeon_DrawText(5, 5, 0.0, testFont, "This is drawn in camera space!");
    aeon_DrawText(5, 25, 1.0, testFont, "This is drawn in static space!");
    for(int i = 0; i < MAX_TEST_SPRITES; i++)
    {
        aeon_SetSpriteRotation(aeon_DegToRad(testSprites[i].r += (testSprites[i].rs * dt)), testSprites[i].sprite);
        if(testSprites[i].r >= 360 || testSprites[i].r <= -360) testSprites[i].r = 0;
        
        aeon_SetSpriteScale(testSprites[i].s += (testSprites[i].ss * dt), testSprites[i].sprite);
        if(testSprites[i].s >= 12 || testSprites[i].s <= 0) {
            testSprites[i].ss *= -1;
        }
    }
    
    if(aeon_GetKeyState(AEON_KEY_ESCAPE)) return AEON_END_GAME;
    
    float camChange = camT * dt;
    if(aeon_GetKeyState('a'))
    {
        camX -= camChange;
    }
    if(aeon_GetKeyState('d'))
    {
        camX += camChange;
    }
    if(aeon_GetKeyState('w'))
    {
        camY -= camChange;
    }
    if(aeon_GetKeyState('s'))
    {
        camY += camChange;
    }
    if(aeon_GetKeyState('q'))
    {
        camZ -= camChange * 2;
    }
    if(aeon_GetKeyState('e'))
    {
        camZ += camChange * 2;
    }
    aeon_CameraLookAt(camX, camY, camZ);
       
    return AEON_CONTINUE_GAME;
}