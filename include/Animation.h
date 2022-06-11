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

#ifndef __ANIMATION
#define __ANIMATION

#include <vector>
#include <array>

#include <gba_types.h>

struct Frame {
    u8 tileIndex{ 255 };    
    u8 frameTime{ 1 };
    u8 flipFlags{ 0 };
};

struct Sprite {
    u8 id{ 255 };
    u8 xOffset{ 0 };
    u8 yOffset{ 0 };
    u8 currentTime{ 0 };
    u8 currentFrame{ 0 };
    bool flipAllowed{ false };
};

typedef std::array<Frame, 16> SpriteFrames;

class Animation {
    private:
        std::vector<SpriteFrames> frames;
        bool flips;

    public:
        Animation();
        Animation(const std::vector<SpriteFrames>&& frames, const bool flips = false);

        SpriteFrames& getFrames(u8 sprite);
        const Frame& getFrame(u8 sprite, u8 index) const;
        const u8 getLength() const;

        void setFrame(const Sprite& sprite, Frame& frame);
        void addFrames(u8 sprite, const SpriteFrames& newFrames);

        const bool hasFlips() const;
};

#endif