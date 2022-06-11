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

#include <gba_video.h>

#include "game_utils.h"

#include "Player.h"

Player::Player() : x(20), y(20), direction(1), state(State::Idle) {
    playerSprite = {
        Sprite{
            .id = addSprite(x, y, 0)
        },
        Sprite{
            .id = addSprite(x, y + 8, 4),
            .yOffset = 8,
            .flipAllowed = true
        }
    };

    Animation idleu{
        {
            { Frame{.tileIndex = 80} },
            { Frame{.tileIndex = 84} } 
        }
    };

    Animation idled{
        {
            { Frame{.tileIndex = 0} },
            { Frame{.tileIndex = 4} }
        }
    };

    Animation idlel{
        {
            { Frame{.tileIndex = 40} },
            { Frame{.tileIndex = 44} }
        }
    };

    Animation idler{
        {
            { Frame{.tileIndex = 40} },
            { Frame{.tileIndex = 44} }
        }
    };

    // Start walking:
    // Frame 0: turn on flip
    // Frame 1: turn off flip

    // Loop:
    // Next cycle:
    // Frame 1: turn on flip
    // Next cycle:
    // Frame 1: turn off flip

    Animation walkd{
        {
            { Frame{.tileIndex = 0} },
            { Frame{.tileIndex = 8, .frameTime = 2, .flipFlags = 1}, Frame{.tileIndex = 12, .frameTime = 3}, Frame{.tileIndex = 16, .frameTime = 2}, Frame{.tileIndex = 20, .frameTime = 2} }
        },
        true
    };

    pushAnim(PlayerAnimation::IDLE_UP, idleu);
    pushAnim(PlayerAnimation::IDLE_DOWN, idled);
    pushAnim(PlayerAnimation::IDLE_LEFT, idlel);
    pushAnim(PlayerAnimation::IDLE_RIGHT, idler);

    pushAnim(PlayerAnimation::WALK_DOWN, walkd);

    changeAnim(PlayerAnimation::IDLE_DOWN);
}

void Player::update() {
    playCurrentAnim(*currentAnim);
}

void Player::movePlayer(int16_t x, int16_t y) {
    if (currentAnim == nullptr)
        return;
    // bleh, maybe have an array for idle/walk animations
    changeAnim(direction + 4);

    for (u8 i = 0; i < SPRITES; i++)
    {
        if (playerSprite[i].id != 255)
            setSpritePosition(playerSprite[i].id, x + playerSprite[i].xOffset, y + playerSprite[i].yOffset);
    }

    REG_BG0HOFS += x;
    REG_BG0VOFS += y;

    if (x > this->x)
        direction = 3;
    else if (x < this->x)
        direction = 2;
    else if (y > this->y)
        direction = 1;
    else if (y < this->y)
        direction = 0;

    this->x = x;
    this->y = y;
}

void Player::inputEvent(InputContext &ctx) {
    // iprintf("\x1b[0;0HDir: %i", direction);

    // Pause game if action is triggered
    if (ctx.action == Action::Pause) {
        state = state != State::PausedState ? State::PausedState : State::Idle;
    }

    // Paused, so return
    if (state == State::PausedState)
        return;

    // Movement state
    if (ctx.state == State::Walk) {
        if (ctx.keyEvent == KeyType::KEYHOLD) {
            if (ctx.keys & KEY_UP)
                movePlayer(x, y - SPEED);
            else if (ctx.keys & KEY_DOWN)
                movePlayer(x, y + SPEED);

            if (ctx.keys & KEY_LEFT)
                movePlayer(x - SPEED, y);
            else if (ctx.keys & KEY_RIGHT)
                movePlayer(x + SPEED, y);
        }

        if (ctx.keyEvent == KeyType::KEYRELEASE) {
            //changeAnim(direction);
        }
    }
}

void Player::setFrame(const Sprite& sprite, Frame& frame) {
    if (currentAnim) {
        setSpriteTiles(sprite.id, frame.tileIndex, frame.flipFlags);

        if (sprite.flipAllowed && currentAnim->hasFlips())
            frame.flipFlags = !frame.flipFlags;
    }
}

void Player::pushAnim(const PlayerAnimation &name, const Animation& anim) {
    animations[name] = anim;
}

void Player::playCurrentAnim(Animation& anim) {
    for (u8 i = 0; i < SPRITES; i++) {
        ++playerSprite[i].currentTime;

        auto& frames = anim.getFrames(i);

        if (playerSprite[i].currentTime >= frames[playerSprite[i].currentFrame].frameTime) {
            playerSprite[i].currentTime = 0;

            // Check to make sure the next frame is in bounds and isn't an invalid tile
            if (playerSprite[i].currentFrame + 1 < frames.size() && frames[playerSprite[i].currentFrame + 1].tileIndex != 255)
                ++playerSprite[i].currentFrame;
            else
                playerSprite[i].currentFrame = 0; // TODO: add support for non looping animations

            setFrame(playerSprite[i], frames[playerSprite[i].currentFrame]);
        }
    }
}

void Player::changeAnim(const PlayerAnimation &name) {
    Animation& animation = animations[name];

    if (currentAnim != &animation && animation.getLength() != 0) {
        currentAnim = &animation;
        
        for (u8 i = 0; i < SPRITES; i++) {
            playerSprite[i].currentFrame = 0;
        }
    }
}

void Player::changeAnim(u8 animIndex) {
    changeAnim(static_cast<PlayerAnimation>(animIndex));
}