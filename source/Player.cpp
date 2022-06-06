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

Player::Player() : x(20), y(20), direction(1), currentFrame(0), currentTime(0), state(State::Idle)
{
    playerSprite = {
        Sprite{
            .id = addSprite(x, y, 0)
        },
        Sprite{
            .id = addSprite(x, y + 8, 4),
            .yOffset = 8
        }
    };

    Animation idleu = Animation{
        .frames = {
            { playerSprite[0].id, {Frame{.tileIndex = 80} } },
            { playerSprite[1].id, {Frame{.tileIndex = 84} } }
        },
        .length = 1
    };

    Animation idled = Animation{
        .frames = {
            { playerSprite[0].id, {Frame{.tileIndex = 0} } },
            { playerSprite[1].id, {Frame{.tileIndex = 4} } }
        },
        .length = 1
    };

    Animation idlel = Animation{
        .frames = {
            { playerSprite[0].id, {Frame{.tileIndex = 40} } },
            { playerSprite[1].id, {Frame{.tileIndex = 44} } }
        },
        .length = 1
    };

    Animation idler = Animation{
        .frames = {
            { playerSprite[0].id, { Frame{.tileIndex = 40} } },
            { playerSprite[1].id, { Frame{.tileIndex = 44} } }
        },
        .length = 1
    };

    Animation walkd = Animation{
        .frames = {
            { playerSprite[1].id, { Frame{.tileIndex = 8}, Frame{.tileIndex = 12}, Frame{.tileIndex = 16}, Frame{.tileIndex = 20} } }
        },
        .length = 4
    };

    pushAnim(PlayerAnimation::IDLE_UP, idleu);
    pushAnim(PlayerAnimation::IDLE_DOWN, idled);
    pushAnim(PlayerAnimation::IDLE_LEFT, idlel);
    pushAnim(PlayerAnimation::IDLE_RIGHT, idler);

    pushAnim(PlayerAnimation::WALK_DOWN, walkd);

    changeAnim(PlayerAnimation::IDLE_DOWN);
}

void Player::update()
{
    playCurrentAnim(*currentAnim);
}

void Player::movePlayer(int16_t x, int16_t y)
{
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

void Player::inputEvent(InputContext &ctx)
{
    // const InputContext& ctx = event.getContext();

    // iprintf("\x1b[0;0HDir: %i", direction);

    // Pause game if action is triggered
    if (ctx.action == Action::Pause)
    {
        state = state != State::PausedState ? State::PausedState : State::Idle;
    }

    // Paused, so return
    if (state == State::PausedState)
        return;

    // Movement state
    if (ctx.state == State::Walk)
    {
        if (ctx.keyEvent == KeyType::KEYHOLD)
        {
            if (ctx.keys & KEY_UP)
                movePlayer(x, y - SPEED);
            else if (ctx.keys & KEY_DOWN)
                movePlayer(x, y + SPEED);

            if (ctx.keys & KEY_LEFT)
                movePlayer(x - SPEED, y);
            else if (ctx.keys & KEY_RIGHT)
                movePlayer(x + SPEED, y);
        }

        if (ctx.keyEvent == KeyType::KEYRELEASE)
        {
            changeAnim(direction);
        }
    }
}

void Player::setFrame(u8 sprite, u8 newFrame)
{
    if (currentAnim) {
        auto found = currentAnim->frames.find(sprite);

        if (found != currentAnim->frames.end() && newFrame < found->second.size())
            setSpriteTiles(sprite, found->second[newFrame].tileIndex);
    }
}

void Player::pushAnim(const PlayerAnimation &name, const Animation &anim)
{
    animations[name] = anim;
}

void Player::playCurrentAnim(const Animation &anim) {
    ++currentTime;

    for (u8 i = 0; i < SPRITES; i++) {
        u8 spriteId = playerSprite[i].id;
        auto found = anim.frames.find(spriteId);

        if (found != anim.frames.end() && currentTime >= found->second[currentFrame].frameTime)
        {
            currentTime = 0;

            if (currentFrame + 1 < anim.length)
                ++currentFrame;
            else
                currentFrame = 0; // TODO: add support for non looping animations

            setFrame(spriteId, currentFrame);
        }
    }
}

void Player::changeAnim(const PlayerAnimation &name)
{
    Animation &animation = animations[name];

    if (((currentAnim && currentAnim != &animation) || (currentAnim != &animation)) && animation.length != 0)
    {
        currentAnim = &animation;
        currentFrame = 0;
    }
}

void Player::changeAnim(u8 animIndex)
{
    changeAnim(static_cast<PlayerAnimation>(animIndex));
}