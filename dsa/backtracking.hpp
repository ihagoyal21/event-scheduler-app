// backtracking.hpp
#pragma once
#include <vector>
#include <map>

class BacktrackingScheduler {
public:
    BacktrackingScheduler(const std::vector<int>& events, const std::map<int, std::vector<int>>& conflicts);
    std::map<int, int> get_schedule();
private:
    std::vector<int> events;
    std::map<int, std::vector<int>> conflicts;
    std::map<int, int> solution;
    bool can_assign(int event, int slot);
    bool solve(int event_index = 0, const std::vector<int>& slots = {0,1,2});
};
