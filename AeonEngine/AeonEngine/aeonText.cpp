//
//  aeonText.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 7/28/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#include "aeonText.h"
#include "aeonEngineInternal.h"

void aeon_DrawText(float x, float y, float z, aeon_Font font, const char* string)
{
    float w = (font.w == 0.0f) ? (font.glyph_rect.lowerRight.x - font.glyph_rect.upperLeft.x) : font.w;
    float h = (font.h == 0.0f) ? (font.glyph_rect.lowerRight.y - font.glyph_rect.upperLeft.y) : font.h;
    int imgW = aeon_GetImageWidth(font.glyph_sheet);
    int imgH = aeon_GetImageHeight(font.glyph_sheet);
    int columns = imgW / ((font.glyph_rect.lowerRight.x - font.glyph_rect.upperLeft.x) + 1);
    char cur_char = 0;
    aeon_Rect cur_rect;
    float oldX = x;
    
    for(int i = 0; (cur_char = string[i]); i++)
    {
        if(cur_char == '\n') // newline
        {
            x = oldX;
            y += ((h + 2) * font.scale);
            continue;
        }
        
        if(cur_char == '\t') // tab
        {
            x += (font.kerning * font.scale) * 4;
            continue;
        }
        
        cur_rect.upperLeft.y = (int)(cur_char / columns);
        cur_rect.upperLeft.x = cur_char - (char)(cur_rect.upperLeft.y * columns);
        
        cur_rect.upperLeft.x *= (font.glyph_rect.lowerRight.x - font.glyph_rect.upperLeft.x) + 1;
        cur_rect.upperLeft.y *= (font.glyph_rect.lowerRight.y - font.glyph_rect.upperLeft.y) + 1;
        cur_rect.lowerRight.x = cur_rect.upperLeft.x + (font.glyph_rect.lowerRight.x - font.glyph_rect.upperLeft.x);
        cur_rect.lowerRight.y = cur_rect.upperLeft.y + (font.glyph_rect.lowerRight.y - font.glyph_rect.upperLeft.y);
        
        aeon_Rect uv_rect;
        if(cur_rect.upperLeft.x == 0)
            uv_rect.upperLeft.x = 0;
        else
        {
            uv_rect.upperLeft.x = 1 / (imgW / cur_rect.upperLeft.x);
        }
        if(cur_rect.upperLeft.y == 0)
            uv_rect.upperLeft.y = 0;
        else
        {
            uv_rect.upperLeft.y = 1 / (imgH / cur_rect.upperLeft.y);
        }
        uv_rect.lowerRight.x = 1 / (imgW / (1 + cur_rect.lowerRight.x));
        uv_rect.lowerRight.y = 1 / (imgH / (1 + cur_rect.lowerRight.y));
        
        aeon_DrawSubImage(x, y, z, w, h, 0, font.scale, font.color, uv_rect);

        x += (font.kerning * font.scale);
    }
}

inline void aeon_DrawText(aeon_Point p, aeon_Font font, const char* string)
{
    aeon_DrawText(p.x, p.y, p.z, font, string);
}