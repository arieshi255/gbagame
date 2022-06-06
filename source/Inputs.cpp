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

#include "Inputs.h"
#include "Events.h"

InputMapper::InputMapper() { }

void InputMapper::pushContext(int keypad, const InputContext& context) {
    activeContexts.push_back(std::make_pair(keypad, context));
}

void InputMapper::popContext(int keypad) {
    if (!activeContexts.empty())
        activeContexts.erase(activeContexts.begin());
}

void InputMapper::registerCallback(std::function<void(InputContext&)> callback, u8 priority) {
    callbackTable.insert(std::make_pair(priority, callback));
}

void InputMapper::setButtonState(KEYPAD_BITS button, bool pressed, bool previous) {
    if (pressed) {
        for (auto& ctx : activeContexts) {
            int keypad = std::get<0>(ctx);
            InputContext& input = std::get<1>(ctx);

            if (button & keypad) {
                currentInput.actions.insert(input.action);
                return;
            }
        }
    }

    /*for (auto& ctx : activeContexts) {
        int keypad = std::get<0>(ctx);
        InputContext& input = std::get<1>(ctx);

        if (keypad & keysD) {
            input.keyType = KeyType::KEYPRESS;
            input.keys = keysD;
            em.send(Event(input));
        } else if (keypad & keysH) {
            input.keyType = KeyType::KEYHOLD;
            input.keys = keysH;
            em.send(Event(input));
        } else if (keypad & keysR) {
            input.keyType = KeyType::KEYRELEASE;
            input.keys = keysR;
            em.send(Event(input));
        }
    }*/
}

void InputMapper::clear() {
    currentInput.actions.clear();
}

void InputMapper::dispatch(KeyType keyType, u16 keys) {
    EventManager& em = EventManager::getInstance();

    for (auto& ctx : activeContexts) {
        int keypad = std::get<0>(ctx);

        if (keypad & keys) {
            InputContext& input = std::get<1>(ctx);

            if (input.keyType != KeyType::ALL && keyType != input.keyType)
                continue;

            input.keyEvent = keyType;
            input.keys = keys;

            for (auto& callback : callbackTable) {
                callback.second(input);
            }
        }
    }
}