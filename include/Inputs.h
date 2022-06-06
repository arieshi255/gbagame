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

#ifndef __INPUTS
#define __INPUTS

#include <functional>
#include <string>
#include <queue>
#include <set>
#include <map>

#include <gba_input.h>

enum class Action {
    WalkUp,
    WalkDown,
    WalkLeft,
    WalkRight,
    Attack,
    Pause
};

enum class State {
    Idle,
    Walk,
    AttackCharge,
    PausedState
};

enum class KeyType {
    ALL,
    KEYPRESS,
    KEYHOLD,
    KEYRELEASE
};

struct InputContext {
    std::string event;
    Action action;
    State state;
    KeyType keyType{ KeyType::ALL };
    u8 priority;
    int keys{ 0 };
    KeyType keyEvent;
};

// Maybe use this instead of inputcontext?
// In each frame, input is mapped to mappedinput and sent to all callbacks
struct MappedInput {
    std::set<Action> actions;
};

//typedef void (*InputCallback)(MappedInput& inputs);

//typedef void (*InputCallback)(InputContext& context);

//typedef std::function<void(MappedInput&)> (*InputCallback)(MappedInput& inputs);

class InputMapper {
    private:
        MappedInput currentInput;

        std::set<InputContext> contexts;
        std::vector<std::pair<int, InputContext>> activeContexts;

        std::multimap<u8, std::function<void(InputContext&)>> callbackTable;
    public:
        InputMapper();

        void pushContext(int keypad, const InputContext& context);

        void popContext(int keypad);

        void registerCallback(std::function<void(InputContext&)> callback, u8 priority);

        void setButtonState(KEYPAD_BITS button, bool pressed, bool previous);

        void clear();

        void dispatch(KeyType keyType, u16 keys);
};

#endif