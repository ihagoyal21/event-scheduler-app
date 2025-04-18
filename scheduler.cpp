#include "scheduler.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <ctime>

int parse_time(const std::string& time_str) {
    std::tm tm = {};
    std::istringstream ss(time_str);
    if (ss >> std::get_time(&tm, "%H:%M")) {
        return tm.tm_hour * 60 + tm.tm_min;
    }
    throw std::runtime_error("Unsupported time format: " + time_str);
}

std::string minutes_to_time(int minutes) {
    int hours = minutes / 60;
    int mins = minutes % 60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << mins;
    return oss.str();
}

bool events_conflict(const Event& e1, const Event& e2, int min_gap_minutes) {
    if (e1.date != e2.date || e1.venue != e2.venue) return false;
    int start1 = parse_time(e1.start_time), end1 = parse_time(e1.end_time);
    int start2 = parse_time(e2.start_time), end2 = parse_time(e2.end_time);
    return !(end1 + min_gap_minutes <= start2 || start1 >= end2 + min_gap_minutes);
}

std::pair<int, int> find_available_time_slot(const Event& event, const std::vector<Event>& scheduled, int preferred_start_time) {
    int duration = event.duration;
    int day_minutes = 24 * 60;
    int min_gap = 60;
    
    // Try preferred slot first
    int start = preferred_start_time, end = preferred_start_time + duration;
    bool ok = true;
    for (const auto& s : scheduled) {
        if (s.venue == event.venue && s.date == event.date) {
            int s_start = parse_time(s.start_time), s_end = parse_time(s.end_time);
            if (!(end + min_gap <= s_start || start >= s_end + min_gap)) {
                ok = false; 
                break;
            }
        }
    }
    if (ok) return {start, end};
    
    // Create and sort hour blocks by distance from preferred time
    std::vector<int> hour_blocks;
    for (int hour = 0; hour < 24; ++hour) {
        int block_start = hour * 60;
        int block_end = block_start + duration;
        if (block_end <= day_minutes) {
            hour_blocks.push_back(block_start);
        }
    }
    
    // Sort blocks by distance from preferred time
    std::sort(hour_blocks.begin(), hour_blocks.end(), 
              [preferred_start_time, day_minutes](int a, int b) {
                  int dist_a = std::min(std::abs(a - preferred_start_time), 
                                        day_minutes - std::abs(a - preferred_start_time));
                  int dist_b = std::min(std::abs(b - preferred_start_time), 
                                        day_minutes - std::abs(b - preferred_start_time));
                  return dist_a < dist_b;
              });
    
    // Try each hour block
    for (int start_time : hour_blocks) {
        start = start_time;
        end = start + duration;
        ok = true;
        for (const auto& s : scheduled) {
            if (s.venue == event.venue && s.date == event.date) {
                int s_start = parse_time(s.start_time), s_end = parse_time(s.end_time);
                if (!(end + min_gap <= s_start || start >= s_end + min_gap)) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) return {start, end};
    }
    
    // Quadratic probing
    int i = 1;
    int max_attempts = 24;
    while (i <= max_attempts) {
        // Calculate offset using quadratic function (i²)
        int offset = i * i * 60; // Convert to minutes
        
        // Try positive offset from preferred time
        int pos_probe = (preferred_start_time + offset) % day_minutes;
        pos_probe = (pos_probe / 60) * 60; // Round to nearest hour
        
        if (pos_probe + duration <= day_minutes) {
            start = pos_probe;
            end = start + duration;
            ok = true;
            for (const auto& s : scheduled) {
                if (s.venue == event.venue && s.date == event.date) {
                    int s_start = parse_time(s.start_time), s_end = parse_time(s.end_time);
                    if (!(end + min_gap <= s_start || start >= s_end + min_gap)) {
                        ok = false;
                        break;
                    }
                }
            }
            if (ok) return {start, end};
        }
        
        // Try negative offset from preferred time
        int neg_probe = (preferred_start_time - offset) % day_minutes;
        if (neg_probe < 0) neg_probe += day_minutes; // Handle negative modulo
        neg_probe = (neg_probe / 60) * 60; // Round to nearest hour
        
        if (neg_probe + duration <= day_minutes) {
            start = neg_probe;
            end = start + duration;
            ok = true;
            for (const auto& s : scheduled) {
                if (s.venue == event.venue && s.date == event.date) {
                    int s_start = parse_time(s.start_time), s_end = parse_time(s.end_time);
                    if (!(end + min_gap <= s_start || start >= s_end + min_gap)) {
                        ok = false;
                        break;
                    }
                }
            }
            if (ok) return {start, end};
        }
        
        i++;
    }
    
    // Last resort: try every hour
    for (int hour = 0; hour < 24; ++hour) {
        start = hour * 60;
        end = start + duration;
        ok = true;
        for (const auto& s : scheduled) {
            if (s.venue == event.venue && s.date == event.date) {
                int s_start = parse_time(s.start_time), s_end = parse_time(s.end_time);
                if (!(end + min_gap <= s_start || start >= s_end + min_gap)) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) return {start, end};
    }
    
    // Absolute fallback: start at midnight
    return {0, duration};
}

std::vector<Event> smart_schedule(std::vector<Event> events) {
    for (auto& event : events) {
        event.duration = parse_time(event.end_time) - parse_time(event.start_time);
    }
    std::sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
        return std::make_tuple(a.date, a.venue, !a.priority, parse_time(a.start_time)) <
               std::make_tuple(b.date, b.venue, !b.priority, parse_time(b.start_time));
    });
    std::vector<Event> scheduled;
    for (auto& event : events) {
        bool ok = true;
        for (const auto& s : scheduled) {
            if (events_conflict(event, s, 60)) {
                ok = false; break;
            }
        }
        if (ok) {
            scheduled.push_back(event);
        } else {
            auto slot = find_available_time_slot(event, scheduled, parse_time(event.start_time));
            event.start_time = minutes_to_time(slot.first);
            event.end_time = minutes_to_time(slot.second);
            scheduled.push_back(event);
        }
    }
    return scheduled;
}
