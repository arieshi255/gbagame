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

#ifndef __EVENTS
#define __EVENTS

#include <string>
#include <queue>
#include <set>
#include <functional>

#include "Inputs.h"

class Event {
    protected:
        std::string name;
        InputContext inputContext;

    public:
        explicit Event(std::string name);

        Event(InputContext context);

        const std::string& getName() const { return name; }

        const InputContext& getContext() const { return inputContext; }
};

struct Listener {
    std::string event;
    std::function<void(Event&)> callback;

    bool operator<(const Listener listener) const {
        return this->event < listener.event;
    }
};

class EventManager {
    private:
        std::queue<Event> event_queue;
        std::set<Listener> listeners;
    public:
        EventManager();

        EventManager(EventManager const&) = delete;
        void operator=(EventManager const&) = delete;

        void addListener(Listener listener);

        void removeListener(Listener listener);

        void send(Event event);

        void pollEvents();

        static EventManager& getInstance() {
            static EventManager em;
            return em;
        }
};

#endif