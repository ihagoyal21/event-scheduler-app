// priority_queue.cpp
#include "priority_queue.hpp"
#include <queue>

PriorityQueueScheduler::PriorityQueueScheduler(const std::map<int, std::vector<int>>& ec)
    : event_conflicts(ec) {}

std::map<int, int> PriorityQueueScheduler::assign_slots() {
    std::map<int, int> event_priority;
    for (const auto& [event, conflicts] : event_conflicts)
        event_priority[event] = conflicts.size();

    using Pair = std::pair<int, int>;
    auto cmp = [](Pair left, Pair right) { return left.first > right.first; };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> min_heap(cmp);

    for (const auto& [event, priority] : event_priority)
        min_heap.emplace(priority, event);

    std::map<int, int> slots;
    int available_slots = 0;
    while (!min_heap.empty()) {
        int event = min_heap.top().second;
        min_heap.pop();
        slots[event] = available_slots++;
    }
    return slots;
}
