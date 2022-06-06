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

#ifndef __SCENEMANAGER
#define __SCENEMANAGER

#include <optional>
#include "Scene.h"

class SceneManager {
    private:
        Scene* currentScene;

        SceneManager();
    public:
        SceneManager(SceneManager const&) = delete;
        void operator=(SceneManager const&) = delete;

        void update();
        void draw();

        // Change to another scene
        void changeScene(Scene* newScene, const Player& player);

        static SceneManager& getInstance() {
            static SceneManager sm;
            return sm;
        }
};

#endif