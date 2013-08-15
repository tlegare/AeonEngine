//
//  aeonMath.h
//  AeonEngine
//
//  Created by Isaac Turner on 6/9/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#ifndef AeonEngine_aeonMath_h
#define AeonEngine_aeonMath_h

#include <math.h>
#include "aeonKernel.h"

// Math
#define PI 3.141592653589793
#define PIover2 1.5707963267949

float aeon_Map(float value, float min, float max, float new_min, float new_max);

float aeon_Dist(float x1, float y1, float x2, float y2);
float aeon_Dist(aeon_Point p1, aeon_Point p2);

float aeon_DegToRad(float angle);
float aeon_RadToDeg(float angle);

#endif
