// greedy_scheduler.cpp
#include "greedy_scheduler.hpp"
#include <algorithm>
#include <set>

GreedyScheduler::GreedyScheduler(const std::vector<int>& ev, const std::map<int, std::vector<int>>& conf)
    : events(ev), conflicts(conf) {}

std::map<int, int> GreedyScheduler::assign_slots() {
    auto cmp = [this](int a, int b) {
        return conflicts.at(a).size() < conflicts.at(b).size();
    };
    std::vector<int> sorted_events = events;
    std::sort(sorted_events.begin(), sorted_events.end(), cmp);

    for (int event : sorted_events) {
        std::set<int> assigned_slots;
        for (int neighbor : conflicts[event]) {
            if (event_slots.count(neighbor))
                assigned_slots.insert(event_slots[neighbor]);
        }
        int slot = 0;
        while (assigned_slots.count(slot)) ++slot;
        event_slots[event] = slot;
    }
    return event_slots;
}
