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

#include <gba_timers.h>
#include <gba_interrupt.h>
#include "Events.h"

Event::Event(std::string name) : name(name) { }

Event::Event(InputContext context) : inputContext(context) {
    name = context.event;
}

EventManager::EventManager() { }

void EventManager::addListener(Listener listener) {
    if (listeners.find(listener) == listeners.end())
        listeners.insert(listener);
}

void EventManager::removeListener(Listener listener) {
    auto found = listeners.find(listener);

    if (found != listeners.end())
        listeners.erase(listener);
}

void EventManager::send(Event event) {
    event_queue.push(event);
}

void EventManager::pollEvents() {
    while (!event_queue.empty()) {
        Event& event = event_queue.front();

        for (auto& listener : listeners) {
            if (event.getName() == listener.event)
                listener.callback(event);
        }

        event_queue.pop();
    }
}