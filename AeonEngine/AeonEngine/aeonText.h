//
//  aeonText.h
//  AeonEngine
//
//  Created by Isaac Turner on 7/28/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#ifndef AeonEngine_aeonText_h
#define AeonEngine_aeonText_h

#include "aeonKernel.h"

enum aeon_GlyphTableType { AEON_GLYPH_ASCII };

struct aeon_Font
{
    aeon_AtlasHandle glyph_sheet;
    float w;
    float h;
    aeon_Rect glyph_rect;
    aeon_GlyphTableType glyph_table = AEON_GLYPH_ASCII;
    aeon_Color color;
    float scale;
    float kerning;
};

void aeon_DrawText(float x, float y, float z, aeon_Font font, const char* string);
void aeon_DrawText(aeon_Point p, aeon_Font font, const char* string);

#endif
