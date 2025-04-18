// graph_coloring.hpp
#pragma once
#include <vector>
#include <map>
#include <set>
#include <utility>

class GraphColoringScheduler {
public:
    GraphColoringScheduler(const std::vector<std::pair<int, int>>& event_conflicts);
    std::map<int, int> assign_slots();
private:
    std::vector<std::pair<int, int>> event_conflicts;
    std::map<int, int> event_slots;
};
