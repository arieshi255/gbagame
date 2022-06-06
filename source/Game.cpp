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
#include <gba_interrupt.h>
#include <gba_input.h>

#include <maxmod.h>

#include "soundbank.h"
#include "soundbank_bin.h"

#include "Game.h"

Game::Game() { }

void Game::init() {
    mapper.pushContext(KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT, InputContext{
        .event = "playerMove",
        .state = State::Walk,
        .priority = 0
    });

    mapper.pushContext(KEY_B, InputContext{
        .event = "playerAttack",
        .action = Action::Attack,
        .keyType = KeyType::KEYPRESS,
        .priority = 0
    });

    mapper.pushContext(KEY_START, InputContext{
        .event = "pause",
        .action = Action::Pause,
        .keyType = KeyType::KEYPRESS,
        .priority = 0
    });

    mapper.pushContext(KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT, InputContext{
        .event = "uiMove",
        .state = State::PausedState,
        .priority = 0
    });

    // Register input events to player
    mapper.registerCallback(std::bind(&Player::inputEvent, &player, std::placeholders::_1), 0);
}

void Game::run(Scene* scene) {
    // Get scene manager singleton
    SceneManager& sm = SceneManager::getInstance();
    // Get event manager singleton
    EventManager& em = EventManager::getInstance();

    // Load scene
    sm.changeScene(scene, player);

    while (true) {
        VBlankIntrWait();
        scanKeys();

        u16 keysD = keysDown();
        u16 keysH = keysHeld();
        u16 keysU = keysUp();
        
        if (keysD)
            mapper.dispatch(KeyType::KEYPRESS, keysD);
        
        if (keysH)
            mapper.dispatch(KeyType::KEYHOLD, keysH);
        
        if (keysU)
            mapper.dispatch(KeyType::KEYRELEASE, keysU);
        
        player.update();

        sm.update();
        sm.draw();

        mmFrame();
        em.pollEvents();

        oamCopy(sprites.oamBuffer);
    }
}