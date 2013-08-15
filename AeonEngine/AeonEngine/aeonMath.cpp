//
//  aeonMath.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 6/9/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#include "aeonMath.h"

//----------------------------------------
// Aeon Functions
//----------------------------------------
float aeon_Map(float value, float min, float max, float new_min, float new_max)
{
    return (((value - min) * (new_max - new_min)) / (max - min)) + new_min;
}

float aeon_Dist(float x1, float y1, float x2, float y2)
{
    return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

float aeon_Dist(aeon_Point p1, aeon_Point p2)
{
    return sqrt(((p2.x - p1.x) * (p2.x - p1.x)) + ((p2.y - p1.y) * (p2.y - p1.y)));
}

float aeon_DegToRad(float angle)
{
    return angle * (PI / 180);
}

float aeon_RadToDeg(float angle)
{
    return angle * (180 / PI);
}