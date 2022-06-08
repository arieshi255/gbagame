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

#include "Animation.h"

Animation::Animation() { }

Animation::Animation(const std::vector<SpriteFrames>&& frames) : frames(frames) { }

const SpriteFrames& Animation::getFrames(u8 sprite) const {
    return frames[sprite];
}

const Frame& Animation::getFrame(u8 sprite, u8 index) const {
    return frames[sprite][index];
}

const u8 Animation::getLength() const {
    return frames.size();
}

void Animation::addFrames(u8 sprite, const SpriteFrames& newFrames) {
    frames.push_back(newFrames);
}