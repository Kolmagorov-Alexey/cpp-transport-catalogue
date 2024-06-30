#pragma once
#include <deque>
#include <string>
#include <vector>
 
#include <iomanip>
#include <iostream>
 
#include <unordered_set>
#include <unordered_map>
 
#include "geo.h"

namespace transport_catalogue{
	
struct Bus;
    
struct Stop {    
    std::string name_;
    detail::geo::Coordinates coordinates;   
    std::vector<Bus*> buses_;
};
 
struct Bus { 
    std::string name_;
    std::vector<Stop*> stops_;
};
    
 struct BusInfoRoute {
    std::string_view name;
    bool not_found;
    size_t stops_count;
    size_t unique_stops_count;
    double length;
 };
    
 using StopMap = std:: unordered_map<std::string_view , Stop*>;
 using BusMap = std:: unordered_map<std::string_view , Bus*>;
 
class TransportCatalogue {
public:   
    void AddBus(const Bus& bus);
    void AddStop(const Stop& stop);
    BusInfoRoute  GetBusInfo( std::string_view _bus_name);
    Bus* GetBus(std::string_view _bus_name);
    Stop* GetStop(std::string_view _stop_name);
    std::unordered_set<const Bus*> StopGetUniqBuses(const Stop* stop);    
    std::unordered_set<const Stop*> GetUniqStops(const Bus* bus);
    double GetLength(Bus* bus);
private:
 
    std::deque<Stop> stops_;
    StopMap stopname_to_stop_;   
    std::deque<Bus> buses_;
    BusMap busname_to_bus_;
};
}//end namespace transport_catalogue