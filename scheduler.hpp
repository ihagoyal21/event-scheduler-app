// scheduler.hpp
#pragma once
#include "models.hpp"
#include <vector>
#include <string>
#include <map>

int parse_time(const std::string& time_str);
std::string minutes_to_time(int minutes);
bool events_conflict(const Event& e1, const Event& e2, int min_gap_minutes = 60);
std::pair<int, int> find_available_time_slot(const Event& event, const std::vector<Event>& scheduled, int preferred_start_time);
std::vector<Event> smart_schedule(std::vector<Event> events);
