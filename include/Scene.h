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

#ifndef __SCENE
#define __SCENE

#include "Player.h"

class Scene {
    public:
        Scene();

        // Load data (on start)
        virtual void load(const Player& player);
        // Unload data (on end)
        virtual void unload();        
        // Update logic (once a frame)
        virtual void update();
        // Draw graphics (once a frame)
        virtual void draw();
};

#endif