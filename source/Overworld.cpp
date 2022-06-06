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

#include <gba_base.h>
#include <gba_video.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>

#include "game_utils.h"
#include "overworld_t1.h"
#include "link.h"
#include "testmap.h"
#include "Events.h"

#include "Overworld.h"

OverworldScene::OverworldScene() { }

void OverworldScene::load(const Player& player) {
    // Load tileset palette
    CpuFastSet(overworld_t1Pal, BG_PALETTE, COPY32 | (overworld_t1PalLen / 2));
    // Load tiles into char block
    CpuSet(overworld_t1Tiles, CHAR_BASE_BLOCK(0), COPY32 | (overworld_t1TilesLen / 4));
    // Load tilemap into screen block
    CpuFastSet(GBAOW, SCREEN_BASE_BLOCK(8), COPY32 | 32 * 32);

    // Load OBJ palette
    CpuFastSet(linkPal, SPRITE_PALETTE, COPY32 | (linkPalLen / 2));
    // Load sprites into OBJ memory
    CpuFastSet(linkTiles, CHAR_BASE_BLOCK(4), COPY32 | (linkTilesLen / 4));

    // Activate the tilemap for bg0
    REG_BG0CNT = BG_SIZE_0 | BG_16_COLOR | SCREEN_BASE(8) | CHAR_BASE(0) | BG_PRIORITY(3);
}

void OverworldScene::unload() {

}

void OverworldScene::update() {

}

void OverworldScene::draw() {

}