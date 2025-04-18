// priority_queue.hpp
#pragma once
#include <map>
#include <vector>

class PriorityQueueScheduler {
public:
    PriorityQueueScheduler(const std::map<int, std::vector<int>>& event_conflicts);
    std::map<int, int> assign_slots();
private:
    std::map<int, std::vector<int>> event_conflicts;
};
