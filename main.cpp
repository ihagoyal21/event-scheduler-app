#include "third_party/crow_all.h"
#include "third_party/json.hpp"
#include "models.hpp"
#include "scheduler.hpp"
#include "dsa/graph_coloring.hpp"
#include "dsa/backtracking.hpp"
#include "dsa/greedy_scheduler.hpp"
#include "dsa/priority_queue.hpp"
#include "dsa/segment_tree.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>

int main() {
    // Print current working directory for debugging
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    
    // Create Crow app with CORS middleware
    crow::App<crow::CORSHandler> app;
    
    // Configure CORS
    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors
        .global()
        .methods("POST"_method, "GET"_method, "PUT"_method, "DELETE"_method, "OPTIONS"_method)
        .headers("Content-Type", "Authorization")
        .origin("*")
        .allow_credentials();

    // Serve index.html at root
    CROW_ROUTE(app, "/")([]() {
        crow::response res;
        res.set_static_file_info("static/index.html");
        return res;
    });

    // Serve static assets (JS, CSS, etc.)
    CROW_ROUTE(app, "/static/<string>")
    ([](const crow::request&, std::string filename){
        crow::response res;
        res.set_static_file_info("static/" + filename);
        return res;
    });

    // Graph Coloring
    CROW_ROUTE(app, "/assign_slots/graph_coloring/").methods("POST"_method, "OPTIONS"_method)
    ([](const crow::request& req){
        if (req.method == "OPTIONS"_method) {
            return crow::response(200);
        }
        
        auto body = nlohmann::json::parse(req.body);
        std::vector<std::pair<int, int>> event_conflicts = body["event_conflicts"].get<std::vector<std::pair<int, int>>>();
        GraphColoringScheduler scheduler(event_conflicts);
        auto slots = scheduler.assign_slots();
        nlohmann::json result = {{"event_slots", slots}};
        return crow::response(result.dump(4)); // Pretty-print with 4 spaces
    });

    // Backtracking
    CROW_ROUTE(app, "/assign_slots/backtracking/").methods("POST"_method, "OPTIONS"_method)
    ([](const crow::request& req){
        if (req.method == "OPTIONS"_method) {
            return crow::response(200);
        }
        
        auto body = nlohmann::json::parse(req.body);
        std::vector<int> events = body["events"].get<std::vector<int>>();
        std::map<int, std::vector<int>> conflicts = body["conflicts"].get<std::map<int, std::vector<int>>>();
        BacktrackingScheduler scheduler(events, conflicts);
        auto schedule = scheduler.get_schedule();
        nlohmann::json result = {{"event_slots", schedule}};
        return crow::response(result.dump(4));
    });

    // Greedy
    CROW_ROUTE(app, "/assign_slots/greedy/").methods("POST"_method, "OPTIONS"_method)
    ([](const crow::request& req){
        if (req.method == "OPTIONS"_method) {
            return crow::response(200);
        }
        
        auto body = nlohmann::json::parse(req.body);
        std::vector<int> events = body["events"].get<std::vector<int>>();
        std::map<int, std::vector<int>> conflicts = body["conflicts"].get<std::map<int, std::vector<int>>>();
        GreedyScheduler scheduler(events, conflicts);
        auto slots = scheduler.assign_slots();
        nlohmann::json result = {{"event_slots", slots}};
        return crow::response(result.dump(4));
    });

    // Priority Queue
    CROW_ROUTE(app, "/assign_slots/priority_queue/").methods("POST"_method, "OPTIONS"_method)
    ([](const crow::request& req){
        if (req.method == "OPTIONS"_method) {
            return crow::response(200);
        }
        
        auto body = nlohmann::json::parse(req.body);
        std::map<int, std::vector<int>> event_conflicts = body["event_conflicts"].get<std::map<int, std::vector<int>>>();
        PriorityQueueScheduler scheduler(event_conflicts);
        auto slots = scheduler.assign_slots();
        nlohmann::json result = {{"event_slots", slots}};
        return crow::response(result.dump(4));
    });

    // Segment Tree
    CROW_ROUTE(app, "/assign_slots/segment_tree/").methods("POST"_method, "OPTIONS"_method)
    ([](const crow::request& req){
        if (req.method == "OPTIONS"_method) {
            return crow::response(200);
        }
        
        auto body = nlohmann::json::parse(req.body);
        std::vector<int> arr = body["array"].get<std::vector<int>>();
        SegmentTreeScheduler scheduler(arr);
        nlohmann::json result = {{"segment_tree", scheduler.tree}};
        return crow::response(result.dump(4));
    });

    // Smart Scheduler
    CROW_ROUTE(app, "/schedule_events/").methods("POST"_method, "OPTIONS"_method)
    ([](const crow::request& req){
        if (req.method == "OPTIONS"_method) {
            return crow::response(200);
        }
        
        try {
            auto body = nlohmann::json::parse(req.body);
            std::vector<Event> events = body["events"].get<std::vector<Event>>();
            auto scheduled = smart_schedule(events);
            nlohmann::json result = {{"scheduled_events", scheduled}};
            return crow::response(result.dump(4));
        } catch (const std::exception& e) {
            return crow::response(400, std::string("Error: ") + e.what());
        }
    });

    app.port(8080).multithreaded().run();
}
