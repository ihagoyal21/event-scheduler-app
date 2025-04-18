// greedy_scheduler.hpp
#pragma once
#include <vector>
#include <map>

class GreedyScheduler {
public:
    GreedyScheduler(const std::vector<int>& events, const std::map<int, std::vector<int>>& conflicts);
    std::map<int, int> assign_slots();
private:
    std::vector<int> events;
    std::map<int, std::vector<int>> conflicts;
    std::map<int, int> event_slots;
};
