#pragma once
#include <string>
#include <vector>
#include <map>
#include <utility> // for std::pair
#include "third_party/json.hpp"

// --- Event ---
struct Event {
    std::string username;
    std::string event_name;
    std::string venue;
    std::string date;
    std::string start_time;
    std::string end_time;
    bool priority;
    int duration;
};

inline void to_json(nlohmann::json& j, const Event& e) {
    j = nlohmann::json{
        {"username", e.username},
        {"event_name", e.event_name},
        {"venue", e.venue},
        {"date", e.date},
        {"start_time", e.start_time},
        {"end_time", e.end_time},
        {"priority", e.priority},
        {"duration", e.duration}
    };
}

inline void from_json(const nlohmann::json& j, Event& e) {
    j.at("username").get_to(e.username);
    j.at("event_name").get_to(e.event_name);
    j.at("venue").get_to(e.venue);
    j.at("date").get_to(e.date);
    j.at("start_time").get_to(e.start_time);
    j.at("end_time").get_to(e.end_time);
    j.at("priority").get_to(e.priority);
    j.at("duration").get_to(e.duration);
}

// --- SmartSchedulerInput ---
struct SmartSchedulerInput {
    std::vector<Event> events;
};

inline void to_json(nlohmann::json& j, const SmartSchedulerInput& s) {
    j = nlohmann::json{{"events", s.events}};
}

inline void from_json(const nlohmann::json& j, SmartSchedulerInput& s) {
    j.at("events").get_to(s.events);
}

// --- ConflictInput ---
struct ConflictInput {
    std::vector<std::pair<int, int>> event_conflicts;
};

inline void to_json(nlohmann::json& j, const ConflictInput& c) {
    j = nlohmann::json{{"event_conflicts", c.event_conflicts}};
}

inline void from_json(const nlohmann::json& j, ConflictInput& c) {
    j.at("event_conflicts").get_to(c.event_conflicts);
}

// --- EventInput ---
struct EventInput {
    std::vector<int> events;
    std::map<int, std::vector<int>> conflicts;
};

inline void to_json(nlohmann::json& j, const EventInput& e) {
    j = nlohmann::json{
        {"events", e.events},
        {"conflicts", e.conflicts}
    };
}

inline void from_json(const nlohmann::json& j, EventInput& e) {
    j.at("events").get_to(e.events);
    j.at("conflicts").get_to(e.conflicts);
}

// --- ArrayInput ---
struct ArrayInput {
    std::vector<int> array;
};

inline void to_json(nlohmann::json& j, const ArrayInput& a) {
    j = nlohmann::json{{"array", a.array}};
}

inline void from_json(const nlohmann::json& j, ArrayInput& a) {
    j.at("array").get_to(a.array);
}
