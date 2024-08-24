
#pragma once
 
#include <algorithm>
#include <vector>
#include <string>
#include <variant>
 
#include "geo.h"
#include "graph.h"
 
namespace domain {
 
struct StatRequest { 
    int id;
    std::string type;
    std::string name;    
    std::string from;
    std::string to;
};
    
struct Bus;
    
struct Stop {    
    
    std::string name;
    geo::Coordinates coordinates; 
    std::vector<Bus*> buses;
};
 
struct Bus { 
    std::string name;
    std::vector<Stop*> stops;
    bool is_roundtrip;
};
 
struct Distance {    
    const Stop* A;
    const Stop* B;
    int distance = 0;
};  
 
struct BusInfoRoute {
    std::string_view name;
    bool not_found = false;
    int stops_on_route = 0;
    int unique_stops = 0;
    int route_length = 0;
    double curvature = 0.0;
};    
   
struct StopInfo {
    std::string_view name;
    bool not_found;
    std::vector<std::string> buses_name;
};
    
struct StopEdge {
    std::string_view name;
    double time = 0;
};
 
struct BusEdge {
    std::string_view bus_name;
    size_t span_count = 0;
    double time = 0;
};
 
struct RoutingSettings {
    double bus_wait_time = 0;
    double bus_velocity = 0;
};
 
struct RouterByStop {
    graph::VertexId bus_wait_start;
    graph::VertexId bus_wait_end;
};
 
struct RouteInfo {
    double total_time = 0.0;
    std::vector<std::variant<StopEdge, BusEdge>> edges;
};
 
}//end namespace domain
