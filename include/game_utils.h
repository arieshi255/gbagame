// Copyright 2022 Skyliegirl33
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __GAMEUTILS
#define __GAMEUTILS

#include <gba_base.h>
#include <gba_systemcalls.h>
#include <gba_sprites.h>

union OAMTable {
    OBJATTR oamBuffer[128];
    OBJAFFINE matrixBuffer[32];
} ALIGN(4);

struct {
    u8 currentSprite;
} SpriteMgr;

extern OAMTable sprites;

void initSprites();

u8 addSprite(int16_t x, int16_t y, u32 tile);

u8 setSpritePosition(u8 sprite, int16_t x, int16_t y);

u8 setSpriteTiles(u8 sprite, u32 startTile);

void oamCopy(const OBJATTR* src);

#endif