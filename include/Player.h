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

#ifndef __PLAYER
#define __PLAYER

#include "Animation.h"
#include "Events.h"

#define SPRITES 2
#define SPEED 1

enum PlayerAnimation : u8 {
    IDLE_UP,
    IDLE_DOWN,
    IDLE_LEFT,
    IDLE_RIGHT,
    WALK_UP,
    WALK_DOWN,
    WALK_LEFT,
    WALK_RIGHT
};

class Player {
    private:
        std::array<Animation, 8> animations{};
        Animation* currentAnim;

        std::array<Sprite, SPRITES> playerSprite;

        int16_t x, y;

        u8 direction;

        State state;

    public:
        Player();
        Player(Player&&) = default;

        void update();

        void inputEvent(InputContext& ctx);

        void movePlayer(int16_t x, int16_t y);

        // Move these functions to some kind of animation class

        void setFrame(u8 sprite, u8 tileIndex);

        void pushAnim(const PlayerAnimation& name, const Animation& anim);

        void playCurrentAnim(const Animation& anim);

        void changeAnim(const PlayerAnimation& name);

        void changeAnim(u8 animIndex);
};

#endif