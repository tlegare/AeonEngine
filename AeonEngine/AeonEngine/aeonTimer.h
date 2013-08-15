//
//  aeonTimer.h
//  AeonEngine
//
//  Created by Isaac Turner on 6/3/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#ifndef AeonEngine_aeonTimer_h
#define AeonEngine_aeonTimer_h

#include "aeonKernel.h"

typedef unsigned int aeon_TimerHandle;

aeon_TimerHandle aeon_CreateTimer(int duration, aeon_LoopType type);
void aeon_DestroyTimer(aeon_TimerHandle timer);

void aeon_StartTimer(aeon_TimerHandle timer);
void aeon_StopTimer(aeon_TimerHandle timer);

void aeon_SetTimerDuration(int t, aeon_TimerHandle timer);
void aeon_SetTimerType(aeon_LoopType type, aeon_TimerHandle timer);
void aeon_SetTimerCallback(int (*callback)(aeon_TimerHandle), aeon_TimerHandle timer);

#endif
