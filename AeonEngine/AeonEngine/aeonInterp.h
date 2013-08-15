//
//  aeonInterp.h
//  AeonEngine
//
//  Created by Isaac Turner on 6/6/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#ifndef AeonEngine_aeonInterp_h
#define AeonEngine_aeonInterp_h

typedef unsigned int aeon_InterpHandle;

enum aeon_InterpType
{
    AEON_INTERP_LINEAR,
    AEON_INTERP_SMOOTH, AEON_INTERP_SMOOTHER,
    AEON_INTERP_DOUBLE_SMOOTH, AEON_INTERP_DOUBLE_SMOOTHER,
    AEON_INTERP_ACCEL, AEON_INTERP_DECEL,
    AEON_INTERP_CUBED_ACCEL, AEON_INTERP_CUBED_DECEL,
    AEON_INTERP_SINE
};

aeon_InterpHandle aeon_CreateInterpolator(float* value, float start, float end, float time, aeon_InterpType type);
void aeon_DestroyInterpolator(aeon_InterpHandle h);

void aeon_SetInterpolatorCallback(int (*callback)(aeon_InterpHandle), aeon_InterpHandle h);

void aeon_StartInterpolator(aeon_InterpHandle h);
void aeon_StopInterpolator(aeon_InterpHandle h);
void aeon_ResetInterpolator(float start, float end, float time, aeon_InterpType type, aeon_InterpHandle h);

bool aeon_InterpolatorIsRunning(aeon_InterpHandle h);

#endif
