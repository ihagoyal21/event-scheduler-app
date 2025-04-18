// backtracking.cpp
#include "backtracking.hpp"

BacktrackingScheduler::BacktrackingScheduler(const std::vector<int>& ev, const std::map<int, std::vector<int>>& conf)
    : events(ev), conflicts(conf) {}

bool BacktrackingScheduler::can_assign(int event, int slot) {
    for (int c : conflicts[event]) {
        if (solution.count(c) && solution[c] == slot)
            return false;
    }
    return true;
}

bool BacktrackingScheduler::solve(int event_index, const std::vector<int>& slots) {
    if (event_index == events.size()) return true;
    int event = events[event_index];
    for (int slot : slots) {
        if (can_assign(event, slot)) {
            solution[event] = slot;
            if (solve(event_index + 1, slots)) return true;
            solution.erase(event);
        }
    }
    return false;
}

std::map<int, int> BacktrackingScheduler::get_schedule() {
    if (solve()) return solution;
    return {};
}
