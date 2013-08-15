//
//  aeonInterp.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 6/6/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#include "aeonKernel.h"
#include "aeonInterp.h"
#include "aeonMath.h"
#include "aeonPool.h"

struct aeon_Interp : public PooledResource
{
    float* value = NULL;
    float start;
    float end;
    float time = 0;
    float destTime;
    aeon_InterpType type;
    int (*callback)(aeon_InterpHandle) = NULL;
    bool isRunning = false;
};

#define MAX_INTERP POOL_MAX

static aeonPool<aeon_Interp> interpPool;

#define LINEAR(x) (x)
#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x)))
#define SMOOTHERSTEP(x) ((x) * (x) * (x) * ((x) * ((x) * 6 - 15) + 10))
#define ACCELERATE(x) (x * x)
#define DECELERATE(x) (1 - (1 - x) * (1 - x))
#define CUBED_ACCEL(x) (x * x * x)
#define CUBED_DECEL(x) (1 - (1 - x) * (1 - x) * (1 - x))
#define SINE(x) (sin(v * PIover2))
void calculateNextValue(aeon_Interp* ip, float dt)
{
    float v = ip->time / ip->destTime;
    switch(ip->type)
    {
        case AEON_INTERP_LINEAR:
        {
            v = LINEAR(v);
            break;
        }
        case AEON_INTERP_SMOOTH:
        {
            v = SMOOTHSTEP(v);
            break;
        }
        case AEON_INTERP_SMOOTHER:
        {
            v = SMOOTHERSTEP(v);
            break;
        }
        case AEON_INTERP_DOUBLE_SMOOTH:
        {
            v = SMOOTHSTEP(SMOOTHSTEP(v));
            break;
        }
        case AEON_INTERP_DOUBLE_SMOOTHER:
        {
            v = SMOOTHERSTEP(SMOOTHERSTEP(v));
            break;
        }
        case AEON_INTERP_ACCEL:
        {
            v = ACCELERATE(v);
            break;
        }
        case AEON_INTERP_DECEL:
        {
            v = DECELERATE(v);
            break;
        }
        case AEON_INTERP_CUBED_ACCEL:
        {
            v = CUBED_ACCEL(v);
            break;
        }
        case AEON_INTERP_CUBED_DECEL:
        {
            v = CUBED_DECEL(v);
            break;
        }
        case AEON_INTERP_SINE:
        {
            v = SINE(v);
            break;
        }
        default:
            assert(0);
    }
    *(ip->value) = (ip->end * v) + (ip->start * (1 - v));
    
    if(ip->time >= ip->destTime)
    {
        ip->isRunning = false;
        ip->time = 0;
    }
    else
    {
        ip->time += dt;
    }
}

aeon_InterpHandle aeon_CreateInterpolator(float* value, float start, float end, float time, aeon_InterpType type)
{
    assert(value);
    
    aeon_Interp* newInterp = NULL;
    aeon_InterpHandle newInterpHandle = 0;
    
    unsigned int i = interpPool.getNextEmptySlot();
    newInterp = &interpPool.storage[i];
    newInterpHandle = interpPool.mangleHandle(i);

    
    newInterp->value = value;
    newInterp->start = start;
    newInterp->end = end;
    newInterp->destTime = time;
    newInterp->type = type;
    newInterp->isAlive = true;
    
    return newInterpHandle;
}

void aeon_SetInterpolatorCallback(int (*callback)(aeon_InterpHandle), aeon_InterpHandle h)
{
    aeon_Interp* ip = interpPool.getElement(h);
    ip->callback = callback;
}

void aeon_DestroyInterpolator(aeon_InterpHandle h)
{
    aeon_Interp* ip = interpPool.getElement(h);
    ip->value = NULL;
    ip->isRunning = false;
    ip->isAlive = false;
    ip->callback = NULL;
}

void aeon_StartInterpolator(aeon_InterpHandle h)
{
    aeon_Interp* ip = interpPool.getElement(h);
    ip->isRunning = true;
}

void aeon_StopInterpolator(aeon_InterpHandle h)
{
    aeon_Interp* ip = interpPool.getElement(h);
    ip->isRunning = false;
}

bool aeon_InterpolatorIsRunning(aeon_InterpHandle h)
{
    aeon_Interp* ip = interpPool.getElement(h);
    return ip->isRunning;
}

void aeon_ResetInterpolator(float start, float end, float time, aeon_InterpType type, aeon_InterpHandle h)
{
    aeon_Interp* ip = interpPool.getElement(h);
    ip->start = start;
    ip->end = end;
    ip->destTime = time;
    ip->type = type;
    ip->time = 0;
}

void aeon_UpdateInterpolators(float dt)
{
    for(int i = 0; i < MAX_INTERP; i++)
    {
        if(interpPool.storage[i].isAlive)
        {
            if(interpPool.storage[i].isRunning)
            {
                calculateNextValue(&interpPool.storage[i], dt);
                if(!interpPool.storage[i].isRunning)
                {
                    if(interpPool.storage[i].callback)
                    {
                        interpPool.storage[i].callback(interpPool.mangleHandle(i));
                    }
                }
            }
        }
    }
}