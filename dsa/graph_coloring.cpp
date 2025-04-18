// graph_coloring.cpp
#include "graph_coloring.hpp"
#include <algorithm>

GraphColoringScheduler::GraphColoringScheduler(const std::vector<std::pair<int, int>>& ec)
    : event_conflicts(ec) {}

std::map<int, int> GraphColoringScheduler::assign_slots() {
    std::map<int, std::vector<int>> adj;
    for (const auto& [e1, e2] : event_conflicts) {
        adj[e1].push_back(e2);
        adj[e2].push_back(e1);
    }
    std::vector<int> sorted_events;
    for (const auto& [event, _] : adj)
        sorted_events.push_back(event);
    std::sort(sorted_events.begin(), sorted_events.end(),
        [&adj](int a, int b) { return adj[a].size() > adj[b].size(); });
    for (int event : sorted_events) {
        std::set<int> assigned;
        for (int neighbor : adj[event])
            if (event_slots.count(neighbor))
                assigned.insert(event_slots[neighbor]);
        for (int slot = 0; slot <= adj.size(); ++slot) {
            if (!assigned.count(slot)) {
                event_slots[event] = slot;
                break;
            }
        }
    }
    return event_slots;
}
