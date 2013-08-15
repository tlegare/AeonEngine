//
//  aeonTimer.cpp
//  AeonEngine
//
//  Created by Isaac Turner on 6/3/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#include "aeonTimer.h"
#include "aeonPool.h"

// Timers

#define MAX_TIMERS POOL_MAX

struct aeon_Timer : public PooledResource
{
    int type;
    int (*reachedZero)(aeon_TimerHandle) = NULL;
    float duration;
    float time;
    bool isRunning = false;
};

static aeonPool<aeon_Timer> timerPool;

aeon_TimerHandle aeon_CreateTimer(float duration, aeon_LoopType type)
{
    assert(duration > 0);
    
    aeon_Timer* newTimer = NULL;
    aeon_TimerHandle newTimerHandle = 0;
    
    unsigned int i = timerPool.getNextEmptySlot();
    newTimer = &timerPool.storage[i];
    newTimerHandle = timerPool.mangleHandle(i);
    
    newTimer->type = type;
    newTimer->reachedZero = NULL;
    newTimer->duration = duration;
    newTimer->isRunning = false;
    newTimer->isAlive = true;
    
    return newTimerHandle;
}

void aeon_StartTimer(aeon_TimerHandle timer)
{
    aeon_Timer* tp = timerPool.getElement(timer);
    tp->time = 0;
    tp->isRunning = true;
}

void aeon_StopTimer(aeon_TimerHandle timer)
{
    aeon_Timer* tp = timerPool.getElement(timer);
    tp->isRunning = false;
}

void aeon_SetTimerDuration(float t, aeon_TimerHandle timer)
{
    assert(t > 0);
    aeon_Timer* tp = timerPool.getElement(timer);
    tp->duration = t;
}

void aeon_SetTimerType(aeon_LoopType type, aeon_TimerHandle timer)
{
    aeon_Timer* tp = timerPool.getElement(timer);
    tp->type = type;
}

void aeon_SetTimerCallback(int (*callback)(aeon_TimerHandle), aeon_TimerHandle timer)
{
    aeon_Timer* tp = timerPool.getElement(timer);
    tp->reachedZero = callback;
}

void aeon_UpdateTimers(float dt)
{
    for(int i = 0; i < MAX_TIMERS; i++)
    {
        aeon_Timer* tp = &timerPool.storage[i];
        if(tp->isAlive)
        {
            if(tp->isRunning)
            {
                tp->time += dt;
                
                if(tp->time >= tp->duration)
                {
                    assert(tp->reachedZero);
                    tp->reachedZero(timerPool.mangleHandle(i));
                    if(tp->type == AEON_LOOP)
                    {
                        tp->time = 0;
                    }
                    else
                    {
                        tp->isRunning = false;
                        if(tp->type == AEON_ONESHOT_DESTROY)
                        {
                            tp->isAlive = false;
                        }
                    }
                }
            }
        }
    }
}

void aeon_DestroyTimer(aeon_TimerHandle timer)
{
    timerPool.killElement(timer);
}