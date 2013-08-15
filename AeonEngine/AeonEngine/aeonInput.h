//
//  aeonInput.h
//  AeonEngine
//
//  Created by Isaac Turner on 7/27/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#ifndef AeonEngine_aeonInput_h
#define AeonEngine_aeonInput_h

//----------------------------------------
// Aeon Keyboard Constants
//----------------------------------------
#define AEON_KEY_IS_DOWN true
#define AEON_KEY_IS_UP false
#define AEON_KEYTYPE_NOREPEAT 0
#define AEON_KEYTYPE_NORMAL 1

#define AEON_KEY_ESCAPE 27

//----------------------------------------
// Aeon Functions
//----------------------------------------
bool aeon_GetKeyState(char key);
void aeon_SetKeyRepeat(int keyType, char key);

#endif
