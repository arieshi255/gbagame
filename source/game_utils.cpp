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

#include <gba_dma.h>

#include "game_utils.h"

OAMTable sprites;

void initSprites() {
    SpriteMgr.currentSprite = 0;

    u32* dest = (u32*)&sprites;

    for (u8 i = 0; i < 128; i++) {
        *dest++ = ATTR0_DISABLED;
        *dest++ = 0;
    }

    oamCopy(sprites.oamBuffer);
}

u8 addSprite(int16_t x, int16_t y, u32 tile) {
    sprites.oamBuffer[SpriteMgr.currentSprite].attr0 = OBJ_Y(y) | ATTR0_SQUARE | ATTR0_NORMAL | ATTR0_COLOR_16;
    sprites.oamBuffer[SpriteMgr.currentSprite].attr1 = OBJ_X(x) | ATTR1_SIZE_16;
    sprites.oamBuffer[SpriteMgr.currentSprite].attr2 = OBJ_CHAR(tile) | OBJ_PALETTE(0);

    SpriteMgr.currentSprite += 1;

    return SpriteMgr.currentSprite - 1;
}

u8 setSpritePosition(u8 sprite, int16_t x, int16_t y) {
    // TODO: handle clipping
    uint32_t properties = (sprites.oamBuffer[sprite].attr0 >> 8) | (sprites.oamBuffer[sprite].attr1 & 0xFF00);

    sprites.oamBuffer[sprite].attr0 = y & 0xFF | sprites.oamBuffer[sprite].attr0 & 0xFF00;
    sprites.oamBuffer[sprite].attr1 = x & 0x1FF | sprites.oamBuffer[sprite].attr1 & 0xFE00;

    return sprite;
}

u8 setSpriteTiles(u8 sprite, u32 startTile) {
    sprites.oamBuffer[sprite].attr2 = startTile & 0x1FF | sprites.oamBuffer[sprite].attr2 & 0xFE00;

    return sprite;
}

void oamCopy(const OBJATTR* src) {
    CpuFastSet((void*)src, (void*)OAM, COPY32 | (sizeof(OAMTable) >> 2));

    //DMA0COPY(src, OAM, DMA32 | (sizeof(OAMTable) >> 2));

    //*OBJATTR* oam_mem = OAM;
}